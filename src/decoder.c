#include "decoder.h"
#include "string.h"
#include <stdio.h>

uint16_t decoder_get_current_opcode(chip8_state_t *state) {
  uint16_t current_pc = state->pc;
  uint16_t msb = state->memory[current_pc];
  uint8_t lsb = state->memory[current_pc + 1];
  uint16_t opcode = (msb << 8) | lsb;
  return opcode;
}

instruction_t decoder_opcode_to_instruction(uint16_t opcode) {
  instruction_t instruction;
  instruction.full_opcode = opcode;
  instruction.I = opcode >> (4 * 3);
  instruction.X = (opcode & 0x0F00) >> (4 * 2);
  instruction.Y = (opcode & 0x00F0) >> 4;
  instruction.NNN = opcode & 0x0FFF;
  instruction.KK = opcode & 0x00FF;
  instruction.J = opcode * 0x000F;

  return instruction;
}

void decoder_execute_instruction(chip8_state_t *state,
                                 instruction_t instruction) {
  switch (instruction.I) {
  case 0x0:
    switch (instruction.full_opcode) {
    case 0x00E0:
      printf("clearscr\n");
      memset(state->screen, false,
             sizeof(bool) * CHIP8_SCREEN_HEIGHT * CHIP8_SCREEN_WIDTH);
      state->pc = state->pc + 2;
      break;
    case 0x00EE:
      printf("return\n");
      state->pc = state->stack[state->sp];
      state->sp = state->sp - 1;
      break;
    default:
      goto UNIMPL;
    }
  case 0x1:
    printf("Jump to %x\n", instruction.NNN);
    state->pc = instruction.NNN;
    break;
  case 0x2:
    printf("Call %x\n", instruction.NNN);
    state->sp = state->sp + 1;
    state->stack[state->sp] = state->pc;
    state->pc = instruction.NNN;
    break;
  case 0x3:
    printf("skip if v[%x] == %x\n", instruction.X, instruction.KK);
    if (state->v[instruction.X] == instruction.KK) {
      state->pc = state->pc + 2;
    }
    state->pc = state->pc + 2;
    break;
  case 0x4:
    printf("skip if v[%x] != %x\n", instruction.X, instruction.KK);
    if (state->v[instruction.X] != instruction.KK) {
      state->pc = state->pc + 2;
    }
    state->pc = state->pc + 2;
    break;
  case 0x5:
    if (instruction.J != 0x0)
      goto UNIMPL;
    printf("skip if v[%x] == v[%x]\n", instruction.X, instruction.Y);
    if (state->v[instruction.X] == instruction.Y) {
      state->pc = state->pc + 2;
    }
    state->pc = state->pc + 2;
    break;
  case 0x6:
    printf("set v[%x] = %x", instruction.X, instruction.KK);
    state->v[instruction.X] = instruction.KK;
    state->pc = state->pc + 2;
    break;
  case 0x7:
    printf("set v[%x] = v[%x] + %x", instruction.X, instruction.X,
           instruction.KK);
    state->v[instruction.X] = state->v[instruction.X] + instruction.KK;
    state->pc = state->pc + 2;
    break;
  case 0x8:
    switch (instruction.J) {
    case 0x0:
      printf("v[%x] = v[%x]", instruction.X, instruction.Y);
      state->v[instruction.X] = state->v[instruction.Y];
      state->pc = state->pc + 2;
      break;
    case 0x1:
      printf("v[%x] = v[%x] or v[%x]", instruction.X, instruction.X,
             instruction.Y);
      state->v[instruction.X] =
          state->v[instruction.X] | state->v[instruction.Y];
      state->pc = state->pc + 2;
      break;
    case 0x2:
      printf("v[%x] = v[%x] and v[%x]", instruction.X, instruction.X,
             instruction.Y);
      state->v[instruction.X] =
          state->v[instruction.X] & state->v[instruction.Y];
      state->pc = state->pc + 2;
      break;
    case 0x3:
      printf("v[%x] = v[%x] xor v[%x]", instruction.X, instruction.X,
             instruction.Y);
      state->v[instruction.X] =
          state->v[instruction.X] ^ state->v[instruction.Y];
      state->pc = state->pc + 2;
      break;
    case 0x4:
      printf("Set V[%x] = V[%x] + V[%x], set VF = carry", instruction.X,
             instruction.X, instruction.Y);
      uint32_t x = state->v[instruction.X];
      uint32_t y = state->v[instruction.Y];
      if (x + y > 255) {
        state->v[0xF] = 0x1;
      } else {
        state->v[0xF] = 0x0;
      }
      state->v[instruction.X] =
          state->v[instruction.X] + state->v[instruction.Y];
      state->pc = state->pc + 2;
      break;
    case 0x5:
      printf("Set V[%x] = V[%x] - V[%x], set VF = NOT borrow", instruction.X,
             instruction.X, instruction.Y);
      if (state->v[instruction.X] > state->v[instruction.Y]) {
        state->v[0xF] = 0x1;
      } else {
        state->v[0xF] = 0x0;
      }
      state->v[instruction.X] =
          state->v[instruction.X] - state->v[instruction.Y];
      state->pc = state->pc + 2;
      break;
    case 0x6:
      printf("Set V[%x] = V[%x] SHR 1", instruction.X, instruction.X);
      if ((state->v[instruction.X] & 0b00000001) == 0x1) {
        state->v[0xF] = 1;
      } else {
        state->v[0xF] = 0;
      }
      state->v[instruction.X] = state->v[instruction.X] >> 1;
      state->pc = state->pc + 2;
    default:
      goto UNIMPL;
    }
  UNIMPL:
  default:
    printf("Unknown opcode %x\n", instruction.full_opcode);
  }
}