CC= /afs/nd.edu/user14/csesoft/new/bin/g++
CCFLAGS= -std=gnu++11
LDFLAGS= -lcurl
WFLAGS= -Wall


all: site-tester

site-tester: site-tester.cpp
	$(CC) $(WFLAGS) site-tester.cpp $(LDFLAGS) $(CCFLAGS) -o site-tester

clean:
	rm site-tester 
	rm *.csv
