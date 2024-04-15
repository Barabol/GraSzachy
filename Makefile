run:
	./szachy
build:
	g++ ./main.cpp ./src/*.cpp ./src/*.h -o szachy
clean:
	rm ./szachy
