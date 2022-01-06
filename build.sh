#!/bin/bash
g++ main.cpp -o dapperdasher -I./raylib/include -L./raylib/lib -lraylib -Wl,-R./raylib/lib
