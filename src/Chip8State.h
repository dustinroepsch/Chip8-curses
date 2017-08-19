#ifndef CHIP8_CHIP8STATE_H
#define CHIP8_CHIP8STATE_H

#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t* memory;
    uint8_t* v;
    uint16_t* stack;
    bool* screen;    
    uint8_t sound_timer;
    uint8_t delay_timer;
    uint16_t pc;
    uint8_t sp;
} chip8_state_t;

void init_chip8_state(chip8_state_t* state);

void free_chip8_state(chip8_state_t* state);

#endif /* CHIP8_CHIP8STATE_H */ 