#include <src/Chip8State.h>

void init_chip8_state(chip8_state_t* state) {
    state->memory = calloc(sizeof(uint8_t), 4096);
    state->v = calloc(sizeof(uint8_t), 0xF);
    state->stack = calloc(sizeof(uint16_t), 16);    
    state->screen = calloc(sizeof(bool), CHIP8_SCREEN_HEIGHT * CHIP8_SCREEN_WIDTH);
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