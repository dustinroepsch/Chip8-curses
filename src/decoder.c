#include "decoder.h"
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
  printf("%x \n", instruction.I);
}