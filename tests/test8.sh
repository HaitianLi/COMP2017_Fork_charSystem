#!bin/bash
###
 # @Author: Haitian Li
 # @Date: 2021-05-18 15:16:38
 # @LastEditors: Haitian Li
 # @LastEditTime: 2021-05-18 18:50:14
 # @Description: Assignment4
### 

##check disconnect
echo "This test take about 60 sec, PING & PONG test: 2 client, 45 sec connection"
name=test8
./test &
g_pid=$!
./writeto_gevent 0 TheDomain bob alice

./client_alice &
alice_pid=$!
./client_bob &
bob_pid=$!

sleep 1
./client_sender 1 0 "./TheDomain/bob_WR"
./client_sender 1 0 "./TheDomain/alice_WR"


## 45 second should send back and force 3 times
sleep 50
##only for test situation, since server assume always alive.
kill -9 $g_pid
kill -9 $alice_pid
kill -9 $bob_pid
ps -axl | grep procchat | grep Z
killall test
rm -r TheDomain

diff ./alice_ping.txt ./tests/test8_alice_ping.out
diff ./bob_ping.txt ./tests/test8_bob_ping.out

if [ $? -eq "1" ]; then
    echo -e "\033[;31m [Failed] $name\033[0m"
else
	echo -e "\033[;32m [Passed] $name\033[0m"
fi

