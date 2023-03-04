#!/bin/env sh
FLAGS='-g3 -O0 -Wall -Wextra -Wpedantic'
FLAGS="$FLAGS $(pkg-config --cflags --libs sdl2)"
FLAGS="$FLAGS $(pkg-config --cflags --libs SDL2_image)"
FLAGS="$FLAGS $(pkg-config --cflags --libs glib-2.0)"
CMD="gcc $FLAGS -o main main.c"

echo $CMD
$($CMD)
