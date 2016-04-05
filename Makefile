
all:
	gcc -g -o bin main.cpp -Ofast -std=c++11 -Wall -fpermissive -lstdc++ -lm
clean:
	@rm -rf ./bin
