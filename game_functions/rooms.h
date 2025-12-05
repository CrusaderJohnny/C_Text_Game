#ifndef ROOMS_H
#define ROOMS_H
#include <stdbool.h>


typedef enum  {
    ROOM_CELL,
    ROOM_HALLWAY,
    ROOM_HALLWAY2,
    ROOM_HALLWAY3,
    ROOM_HALLWAY4,
    ROOM_HALLWAY_END,
    ROOM_ARMORY,
    ROOM_EXIT,
    GAME_OVER,
    GAME_WIN
} location_t;

typedef struct {
    location_t location;
    int health;
    bool has_key;
    bool has_exit_key;
    bool has_sword;
    bool visited_cells;
    bool guard_alive;
} gameState_t;


int get_input(int max_choice);
void print_header(char *title);
void print_image(char *file);
void handle_cell(gameState_t *state);
void handle_hallway(gameState_t *state);
void handle_armory(gameState_t *state);
void handle_hallway2(gameState_t *state);
void handle_hallway3(gameState_t *state);
void handle_hallway4(gameState_t *state);
void handle_hallway_end(gameState_t *state);


#endif