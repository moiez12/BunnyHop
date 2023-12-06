CXXFLAGS = -std=c++11

compile:
	g++ -o game ${CXXFLAGS} main.cpp lib/Game.cpp -I/usr/local/include -L/usr/local/lib -lGL -lglut

run_game:
	DISPLAY=:0 ./game

clean:
	rm -f game