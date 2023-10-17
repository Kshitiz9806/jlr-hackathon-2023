cd build/CopyCatIntruder/
./copy_service &
sleep 2

cd ../Server/
./service &
sleep 2

cd ../Client/
./client &
sleep 28

killall -e copy_service
killall -e service
killall -e client