###
 # @Author: Haitian Li
 # @Date: 2021-05-17 22:40:54
 # @LastEditors: Haitian Li
 # @LastEditTime: 2021-05-18 05:08:12
 # @Description: Assignment4
### 


#!bin/bash

##check if the gevent, and 3 fifo been created properly.
echo "This test take about 10 sec, CONNECT test: gevent create domain & fifo"
name=test1
./test &
g_pid=$!
sleep 1
./writeto_gevent 0 TheDomain bob kk jack 
sleep 3
ls ./TheDomain > ./tests/test1_file_check_ls.out

kill -9 $g_pid
##only for test situation, since server assume always alive.
ps -axl | grep procchat | grep Z
killall test

diff ./server.txt ./tests/test1_server.out
diff ./tests/test1_file_check_ls.out ./tests/test1_file_check.out
rm -r TheDomain

if [ $? -eq "1" ]; then
    echo -e "\033[;31m [Failed] $name\033[0m"
else
	echo -e "\033[;32m [Passed] $name\033[0m"
fi



