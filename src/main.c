#include "Chip8State.h"
#include "decoder.h"
#include "string.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NANOS_BETWEEN_CYCLES 16666666

void get_keyboard_input(chip8_state_t *state) {

  
  char input = getch();
  if (input == ERR) {
    return;
  }
  // update the keyboard
  memset(state->keyboard, false, sizeof(bool) * 16);
  switch (input) {
  case '1':
    state->keyboard[1] = true;
    break;
  case '2':
    state->keyboard[2] = true;
    break;
  case '3':
    state->keyboard[3] = true;
    break;
  case '4':
    state->keyboard[0xC] = true;
    break;
  case 'q':
    state->keyboard[4] = true;
    break;
  case 'w':
    state->keyboard[5] = true;
    break;
  case 'e':
    state->keyboard[6] = true;
    break;
  case 'r':
    state->keyboard[0xD] = true;
    break;
  case 'a':
    state->keyboard[7] = true;
    break;
  case 's':
    state->keyboard[8] = true;
    break;
  case 'd':
    state->keyboard[9] = true;
    break;
  case 'f':
    state->keyboard[0xE] = true;
    break;
  case 'z':
    state->keyboard[0xA] = true;
    break;
  case 'x':
    state->keyboard[0] = true;
    break;
  case 'c':
    state->keyboard[0xB] = true;
    break;
  case 'v':
    state->keyboard[0xF] = true;
    break;
  default:
    break;
  }
}

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
    cbreak();
    nodelay(stdscr, true);
  }

  while (true) {
    uint16_t opcode = decoder_get_current_opcode(&state);
    instruction_t instruction = decoder_opcode_to_instruction(opcode);
    decoder_execute_instruction(&state, instruction);

    if (!DEBUG) {
      // draw the screen
      for (size_t row = 0; row < CHIP8_SCREEN_HEIGHT; row++) {
        for (size_t col = 0; col < CHIP8_SCREEN_WIDTH; col++) {
          size_t screen_index = row * CHIP8_SCREEN_WIDTH + col;
          mvaddch(row, col, state.screen[screen_index] ? '#' : ' ');
        }
      }

      get_keyboard_input(&state);
      refresh();
    }

    // slow loop down to 60HZ
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    while (current_time.tv_nsec - time_since_last_cycle.tv_nsec <
           NANOS_BETWEEN_CYCLES) {
      if (!DEBUG) {
        get_keyboard_input(&state);
      }
      clock_gettime(CLOCK_MONOTONIC, &current_time);
      if (current_time.tv_nsec - time_since_last_cycle.tv_nsec < 0) {
        break;
      }
    }
    clock_gettime(CLOCK_MONOTONIC, &time_since_last_cycle);
  }
  return 0;
}