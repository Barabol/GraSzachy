run:
	./szachy
build:
	g++ ./main.cpp ./src/*.cpp ./src/*.h ./src/*.hpp -o szachy -lallegro_primitives -lallegro -lallegro_ttf -lallegro_font -lallegro_image -lallegro_audio -lallegro_acodec

clean:
	rm ./szachy
memtest:
	valgrind --leak-check=full ./szachy
debug:
	g++ -g ./main.cpp ./src/*.cpp ./src/*.h ./src/*.hpp -o szachy -lallegro_primitives -lallegro -lallegro_ttf -lallegro_font -lallegro_image -lallegro_audio -lallegro_acodec
	gdb ./szachy
