set -e

if [ -f Game ]; then
    rm Game
fi

if [ -f maze.o ]; then
    rm maze.o
fi

if [ -f utils.o ]; then
    rm utils.o
fi

if [ -f player.o ]; then
    rm player.o
fi

gcc -c maze.c -o maze.o
gcc -c utils.c -o utils.o
gcc -c player.c -o player.o

gcc main.c maze.o utils.o player.o -o Game -Wno-unused -lncurses

./Game