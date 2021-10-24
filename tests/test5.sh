#!bin/bash

##check if the gevent, and 3 fifo been created properly.
echo "This test take about under 15 sec, SAY RECV test: 4 clients 2 domains"
name=test5
./test &
g_pid=$!
./client_alice &
alice_pid=$!
./client_bob &
bob_pid=$!
./client_jack &
jack_pid=$!
./client_li &
li_pid=$!

sleep 1
#1 for (type say), 0 for(terminal code)
./writeto_gevent 0 TheDomain bob alice
./writeto_gevent 0 AnotherDomain jack li

./client_sender 1 0 "./TheDomain/bob_WR"
./client_sender 1 0 "./TheDomain/alice_WR"
./client_sender 1 0 "./AnotherDomain/jack_WR"
./client_sender 1 0 "./AnotherDomain/li_WR"

sleep 6
##only for test situation, since server assume always alive.
kill -9 $g_pid
kill -9 $alice_pid
kill -9 $bob_pid
kill -9 $jack_pid
kill -9 $li_pid
ps -axl | grep procchat | grep Z
killall test
rm -r TheDomain
rm -r AnotherDomain

diff ./server.txt ./tests/test5_server.out
diff ./alice.txt ./tests/test5_alice_expect.out
diff ./bob.txt ./tests/test5_bob_expect.out
diff ./jack.txt ./tests/test5_jack_expect.out
diff ./li.txt ./tests/test5_li_expect.out


if [ $? -eq "1" ]; then
    echo -e "\033[;31m [Failed] $name\033[0m"
else
	echo -e "\033[;32m [Passed] $name\033[0m"
fi

