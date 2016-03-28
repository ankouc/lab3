
all:
	gcc -g -o bin main.cpp -std=c++11 -Wall -fpermissive -lstdc++ -lm
clean:
	@rm -rf ./bin
