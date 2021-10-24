#!bin/bash

#test the nested domain file
echo "This test take about 12 sec, CONNECT test: gevent creat nested domain & fifo"
name=test2
./test &
g_pid=$!
sleep 1
./writeto_gevent 0 AnotherDomain client1 client2 client3 client4
./writeto_gevent 0 AnotherDomain/nestedDomain client5 client6 client7

ls ./AnotherDomain > ./tests/test2_file_check_ls1.out
ls ./AnotherDomain/nestedDomain > ./tests/test2_file_check_ls2.out


sleep 4
kill -9 $g_pid
ps -axl | grep procchat | grep Z
##only for test situation, since server assume always alive.
killall test

diff ./server.txt ./tests/test2_server.out
# diff ./tests/test2_file_check_ls1.out ./tests/test2_file_check1.out
# diff ./tests/test2_file_check_ls2.out ./tests/test2_file_check.out

rm -r AnotherDomain
if [ $? -eq "1" ]; then
    echo -e "\033[;31m [Failed] $name\033[0m"
else
	echo -e "\033[;32m [Passed] $name\033[0m"
fi
