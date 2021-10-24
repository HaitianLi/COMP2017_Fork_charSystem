CC=gcc
CFLAGS=-Wall -Werror -Wvla -std=gnu11 -fsanitize=address
PFLAGS=-fprofile-arcs -ftest-coverage
DFLAGS=-g
HEADERS=server.h
SRC=server.c

procchat: $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(DFLAGS) $(SRC) -o $@

test: procchat gev client
	$(CC) $(CFLAGS) $(PFLAGS) $(SRC) -o $@
	bash run_bash.sh
	gcov server.gcno

gev:
	gcc -o writeto_gevent writeto_gevent.c

client:
	gcc -o client_sender client_sender.c
	gcc -o client_alice client_alice.c
	gcc -o client_bob client_bob.c
	gcc -o client_jack client_jack.c
	gcc -o client_li client_li.c

clean:
	rm -f procchat &
	rm gevent &
	rm writeto_gevent &
	rm -r TheDomain/ &
	rm client_sender &
	rm client_alice &
	rm client_bob &
	rm client_jack &
	rm client_li &
	rm test &
	rm server.gcda &
	rm server.gcno &
	rm server.c.gcov &
	rm _fd_def.h.gcov

# git:
# 	git add .
# 	git commit -m "try"
# 	git push

