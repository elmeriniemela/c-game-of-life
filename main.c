#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "aaltogame.h"





int main(int argc, char *argv[]) {

    if (argc != 2) {
        char *filename = "./game-of-life.out";
        if (argc > 0) {
            filename = argv[0];
        }

        printf("Specify the amount of rounds as a command line argument i.e  '%s 10'\n", filename);
        return 0;
    }

    int rounds = atoi(argv[1]);
    if (rounds == 0) {
        printf("Invalid integer for the amount of rounds: '%s'\n", argv[1]);
    }


    // re-seed random number generator
    srand((unsigned)time(NULL));

    GameArea *a = createGameArea(50,20);
    if (a == NULL) {
        printf("Allocating memory failed\n");
        return -1;
    }

    initGameArea(a, 150);

    // loop iterations, cancel with ctrl-c
    for(int i=0; i<rounds; i++) {
        printf("\nGeneration: %d\n", i+1);
        printGameArea(a);
        // slow down iterations
        sleep(1);
        gameTick(a);
    }
    releaseGameArea(a);
    return 0;
}
