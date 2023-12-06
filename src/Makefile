CXXFLAGS = -std=c++17 -DGL_GLEXT_PROTOTYPES

compile:
	g++ -o game ${CXXFLAGS} main.cpp lib/Game.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lGL -lglut

run_game:
	DISPLAY=:0 ./game

clean:
	rm -f game