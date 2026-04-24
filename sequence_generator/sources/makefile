sequence_generator: function.o  main.o grammar.o
	g++ -std=c++17 -g -o sequence_generator function.o main.o grammar.o  -L /usr/local/boost_1_72_0/stage/lib/* -I /usr/local/boost_1_72_0/
	rm -rf *.o

grammar.o: include/grammar.cpp
	g++ -std=c++17 -g -o grammar.o -c include/grammar.cpp -W    -I /usr/local/boost_1_72_0/

function.o: include/function.cpp include/patients.h include/json.hpp include/bdd/serialization.cpp  include/bdd/Event.h include/bdd/Impact.h
	g++ -std=c++17 -g -o function.o -c include/function.cpp -W    -I /usr/local/boost_1_72_0/

main.o: main.cpp include/function.h include/patients.h include/json.hpp
	g++ -std=c++17 -g -o main.o -c main.cpp -W    -I /usr/local/boost_1_72_0/

clean:
		rm -rf *.o
