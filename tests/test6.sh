#!bin/bash
###
 # @Author: Haitian Li
 # @Date: 2021-05-18 15:16:38
 # @LastEditors: Haitian Li
 # @LastEditTime: 2021-05-18 19:17:23
 # @Description: Assignment4
### 

##the say cont have the similar process as say, the only importtant is the order
echo "This test take about under 15 sec, SAY RECV CONT test: test the order of cont"
name=test6
./test &
g_pid=$!
./client_alice &
alice_pid=$!
./client_bob &
bob_pid=$!

./writeto_gevent 0 TheDomain bob alice
sleep 2 
./client_sender 2 1 "./TheDomain/bob_WR"
./client_sender 2 2 "./TheDomain/bob_WR"
./client_sender 2 3 "./TheDomain/bob_WR"
sleep 1
./client_sender 2 1 "./TheDomain/alice_WR"
./client_sender 2 2 "./TheDomain/alice_WR"

sleep 4 
##only for test situation, since server assume always alive.
kill -9 $g_pid
kill -9 $alice_pid
kill -9 $bob_pid
ps -axl | grep procchat | grep Z
killall test
rm -r TheDomain


diff ./server.txt ./tests/test6_server.out
diff ./alice.txt ./tests/test6_alice_expect.out
diff ./bob.txt ./tests/test6_bob_expect.out

if [ $? -eq "1" ]; then
    echo -e "\033[;31m [Failed] $name\033[0m"
else
	echo -e "\033[;32m [Passed] $name\033[0m"
fi

