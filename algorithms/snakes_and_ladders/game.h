#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

class board {
    public:
        int** AM; //adjacency matrix

        int** snakes;
        int** ladders;
        int num_snakes;
        int num_ladders;
        board(int snakes, int ladders);
        ~board();
};
