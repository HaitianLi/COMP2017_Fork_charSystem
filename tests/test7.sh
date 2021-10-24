#!bin/bash
###
 # @Author: Haitian Li
 # @Date: 2021-05-18 15:16:38
 # @LastEditors: Haitian Li
 # @LastEditTime: 2021-05-18 18:45:49
 # @Description: Assignment4
### 

##check disconnect
echo "This test take about under 15 sec, DISCONNECT test: check server log, check zombie"
name=test7
./test &
g_pid=$!
./writeto_gevent 0 TheDomain bob alice

./client_alice &
alice_pid=$!
./client_bob &
bob_pid=$!

sleep 1
./client_sender 7 0 "./TheDomain/bob_WR"
./client_sender 7 0 "./TheDomain/alice_WR"

sleep 10
##only for test situation, since server assume always alive.
kill -9 $g_pid
kill -9 $alice_pid
kill -9 $bob_pid
ps -axl | grep procchat | grep Z
killall test

diff ./server.txt ./tests/test7_server.out

if [ $? -eq "1" ]; then
    echo -e "\033[;31m [Failed] $name\033[0m"
else
	echo -e "\033[;32m [Passed] $name\033[0m"
fi

