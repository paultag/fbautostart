all:
	g++ -o ./bin/fbautostart ./src/fbautostart.cpp -I./include/ -D_DEBUG_MODE=1 -D_ON_BEHALF_OF=\"FLUXBOX\" -Wall
