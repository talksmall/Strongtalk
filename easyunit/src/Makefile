all: defaulttestprinter.o simplestring.o test.o testcase.o testfailure.o testpartresult.o testregistry.o testresult.o testrunner.o
	ar r libeasyunit.a defaulttestprinter.o simplestring.o test.o testcase.o testfailure.o testpartresult.o testregistry.o testresult.o testrunner.o
	rm -f defaulttestprinter.o simplestring.o test.o testcase.o testfailure.o testpartresult.o testregistry.o testresult.o testrunner.o

defaulttestprinter.o: defaulttestprinter.cpp defaulttestprinter.h
	g++ -c defaulttestprinter.cpp -o defaulttestprinter.o

simplestring.o: simplestring.cpp simplestring.h
	g++ -c simplestring.cpp -o simplestring.o

test.o: test.cpp test.h
	g++ -c test.cpp -o test.o

testcase.o: testcase.cpp testcase.h 
	g++ -c testcase.cpp -o testcase.o
	
testfailure.o: testfailure.cpp testfailure.h  
	g++ -c testfailure.cpp -o testfailure.o

testpartresult.o: testpartresult.cpp testpartresult.h 
	g++ -c testpartresult.cpp -o testpartresult.o

testregistry.o: testregistry.cpp testregistry.h  
	g++ -c testregistry.cpp -o testregistry.o

testresult.o: testresult.cpp testresult.h
	g++ -c testresult.cpp -o testresult.o

testrunner.o: testrunner.cpp testrunner.h
	g++ -c testrunner.cpp -o testrunner.o
	
clean:
	rm -f defaulttestprinter.o simplestring.o test.o testcase.o testfailure.o testpartresult.o testregistry.o testresult.o testrunner.o libeasyunit.a
