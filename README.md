### Lecture 2 & 3
```bash
g++ -std=c++17 lecture2.cpp -o lecturecode
```
### Lecture 4
```bash
g++ src/*.cpp -Isrc -std=c++17 -o bin/sfmlgame -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
cd bin
./sfmlgame
```
run with `run.sh` script using `./run.sh` 