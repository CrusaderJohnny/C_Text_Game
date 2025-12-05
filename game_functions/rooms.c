#include "rooms.h"
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int get_input(int max_choice) {
    char buffer[100];
    while (1) {
        printf("\n> Select an option (1-%d): ", max_choice);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            char *endptr;
            errno = 0;
            const long val = strtol(buffer, &endptr, 10);
            if (val >= 1 && val <= max_choice) {
                return (int)val;
            }
        }
        printf("Invalid choice, Please enter a valid number between 1 and %d.\n", max_choice);
    }
}


void print_header(char *title) {
    printf("\n=== %s ===\n", title);
}

void print_image(char *file) {
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        return;
    }
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        putchar(ch);
    }
    fclose(fp);
}

void handle_cell(gameState_t *state) {
    print_header("THE PRISON CELL");
    print_image("../images/door.txt");
    printf("\n\nYou are in a damp, dark cell. The door is locked.\n");
    printf("There is a loose stone in the wall.\n");

    printf("\n1. Inspect the loose stone\n");
    printf("2. Examine the door\n");
    printf("3. Sleep on the floor (Rest)\n");

    int choice = get_input(3);

    if (choice == 1) {
        printf("\nYou wiggle the stone out of its spot in the wall...");
        if (state->has_key) {
            printf("\nThe hole behind the stone is empty.\n");
            printf("\nPress ENTER to continue.");
            getchar();
        } else {
            printf("\nAnd find a rusty key!\n");
            state->has_key = true;
            printf("\nPress ENTER to continue.");
            getchar();
        }
    } else if (choice == 2) {
        printf("\nYou approach the door...\n");
        if (state->has_key) {
            printf("The key fits in the keyhole, you turn the key and exit the cell\n");
            state->location = ROOM_HALLWAY;
            printf("\nPress ENTER to continue.");
            getchar();
        } else {
            printf("The door is locked.\n");
            if (!state->has_key) {
                printf("If only this room had a secret.\n");
                printf("\nPress ENTER to continue.");
                getchar();
            }
        }
    } else if (choice == 3) {
        printf("\nYou fall asleep. As you rest up, a rat bites your toe.\n");
        printf("Health -10\n");
        state->health -= 10;
        printf("Current health: %d\n", state->health);
        printf("\nPress ENTER to continue.");
        getchar();
    }
}

void handle_armory(gameState_t *state) {
    print_header("THE ARMORY");
    printf("\nAs you approach the door, you notice the chain is not actually locked.\n");
    printf("You remove the chain and the swing open the door.\n");
    printf("Racks of weapons line the walls, but most are broken.\n");

    printf("\n1. Search for a weapon\n");
    printf("2. Return to the hallway\n");

    int choice = get_input(2);

    if (choice == 1) {
        if (state->has_sword) {
            printf("You already found the best weapon here.\n");
            printf("\nPress ENTER to continue.");
            getchar();
        } else {
            printf("\nYou find a sharp Short Sword hidden under a pile of shields!\n\n");
            print_image("../images/sword.txt");
            state->has_sword = true;
            printf("\n\nPress ENTER to continue.");
            getchar();
        }
    } else if (choice == 2) {
        state->location = ROOM_HALLWAY;
    }
}

void handle_hallway(gameState_t *state) {
    print_header("THE HALLWAY");
    print_image("../images/hallway.txt");
    printf("\n\nTorches line the walls.\n");
    printf("To the North, the hallway stretches beyond a bend.\n");
    printf("To the East, a door that appears to be locked with a chain.\n");

    printf("\n1. Head North\n");
    printf("2. Head East\n");
    printf("4. Return to your cell (South)\n");

    int choice = get_input(3);
    if (choice == 1) {
        state->location = ROOM_HALLWAY2;
    } else if (choice == 2) {
        state->location = ROOM_ARMORY;
    } else if (choice == 3) {
        state->location = ROOM_CELL;
    }
}

void handle_hallway2(gameState_t *state) {
    print_header("THE HALLWAY CONTINUES");
    printf("\nThe hallway continues for a bit before another crossroads.\n");
    printf("To the North, the hallway seems to continue onwards.\n");
    printf("To the East and West, the hallway stretches and then turns out of sight.\n");

    printf("\n1. Head North.\n");
    printf("2. Head East.\n");
    printf("3. Head West.\n");
    printf("4. Head back the way you came.(South)\n");

    int choice = get_input(4);
    if (choice == 1) {
        state->location = ROOM_HALLWAY_END;
    } else if (choice == 2) {
        state->location = ROOM_HALLWAY3;
    } else if (choice == 3) {
        state->location = ROOM_HALLWAY4;
    } else if (choice == 4) {
        state->location = ROOM_HALLWAY;
    }
}

void handle_hallway3(gameState_t *state) {
    print_header("THE HALLWAY CONTINUES");
    printf("As you head East, You get to another room full of cells.\n");

    printf("\n1. Explore the room to find an exit.\n");
    printf("2. Inspect the Cells.\n");
    printf("3. Return to the previous hallway.\n");

    int choice = get_input(3);
    if (choice == 1) {
        printf("\nYou explore the cell for an exit\n");
        if (state->visited_cells) {
            printf("Just like the other cell, this is also a dead end\n");
            printf("\nPress ENTER to continue.");
            getchar();
        } else {
            printf("The room appears to be a dead-end.\n");
            printf("\nPress ENTER to continue.");
            getchar();
        }
    } else if (choice == 2) {
        if (state->visited_cells) {
            printf("\nYou approach quietly like the other cell.\n");
            printf("More starving prisoners and a few empty cells.\n");
            printf("When searching the empty cells in this room, you find a key hidden behind a loose stone\n");
            state->has_exit_key = true;
            printf("\nPress ENTER to continue.");
            getchar();
        } else {
            printf("\nYou approach the cells with breathing carefully.\n");
            printf("You peek inside and see starving prisoners.\n");
            printf("A few cells are empty.\nYou think back to your cell and search for a loose stone.\n");
            printf("Sadly none of the cells here seem to hold secrets.\n");
            printf("\nPress ENTER to continue.");
            getchar();
        }
    } else if (choice == 3) {
        state->visited_cells = true;
        state->location = ROOM_HALLWAY2;
    }
}

void handle_hallway4(gameState_t *state) {
    print_header("THE HALLWAY CONTINUES");
    printf("As you head West, You get to another room full of cells.\n");

    printf("\n1. Explore the room to find an exit.\n");
    printf("2. Inspect the Cells.\n");
    printf("3. Return to the previous hallway.\n");

    int choice = get_input(3);
    if (choice == 1) {
        printf("\nYou explore the cell for an exit\n");
        if (state->visited_cells) {
            printf("Just like the other cell, this is also a dead end\n");
            printf("\nPress ENTER to continue.");
            getchar();
        } else {
            printf("The room appears to be a dead-end.\n");
            printf("\nPress ENTER to continue.");
            getchar();
        }
    } else if (choice == 2) {
        if (state->visited_cells) {
            printf("\nYou approach quietly like the other cell.\n");
            printf("More starving prisoners and a few empty cells.\n");
            printf("When searching the empty cells in this room, you find a key hidden behind a loose stone\n");
            state->has_exit_key = true;
            printf("\nPress ENTER to continue.");
            getchar();
        } else {
            printf("\nYou approach the cells with breathing carefully.\n");
            printf("You peek inside and see starving prisoners.\n");
            printf("A few cells are empty.\nYou think back to your cell and search for a loose stone.\n");
            printf("Sadly none of the cells here seem to hold secrets.\n");
            printf("\nPress ENTER to continue.");
            getchar();
        }
    } else if (choice == 3) {
        state->visited_cells = true;
        state->location = ROOM_HALLWAY2;
    }
}

void handle_hallway_end(gameState_t *state) {
    print_header("THE HALLWAY CONTINUES?");
    printf("As you continue down this hallway, you turn the corner and see a door!\n");
    if (state->guard_alive) {
        printf("\nAs you head towards the door, a guard appears!\n");
        printf("\n1. Continue forward\n");
        printf("2. Return back to the hallway\n");
        int choice2 = get_input(2);
        if (choice2 == 1) {
            if (state->has_sword) {
                printf("\nYou sneak up on the guard and attack with your sword!\n");
                state->guard_alive = false;
                printf("The Orc Guard Lies Defeated!\n");
                printf("\nPress ENTER to escape!");
                getchar();
                state->location = ROOM_EXIT;
            } else {
                printf("\nYou sneak up on the guard and try to punch him.\nHe simply laughs at you and strikes back\nMaybe a weapon is hidden somewhere...\n");
                state->health -= 50;
                printf("Current Health: %d\n", state->health);
                printf("\nPress ENTER to retreat.");
                getchar();
                state->location = ROOM_HALLWAY2;
            }
        } else if (choice2 == 2) {
            state->location = ROOM_HALLWAY2;
        }
    } else {
        if (state->has_exit_key) {
            printf("\nYou step over the dead guard and escape!\n");
            printf("\nPress ENTER to continue.");
            getchar();
            state->location = ROOM_EXIT;
        } else {
            printf("\nThe guard is dead but the door is locked.\nPerhaps a key can be found somewhere.\n");
            printf("\nPress ENTER to return to the hallway.");
            getchar();
            state->location = ROOM_HALLWAY2;
        }
    }
}