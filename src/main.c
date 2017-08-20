#include "Chip8State.h"
#include "decoder.h"
#include "string.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define OVERCLOCK_FACTOR 0.12
#define NANOS_BETWEEN_CYCLES (16666666 * OVERCLOCK_FACTOR)

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
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 1;
    }
    break;
  case '2':
    state->keyboard[2] = true;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 2;
    }
    break;
  case '3':
    state->keyboard[3] = true;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 3;
    }
    break;
  case '4':
    state->keyboard[0xC] = true;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 0xC;
    }
    break;
  case 'q':
    state->keyboard[4] = true;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 4;
    }
    break;
  case 'w':
    state->keyboard[5] = true;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 5;
    }
    break;
  case 'e':
    state->keyboard[6] = true;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 6;
    }
    break;
  case 'r':
    state->keyboard[0xD] = true;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 0xD;
    }
    break;
  case 'a':
    state->keyboard[7] = true;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 7;
    }
    break;
  case 's':
    state->keyboard[8] = true;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 8;
    }
    break;
  case 'd':
    state->keyboard[9] = true;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 9;
    }
    break;
  case 'f':
    state->keyboard[0xE] = true;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 0xE;
    }
    break;
  case 'z':
    state->keyboard[0xA] = true;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 0xA;
    }
    break;
  case 'x':
    state->keyboard[0] = true;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 0;
    }
    break;
  case 'c':
    state->keyboard[0xB] = true;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 0xB;
    }
    break;
  case 'v':
    state->keyboard[0xF] = true;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 0xF;
    }
    break;
  default:
    break;
  }
  if (state->waiting_for_key_press) {
    state->pc = state->pc + 2;
  }
  state->waiting_for_key_press = false;
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

      if (state.sound_timer > 0) {
        beep();
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