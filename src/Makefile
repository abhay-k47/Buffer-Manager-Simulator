all: queryProcessor dbUploader

queryProcessor: queryProcessor.o bufferManager.o
	g++ -Wall queryProcessor.o bufferManager.o -o queryProcessor

bufferManager.o: bufferManager.cpp bufferManager.hpp
	g++ -Wall -c bufferManager.cpp

queryProcessor.o: queryProcessor.cpp queryProcessor.hpp
	g++ -Wall -c queryProcessor.cpp

dbUploader: databaseUploader.cpp
	g++ -Wall databaseUploader.cpp -o dbUploader

clean:
	rm -rf *.o queryProcessor dbUploader
