rm ./bin/sfmlgame
# compile
g++ src/*.cpp -Isrc -std=c++17 -o bin/sfmlgame -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
# run
./bin/sfmlgame