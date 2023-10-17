cd build/Intruder/
./intruder &
sleep 2

cd ../Client
./client &
sleep 2

cd ../Server
./service &
sleep 60

killall -e service
killall -e client
killall -e intruder