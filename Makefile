run:
	./szachy
build:
	g++ ./main.cpp ./src/*.cpp ./src/*.h ./src/*.hpp -o szachy
clean:
	rm ./szachy
memtest:
	valgrind --leak-check=full ./szachy
