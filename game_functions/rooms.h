#ifndef ROOMS_H
#define ROOMS_H
#include <stdbool.h>


typedef enum  {
    ROOM_CELL,
    ROOM_HALLWAY,
    ROOM_ARMORY,
    ROOM_EXIT,
    GAME_OVER,
    GAME_WIN
} location_t;

typedef struct {
    location_t location;
    int health;
    bool has_key;
    bool has_sword;
    bool guard_alive;
} gameState_t;


int get_input(int max_choice);
void print_header(char *title);
void print_image(char *file);
void handle_cell(gameState_t *state);
void handle_hallway(gameState_t *state);
void handle_armory(gameState_t *state);


#endif