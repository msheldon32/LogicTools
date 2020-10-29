truth_table: truth_table.o logic_engine.o parser.o
	g++ -g truth_table.o logic_engine.o parser.o -o truth_table

truth_table.o: truth_table.cpp logic_engine.h parser.h
	g++ -Wall -g -c truth_table.cpp

logic_engine.o: logic_engine.cpp logic_engine.h
	g++ -Wall -g -c logic_engine.cpp

parser.o: parser.cpp parser.h
	g++ -Wall -g -c parser.cpp
