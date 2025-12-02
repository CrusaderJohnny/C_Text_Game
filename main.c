#include <stdio.h>
#include "game_functions/rooms.h"

int main(void) {
    gameState_t state = {
        .location = ROOM_CELL,
        .health = 100,
        .has_key = false,
        .has_sword = false,
        .guard_alive = true
    };

    printf("\nWelcome to ESCAPE THE TEXT DUNGEON.\n");
    printf("-----------------------------------------\n");
    bool running = true;
    while (running) {
        if (state.health <= 0) {
            state.location = GAME_OVER;
        }
        switch (state.location) {
            case ROOM_CELL:
                handle_cell(&state);
                break;
            case ROOM_HALLWAY:
                handle_hallway(&state);
                break;
            case ROOM_ARMORY:
                handle_armory(&state);
                break;
            case ROOM_EXIT:
                state.location = GAME_WIN;
                break;
            case GAME_OVER:
                printf("\n--- GAME OVER ---\nYour journey ends here.\n");
                running = false;
                break;
            case GAME_WIN:
                printf("\n--- VICTORY! ---\nYou have escaped text hell!\n");
                running = false;
                break;
        }
    }
    return 0;
}
