cd build/Server/
./service &
sleep 2

cd ../Client/
./client &
sleep 60

killall -e client
killall -e service
