// Copyright (C) 2015-2017 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
#include <vsomeip/vsomeip.hpp>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#if defined ANDROID || defined __ANDROID__
#include "android/log.h"
#define LOG_TAG "copy_service"
#define LOG_INF(...) fprintf(stdout, __VA_ARGS__), fprintf(stdout, "\n"), (void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, ##__VA_ARGS__)
#define LOG_ERR(...) fprintf(stderr, __VA_ARGS__), fprintf(stderr, "\n"), (void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, ##__VA_ARGS__)
#else
#include <cstdio>
#define LOG_INF(...) fprintf(stdout, __VA_ARGS__), fprintf(stdout, "\n")
#define LOG_ERR(...) fprintf(stderr, __VA_ARGS__), fprintf(stderr, "\n")
#endif



class copy_service {
public:
    // Get the vSomeIP runtime and
    // create a application via the runtime, we could pass the application name
    // here otherwise the name supplied via the VSOMEIP_APPLICATION_NAME
    // environment variable is used
    copy_service() :
                    rtm_(vsomeip::runtime::get()),
                    app_(rtm_->create_application("service", "../../CopyCatIntruder/local.json")),
                    stop_(false),
                    stop_thread_(std::bind(&copy_service::stop, this))
    {
        std::ifstream config_file("../../CopyCatIntruder/config.json");

        if(!config_file)
        {
            LOG_ERR("Could not open the config file");
        }
        else{
            config_file >> config_data_;
            config_file.close();

            service_id_ = config_data_["service_id_"];
            service_instance_id_ = config_data_["service_instance_id_"];

            LOG_INF("Service id: %d", service_id_);
            LOG_INF("Service instance id: %d", service_instance_id_);
        }

        set_state();
        
    }

    ~copy_service()
    {
        stop_thread_.join();
    }

    void set_state()
    {
        if(service_id_ != 0 || service_instance_id_ != 0)
        {
            state_ = 1;
        }
        
    }

    void on_any_availibility_cbk(vsomeip::service_t _service,
            vsomeip::instance_t _instance, bool _is_available)
    {
        // if(_instance == 0xFFFF)
        // {
        //     return;
        // }
        service_id_ = _service;
        LOG_INF("Setting Service id: %d", service_id_);


        service_instance_id_ = _instance;
        LOG_INF("Setting Service instance id: %d", service_instance_id_);

        // json config_data;
        // config_data["service_id_"] = service_id_;
        // config_data["service_instance_id_"] = service_instance_id_;

        // std::ofstream config_file("../../CopyCatIntruder/config.json");
        // config_file << config_data.dump(4);
        // config_file.close();

        // json local_data;
        // std::ifstream local_file("../../CopyCatIntruder/local.json");
        // local_file>>local_data;
        // local_file.close();

        // local_data["applications"][0]["id"] = "0x4444";
        // local_data["routing"] = "service";

        // std::ofstream local_output("../../CopyCatIntruder/local.json");
        // local_output<<local_data.dump(4);
        // local_output.close();
        run_();
    }

    void run_()
    {
        while(true)
        {
            app_->request_service(service_id_, service_instance_id_);
            // {
            //     // The service is available then we send the request
            //     // Create a new request
            //     std::shared_ptr<vsomeip::message> rq = rtm_->create_request();
            //     // Set the hello world service as target of the request
            //     rq->set_service(service_id_);
            //     rq->set_instance(service_instance_id_);
            //     rq->set_method(service_method_id_);

            //     // Create a payload which will be sent to the service
            //     std::shared_ptr<vsomeip::payload> pl = rtm_->create_payload();
            //     std::string str("compromised");
            //     std::vector<vsomeip::byte_t> pl_data(std::begin(str), std::end(str));

            //     pl->set_data(pl_data);
            //     rq->set_payload(pl);
            //     // Send the request to the service. Response will be delivered to the
            //     // registered message handler
            //     LOG_INF("Sending: %s", str.c_str());
            //     app_->send(rq);
            // }
            LOG_INF("Copy intruder waiting...");
            std::this_thread::sleep_for(std::chrono::seconds(2));
            if(exit_)
            {
                return;
            }
        }
    }

    bool init()
    {
        // init the application
        if (!app_->init()) {
            LOG_ERR("Couldn't initialize application");
            return false;
        }

        if(state_ == 0)
        {
            LOG_INF("Waiting to get he ids");
            app_->register_availability_handler(0x1112, vsomeip::ANY_INSTANCE, std::bind(&copy_service::on_any_availibility_cbk, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            LOG_INF("Registered the handler");
            return true;
        }

        // register a message handler callback for messages sent to our service
        app_->register_message_handler(service_id_, service_instance_id_,
                service_method_id_,
                std::bind(&copy_service::on_message_cbk, this,
                        std::placeholders::_1));

        // register a state handler to get called back after registration at the
        // runtime was successful
        app_->register_state_handler(
                std::bind(&copy_service::on_state_cbk, this,
                        std::placeholders::_1));
        return true;
    }

    void start()
    {
        app_->start();
    }

    void stop()
    {
        std::unique_lock<std::mutex> its_lock(mutex_);
        while(!stop_) {
            condition_.wait(its_lock);
        }
        exit_ = true;
        LOG_INF("Terminating the CopyIntruder");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        // Stop offering the service
        app_->stop_offer_service(service_id_, service_instance_id_);
        // // unregister the state handler
        app_->unregister_state_handler();
        // // unregister the message handler
        app_->unregister_message_handler(service_id_, service_instance_id_,
                service_method_id_);
        // // shutdown the application
        app_->stop();
    }

    void terminate() {
        std::lock_guard<std::mutex> its_lock(mutex_);
        stop_ = true;
        condition_.notify_one();
    }

    void on_state_cbk(vsomeip::state_type_e _state)
    {
        if(_state == vsomeip::state_type_e::ST_REGISTERED)
        {
            // we are registered at the runtime and can offer our service
            LOG_INF("Copy Cat Intruder at a desired state");
            app_->offer_service(service_id_, service_instance_id_);
        }
    }

    void on_message_cbk(const std::shared_ptr<vsomeip::message> &_request)
    {
        // Create a response based upon the request
        std::shared_ptr<vsomeip::message> resp = rtm_->create_response(_request);

        // Construct string to send back
        std::string str("");
        str.append(
                reinterpret_cast<const char*>(_request->get_payload()->get_data()),
                0, _request->get_payload()->get_length());

        // Create a payload which will be sent back to the client
        int location = stoi(str);
        if(location<30)
        {
            location = location - 1;
            writeFile(location, "left");

        }
        if(location==30)
        {
            location = location - 1;
            writeFile(location, "left");
        }
        if(location > 30)
        {
            location = location + 1;
            writeFile(location, "right");
        }
        LOG_INF("Requested location log: %d", location);
        // Create a payload which will be sent back to the client
        str = std::to_string(location);
        std::shared_ptr<vsomeip::payload> resp_pl = rtm_->create_payload();
        std::vector<vsomeip::byte_t> pl_data(str.begin(), str.end());
        resp_pl->set_data(pl_data);
        resp->set_payload(resp_pl);

        // Send the response back
        app_->send(resp);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        LOG_INF("Sending response to client");
        // we have finished
        // terminate();
    }

private:
    std::shared_ptr<vsomeip::runtime> rtm_;
    std::shared_ptr<vsomeip::application> app_;
    bool stop_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::thread stop_thread_;

    vsomeip::service_t service_id_;
    vsomeip::instance_t service_instance_id_;
    vsomeip::method_t service_method_id_ = 0x3333;

    json config_data_;
    int state_ = 0;
    bool exit_ = false;


    void writeFile(int left, std::string steer) {
        std::string pref = "[COPY CAT INTRUDER]";
        std::string path = "../../jlr-hackathon-2023/src/Components/logs_data/log_attacker_dummy_1.log";
        std::ofstream fp;
        fp.open(path, std::ios_base::app);
        fp << "\n" << pref << " Where to move info: " << left << " " << steer;
        fp.close();
    }
};
