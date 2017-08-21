#include "Chip8State.h"
#include "decoder.h"
#include "string.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define OVERCLOCK_FACTOR 0.12
#define NANOS_BETWEEN_CYCLES (16666666 * OVERCLOCK_FACTOR)
#define KEYBOARD_TIMEOUT_CYCLES 50

void get_keyboard_input(chip8_state_t *state) {

  char input = getch();

  switch (input) {
  case '1':
    state->keyboard_time_left[1] = KEYBOARD_TIMEOUT_CYCLES;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 1;
    }
    break;
  case '2':
    state->keyboard_time_left[2] = KEYBOARD_TIMEOUT_CYCLES;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 2;
    }
    break;
  case '3':
    state->keyboard_time_left[3] = KEYBOARD_TIMEOUT_CYCLES;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 3;
    }
    break;
  case '4':
    state->keyboard_time_left[0xC] = KEYBOARD_TIMEOUT_CYCLES;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 0xC;
    }
    break;
  case 'q':
    state->keyboard_time_left[4] = KEYBOARD_TIMEOUT_CYCLES;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 4;
    }
    break;
  case 'w':
    state->keyboard_time_left[5] = KEYBOARD_TIMEOUT_CYCLES;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 5;
    }
    break;
  case 'e':
    state->keyboard_time_left[6] = KEYBOARD_TIMEOUT_CYCLES;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 6;
    }
    break;
  case 'r':
    state->keyboard_time_left[0xD] = KEYBOARD_TIMEOUT_CYCLES;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 0xD;
    }
    break;
  case 'a':
    state->keyboard_time_left[7] = KEYBOARD_TIMEOUT_CYCLES;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 7;
    }
    break;
  case 's':
    state->keyboard_time_left[8] = KEYBOARD_TIMEOUT_CYCLES;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 8;
    }
    break;
  case 'd':
    state->keyboard_time_left[9] = KEYBOARD_TIMEOUT_CYCLES;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 9;
    }
    break;
  case 'f':
    state->keyboard_time_left[0xE] = KEYBOARD_TIMEOUT_CYCLES;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 0xE;
    }
    break;
  case 'z':
    state->keyboard_time_left[0xA] = KEYBOARD_TIMEOUT_CYCLES;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 0xA;
    }
    break;
  case 'x':
    state->keyboard_time_left[0] = KEYBOARD_TIMEOUT_CYCLES;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 0;
    }
    break;
  case 'c':
    state->keyboard_time_left[0xB] = KEYBOARD_TIMEOUT_CYCLES;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 0xB;
    }
    break;
  case 'v':
    state->keyboard_time_left[0xF] = KEYBOARD_TIMEOUT_CYCLES;
    if (state->waiting_for_key_press) {
      state->v[state->register_to_save_key] = 0xF;
    }
    break;
  default:
    break;
  }
  if (state->waiting_for_key_press && input != ERR) {
    state->pc = state->pc + 2;
    state->waiting_for_key_press = false;
  }

  for (size_t i = 0; i < 16; i++) {
    state->keyboard[i] = state->keyboard_time_left[i] > 0;
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
    chip8_decrement_keyboard_timeout(&state);

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