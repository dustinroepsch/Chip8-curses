#include "Chip8State.h"
#include <stdlib.h>
#include <inttypes.h>

void chip8_state_init(chip8_state_t *state)
{
    state->memory = calloc(4096, sizeof(uint8_t));
    state->v = calloc(0xF, sizeof(uint8_t));
    state->stack = calloc(16, sizeof(uint16_t));
    state->screen = calloc(CHIP8_SCREEN_HEIGHT * CHIP8_SCREEN_WIDTH, sizeof(bool));
    state->sound_timer = 0;
    state->delay_timer = 0;
    state->pc = 0x200;
    state->sp = 0;
}

void chip8_state_free(chip8_state_t *state)
{
    free(state->memory);
    free(state->v);
    free(state->stack);
    free(state->screen);
}

void chip8_load_cartridge(chip8_state_t *state, FILE *cart)
{
    fseek(cart, 0, SEEK_END);
    size_t file_length = ftell(cart);
    rewind(cart);

    fread(state->memory + 0x200 - 1, file_length, 1, cart);
}