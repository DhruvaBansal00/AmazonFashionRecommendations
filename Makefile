#  output: main.o garage.o
#         g++ -std=c++0x -Wall main.o garage.o -o output

#  main.o: main.cpp
#         g++ -c main.cpp

#  garage.o: garage.cpp garage.hpp
#         g++ -c garage.cpp

readDataset: readDataset.cpp
	g++ -o readDataset readDataset.cpp -lstdc++fs -ljsoncpp
 clean:
	rm *.o readDataset