# Choose the heuristic to compile with
# CHOSEN_H=H_DISKS
# CHOSEN_H=H_MOBI
# CHOSEN_H=H_POSI
# CHOSEN_H=H_COMBI
CHOSEN_H=H_TIMEREL

all: clean otello_test

debug: clean otello_test_debug

otello_test: otello_test.cpp otello.o minimax.o alphabeta.o heuristic.o
	g++ -Wall otello.o minimax.o alphabeta.o heuristic.o otello_test.cpp -o otello_test.bin

otello_test_debug: otello_test.cpp otello.o minimax.o alphabeta.o heuristic.o
	g++ -Wall -g otello.o minimax.o alphabeta.o heuristic.o otello_test.cpp -o otello_test_debug.bin

otello.o: otello.cpp otello.h
	g++ -Wall -c otello.cpp

minimax.o: minimax.cpp minimax.h otello.h
	g++ -Wall -c minimax.cpp

alphabeta.o: alphabeta.cpp alphabeta.h otello.h
	g++ -Wall -c alphabeta.cpp

heuristic.o: heuristic.cpp heuristic.h otello.h
	g++ -Wall -D${CHOSEN_H} -c heuristic.cpp

clean:
	-rm -f *.o
	-rm -f *.bin

