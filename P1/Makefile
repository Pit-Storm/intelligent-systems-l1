# Uncomment one of this to compile the program with each heuristis
# Non-informative
CHOICE=ni
# Any other: give it a name that matches your 'heuristic.cpp'
# CHOICE=euklid

CHOSEN_H=${CHOICE}_HEURISTIC

lab: lab.cpp Maze.o heuristic.o
	g++ -Wall Maze.o heuristic.o -D${CHOSEN_H} lab.cpp -o lab_${CHOICE}.bin

lab_debug: lab.cpp Maze.o heuristic.o
	g++ -Wall -g Maze.o heuristic.o -D${CHOSEN_H} lab.cpp -o lab_${CHOICE}_debug.bin

Maze.o: Maze.cpp Maze.h
	g++ -Wall -c Maze.cpp

heuristic.o: heuristic.cpp heuristic.h
	g++ -Wall -D${CHOSEN_H} -c heuristic.cpp

# Removes the objects
clean:
	-rm *.o

# Removes the executables, too
allclean:
	-rm *.o
	-rm *.bin
