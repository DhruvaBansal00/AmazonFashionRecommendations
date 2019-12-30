#  output: main.o garage.o
#         g++ -std=c++0x -Wall main.o garage.o -o output

#  main.o: main.cpp
#         g++ -c main.cpp

#  garage.o: garage.cpp garage.hpp
#         g++ -c garage.cpp

main: readDataset.o metrics.o knn.o splitData.o main.cpp 
	g++ readDataset.o metrics.o knn.o splitData.o main.cpp -o main -lstdc++fs

readDataset.o: readDataset.cpp readDataset.h
	g++ -c readDataset.cpp -lstdc++fs

metrics.o: metrics.cpp metrics.h
	g++ -c metrics.cpp 

knn.o : knn.cpp knn.h
	g++ -c knn.cpp

splitData.o : splitData.cpp splitData.h
	g++ -c splitData.cpp

 clean:
	rm *.o