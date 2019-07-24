#include <stdio.h>
#include <stdlib.h>
#include "aaltogame.h"

GameArea *createGameArea(unsigned int x_size, unsigned int y_size) {
    GameArea *game = malloc(sizeof(GameArea));
    game->x_size = x_size;
    game->y_size = y_size;

    game->cells = malloc(y_size * sizeof(CellStatus *));

    if (!game->cells) {
		return NULL; // memory allocation failed
    }

    for (int j = 0; j < y_size; j++) {
		game->cells[j] = malloc(x_size * sizeof(CellStatus));
		if (!game->cells[j]) {
            // memory allocation failed, release memory
			// will have to go through previously allocated rows
			for (int i = 0; i < j; i++) {
				free(game->cells[i]);
			}
			free(game->cells);
			return NULL;
		}
        
        for (int i = 0; i < x_size; i++) {
            game->cells[j][i] = DEAD;
        }

	}
    return game;
}

GameArea *copyGameArea(const GameArea *game) {
    GameArea *new = createGameArea(game->x_size, game->y_size);
    for (int y = 0; y < new->y_size; y++) {
        for (int x = 0; x < new->x_size; x++) {
            new->cells[y][x] = game->cells[y][x];
        }
    }
    return new;
}


void releaseGameArea(GameArea *game) {
    for (int i = 0; i < game->y_size; i++) {
        free(game->cells[i]);
    }
    free(game->cells);
    free(game);
}

void printGameArea(const GameArea *game) {
    for (int y = 0; y < game->y_size; y++) {
        for (int x = 0; x < game->x_size; x++) {
            if (game->cells[y][x] == DEAD) {
                printf(".");
            } else {
                printf("*");
            }
        }
        printf("\n");
    }
}

void initGameArea(GameArea *game, unsigned int n) {
    int rand_y, rand_x;

    int i = 0;
    while (i<n) {
        rand_y = rand() % game->y_size;
        rand_x = rand() % game->x_size;

        if (game->cells[rand_y][rand_x] == DEAD) {
            game->cells[rand_y][rand_x] = ALIVE;
            i++;
        }

    }
}

void gameTick(GameArea *game) {
    GameArea *copy = copyGameArea(game);
    int aliveCount = 0;

    for (int y = 0; y < game->y_size; y++) {
        for (int x = 0; x < game->x_size; x++) {
            // Three y layers from -1 to + 1
            for (int test_y = y - 1; test_y <= y + 1; test_y++) {
                if (test_y < 0 || test_y >= game->y_size) {
                    // Outside game boundaries
                    continue;
                }
                // Three x layers from -1 to + 1
                for (int test_x = x - 1; test_x <= x + 1; test_x++) {
                    if (test_x < 0 || test_x >= game->x_size) {
                        // Outside game boundaries
                        continue;
                    }

                    if (test_x == x && test_y == y) {
                        // Cell it self is not a neighbour
                        continue;
                    }

                    // Count living neighbours
                    if (copy->cells[test_y][test_x] == ALIVE) {
                        aliveCount++;
                    }
                }
            }

            // Alive neighbours are counted, time for condition check
            int state = copy->cells[y][x];
            if (state == ALIVE) {
                if (!(aliveCount == 2 || aliveCount == 3)) {
                    game->cells[y][x] = DEAD;
                }
            } else if (state == DEAD) {
                if (aliveCount == 3) {
                    game->cells[y][x] = ALIVE;
                }
            }

            // Reset counter for next iteration
            aliveCount = 0;
        }
    }



    releaseGameArea(copy);
}