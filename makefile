#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# g++ -L /usr/lib -l uhd -o e100test test_routines.cpp
# g++ -g -L /usr/lib -l uhd -o rxtest  receiver_test.cpp uhd_utilities.cpp
# g++ -g -L /usr/lib -l uhd -o serial_port_test serial_port_test.cpp
# g++ -pthread -o thread_test thread_test.cpp





e100test: test_routines.o
	g++ -L /usr/lib -l uhd -o e100test test_routines.cpp

rxtest: receiver_test.o uhd_utilities.o
	g++ -g -L /usr/lib -l uhd -o rxtest  receiver_test.cpp uhd_utilities.cpp
	
serialtest: serial_port_test.o 	
	g++ -g -L /usr/lib -l uhd -o serial_port_test serial_port_test.cpp
