#include "Chip8State.h"
#include "decoder.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NANOS_BETWEEN_CYCLES 16666666

int main(int argc, char **argv) {
  srand(time(NULL));
  // open the cartridge file
  FILE *cart;
  if (argc >= 2) {
    cart = fopen(argv[1], "rb");
  } else {
    printf("usage Chip8 PATH_TO_GAME\n");
    return -1;
  }

  // create and initialize the chip8 vm state and close the file
  chip8_state_t state;
  chip8_state_init(&state);
  chip8_load_cartridge(&state, cart);
  fclose(cart);

  struct timespec time_since_last_cycle;
  clock_gettime(CLOCK_MONOTONIC, &time_since_last_cycle);

  if (!DEBUG) {
    initscr();
    noecho();
  }

  while (true) {
    uint16_t opcode = decoder_get_current_opcode(&state);
    instruction_t instruction = decoder_opcode_to_instruction(opcode);
    decoder_execute_instruction(&state, instruction);

    // draw the screen
    if (!DEBUG) {
      for (size_t row = 0; row < CHIP8_SCREEN_HEIGHT; row++) {
        for (size_t col = 0; col < CHIP8_SCREEN_WIDTH; col++) {
          size_t screen_index = row * CHIP8_SCREEN_WIDTH + col;
          mvaddch(row, col, state.screen[screen_index] ? '#' : ' ');
        }
      }
      refresh();
    }

    // slow loop down to 60HZ
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    while (current_time.tv_nsec - time_since_last_cycle.tv_nsec <
           NANOS_BETWEEN_CYCLES) {
      clock_gettime(CLOCK_MONOTONIC, &current_time);
      if (current_time.tv_nsec - time_since_last_cycle.tv_nsec < 0) {
        break;
      }
    }
    clock_gettime(CLOCK_MONOTONIC, &time_since_last_cycle);
  }
  return 0;
}