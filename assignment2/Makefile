CC=g++

ass2: src/*.cpp
	$(CC) src/*.cpp -Isrc -std=c++17 -o bin/main -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

run: bin/main
	./bin/main

clean:
	rm -f bin/main