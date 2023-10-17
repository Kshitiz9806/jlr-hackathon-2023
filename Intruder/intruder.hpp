// Copyright (C) 2015-2017 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
#include <vsomeip/vsomeip.hpp>
#include <thread>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#if defined ANDROID || defined __ANDROID__
#include "android/log.h"
#define LOG_TAG "Intruder"
#define LOG_INF(...) fprintf(stdout, __VA_ARGS__), fprintf(stdout, "\n"), (void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, ##__VA_ARGS__)
#define LOG_ERR(...) fprintf(stderr, __VA_ARGS__), fprintf(stderr, "\n"), (void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, ##__VA_ARGS__)
#else
#include <cstdio>
#define LOG_INF(...) fprintf(stdout, __VA_ARGS__), fprintf(stdout, "\n")
#define LOG_ERR(...) fprintf(stderr, __VA_ARGS__), fprintf(stderr, "\n")
#endif



class Intruder {
public:
    // Get the vSomeIP runtime and
    // create a application via the runtime, we could pass the application name
    // here otherwise the name supplied via the VSOMEIP_APPLICATION_NAME
    // environment variable is used
    Intruder() :
                    rtm_(vsomeip::runtime::get()),
                    app_(rtm_->create_application("intruder", "../../Intruder/local.json"))
    {
    }

    bool init(){
        // init the application
        if (!app_->init()) {
            LOG_ERR ("Couldn't initialize application");
            return false;
        }

        // register a state handler to get called back after registration at the
        // runtime was successful
        app_->register_state_handler(
                std::bind(&Intruder::on_state_cbk, this,
                        std::placeholders::_1));

        
        // register a callback which is called as soon as the service is available
        app_->register_availability_handler(service_id_, service_instance_id_,
                std::bind(&Intruder::on_availability_cbk, this,
                        std::placeholders::_1, std::placeholders::_2,
                        std::placeholders::_3));
        app_->register_availability_handler(0x1112, vsomeip::ANY_INSTANCE, std::bind(&Intruder::on_any_availibility_cbk, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        return true;
    }

    void start()
    {
        // start the application and wait for the on_event callback to be called
        // this method only returns when app_->stop() is called
        app_->start();
    }

    void on_state_cbk(vsomeip::state_type_e _state)
    {
        if(_state == vsomeip::state_type_e::ST_REGISTERED)
        {
            // we are registered at the runtime now we can request the service
            // and wait for the on_availability callback to be called
            LOG_INF("[---------] State set to registered");
            // run();
        }
    }

    void on_availability_cbk(vsomeip::service_t _service,
            vsomeip::instance_t _instance, bool _is_available)
    {
        // Check if the available service is the the hello world service
        if(service_id_ == _service && service_instance_id_ == _instance
                && _is_available)
        {
            // The service is available then we send the request
            // Create a new request
            std::shared_ptr<vsomeip::message> rq = rtm_->create_request();
            // Set the hello world service as target of the request
            rq->set_service(service_id_);
            rq->set_instance(service_instance_id_);
            rq->set_method(service_method_id_);

            // Create a payload which will be sent to the service
            std::shared_ptr<vsomeip::payload> pl = rtm_->create_payload();
            std::string str("0");
            std::vector<vsomeip::byte_t> pl_data(std::begin(str), std::end(str));

            pl->set_data(pl_data);
            rq->set_payload(pl);
            // Send the request to the service. Response will be delivered to the
            // registered message handler
            LOG_INF("Sending: %s", str.c_str());
            writeFile(0);
            app_->send(rq);
        }
    }

    void on_message_cbk(const std::shared_ptr<vsomeip::message> &_response)
    {
        if(service_id_ == _response->get_service()
                && service_instance_id_ == _response->get_instance()
                && vsomeip::message_type_e::MT_RESPONSE
                        == _response->get_message_type()
                && vsomeip::return_code_e::E_OK == _response->get_return_code())
        {
            // Get the payload and print it
            std::shared_ptr<vsomeip::payload> pl = _response->get_payload();
            std::string resp = std::string(
                    reinterpret_cast<const char*>(pl->get_data()), 0,
                    pl->get_length());
            LOG_INF("Received: %s", resp.c_str());
            // stop();
        }

    }

    void on_any_availibility_cbk(vsomeip::service_t _service,
            vsomeip::instance_t _instance, bool _is_available)
    {
        if(_service == 0xFFFF)
        {
            return;
        }
        service_id_ = _service;
        LOG_INF("Setting Service id: %d", service_id_);


        service_instance_id_ = _instance;
        LOG_INF("Setting Service instance id: %d", service_instance_id_);

        json config_data;
        config_data["service_id_"] = service_id_;
        config_data["service_instance_id_"] = service_instance_id_;

        std::ofstream config_file("../../CopyCatIntruder/config.json");
        config_file << config_data.dump(4);
        config_file.close();

        // register a callback for responses from the service
        app_->register_message_handler(vsomeip::ANY_SERVICE,
                service_instance_id_, vsomeip::ANY_METHOD,
                std::bind(&Intruder::on_message_cbk, this,
                        std::placeholders::_1));
        run();

    }

    void run()
    {
        int i = 0;
        while(true)
        {
            app_->request_service(service_id_, service_instance_id_);
            {
                // The service is available then we send the request
                // Create a new request
                std::shared_ptr<vsomeip::message> rq = rtm_->create_request();
                // Set the hello world service as target of the request
                rq->set_service(service_id_);
                rq->set_instance(service_instance_id_);
                rq->set_method(service_method_id_);

                // Create a payload which will be sent to the service
                std::shared_ptr<vsomeip::payload> pl = rtm_->create_payload();
                std::string str("0");
                std::vector<vsomeip::byte_t> pl_data(std::begin(str), std::end(str));

                pl->set_data(pl_data);
                rq->set_payload(pl);
                // Send the request to the service. Response will be delivered to the
                // registered message handler
                LOG_INF("Sending: %s", str.c_str());
                if(i%20 == 0)
                {
                    writeFile(0);
                }
                app_->send(rq);
            }
            LOG_INF("Intruder waiting...");
            std::this_thread::sleep_for(std::chrono::milliseconds(39));
            if(exit_)
            {
                return;
            }
            i++;
        }
    }

    void stop()
    {
        exit_ = true;
        // unregister the state handler
        app_->unregister_state_handler();
        // unregister the message handler
        app_->unregister_message_handler(vsomeip::ANY_SERVICE,
                service_instance_id_, vsomeip::ANY_METHOD);
        // alternatively unregister all registered handlers at once
        app_->clear_all_handler();
        // release the service
        app_->release_service(service_id_, service_instance_id_);
        // shutdown the application
        app_->stop();
    }

private:
    std::shared_ptr<vsomeip::runtime> rtm_;
    std::shared_ptr<vsomeip::application> app_;
    bool exit_ = false;

    vsomeip::service_t service_id_ = 0x1000;
    vsomeip::service_t service_instance_id_ = 0x2;
    vsomeip::method_t service_method_id_ = 0x3333;

    void writeFile(int left) {
        std::string pref = "[INTRUDER CLIENT]";
        std::string path = "../../jlr-hackathon-2023/src/Components/logs_data/log_attacker_dummy_1.log";
        std::ofstream fp;
        fp.open(path, std::ios_base::app);
        fp << "\n" << pref << " Current Location: " << left;
        fp.close();
    }
};
