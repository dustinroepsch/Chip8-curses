#include "Chip8State.h"
#include <stdlib.h>

void init_chip8_state(chip8_state_t* state) {
    state->memory = calloc(4096, sizeof(uint8_t));
    state->v = calloc(0xF, sizeof(uint8_t));
    state->stack = calloc(16, sizeof(uint16_t));    
    state->screen = calloc(CHIP8_SCREEN_HEIGHT * CHIP8_SCREEN_WIDTH, sizeof(bool));
    state->sound_timer = 0;
    state->delay_timer = 0;
    state->pc = 0x200;
    state->sp = 0;
}

void free_chip8_state(chip8_state_t* state) {
    free(state->memory);
    free(state->v);
    free(state->stack);
    free(state->screen);
}