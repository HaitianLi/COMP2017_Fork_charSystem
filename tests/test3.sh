#!bin/bash

##check if the gevent, and 3 fifo been created properly.
echo "This test take about 10 sec, CONNECT test: dupplicated connection"
name=test3
./test &
g_pid=$!
sleep 1
./writeto_gevent 0 TheDomain bob kk jack 
./writeto_gevent 0 TheDomain kk jack 
sleep 3
##only for test situation, since server assume always alive.
kill -9 $g_pid
ps -axl | grep procchat | grep Z
killall test

diff ./server.txt ./tests/test3_server.out
rm -r TheDomain
if [ $? -eq "1" ]; then
    echo -e "\033[;31m [Failed] $name\033[0m"
else
	echo -e "\033[;32m [Passed] $name\033[0m"
fi

