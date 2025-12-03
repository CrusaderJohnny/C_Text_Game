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
    printf("\nYou are in a damp, dark cell. The door is locked.\n");
    printf("There is a loose stone in the wall.\n");

    printf("1. Inspect the loose stone\n");
    printf("2. Examine the door\n");
    printf("3. Sleep on the floor (Rest)\n");

    int choice = get_input(3);

    if (choice == 1) {
        printf("\nYou wiggle the stone out of its spot in the wall...");
        if (state->has_key) {
            printf("\nThe hole behind the stone is empty.\n");
            printf("Press ENTER to continue.");
            getchar();
        } else {
            printf("\nAnd find a rusty key!\n");
            state->has_key = true;
            printf("Press ENTER to continue.");
            getchar();
        }
    } else if (choice == 2) {
        printf("\nYou approach the door...\n");
        if (state->has_key) {
            printf("The key fits in the keyhole, you turn the key and exit the cell\n");
            state->location = ROOM_HALLWAY;
            printf("Press ENTER to continue.");
            getchar();
        } else {
            printf("The door is locked.\n");
            if (!state->has_key) {
                printf("If only this room had a secret.\n");
                printf("Press ENTER to continue.");
                getchar();
            }
        }
    } else if (choice == 3) {
        printf("\nYou fall asleep. As you rest up, a rat bites your toe.\n");
        printf("Health -10\n");
        state->health -= 10;
        printf("Current health: %d\n", state->health);
        printf("Press ENTER to continue.");
        getchar();
    }
}

void handle_hallway(gameState_t *state) {
    print_header("THE HALLWAY");
    print_image("../images/hallway.txt");
    printf("\nTorches line the walls.\n");
    printf("To the north is a door, to the south is your cell\n");
    printf("To the east, a door with a chain on it. It appears to be locked.\n");

    printf("1. Head North\n");
    printf("2. Head East\n");
    printf("3. Return to your cell (South)\n");

    int choice = get_input(3);
    if (choice == 1) {
        if (state->guard_alive) {
            printf("\nAs you head towards the door, a guard appears!\n");
            printf("1. Continue forward\n");
            printf("2. Return back to the hallway\n");
            int choice2 = get_input(2);
            if (choice2 == 1) {
                if (state->has_sword) {
                    printf("\nYou sneak up on the guard and attack with your sword!\n");
                    state->guard_alive = false;
                    printf("The Orc Guard Lies Defeated!\n");
                    printf("Press ENTER to continue.");
                    getchar();
                    state->location = GAME_WIN;
                } else {
                    printf("\nYou sneak up on the guard and try to punch him.\nHe simply laughs at you and strikes back\n");
                    state->health -= 50;
                    printf("Current Health: %d\n", state->health);
                    printf("Press ENTER to continue.");
                    getchar();
                }
            }
        } else {
            printf("\nYou step over the dead guard and escape!\n");
            state->location = ROOM_EXIT;
            printf("Press ENTER to continue.");
            getchar();
        }
    } else if (choice == 2) {
        state->location = ROOM_ARMORY;
    } else if (choice == 3) {
        state->location = ROOM_CELL;
    }
}

void handle_armory(gameState_t *state) {
    print_header("THE ARMORY");
    printf("Racks of weapons line the walls, but most are broken.\n");

    printf("1. Search for a weapon\n");
    printf("2. Return to the hallway\n");

    int choice = get_input(2);

    if (choice == 1) {
        if (state->has_sword) {
            printf("You already found the best weapon here.\n");
            printf("Press ENTER to continue.");
            getchar();
        } else {
            printf("\nYou find a sharp Short Sword hidden under a pile of shields!\n");
            print_image("../images/sword.txt");
            state->has_sword = true;
            printf("\nPress ENTER to continue.");
            getchar();
        }
    } else if (choice == 2) {
        state->location = ROOM_HALLWAY;
    }
}