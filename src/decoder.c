#include "decoder.h"
#include <stdio.h>
#include "string.h"

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
  switch(instruction.I) {
      case 0x0:
        switch (instruction.full_opcode){
            case 0x00E0:
                printf("clearscr\n");
                memset(state->screen, false, CHIP8_SCREEN_HEIGHT* CHIP8_SCREEN_WIDTH);
                state->pc = state->pc + 2;
                break;
            case 0x00EE:
                printf("return\n");
                state->pc = state->stack[state->sp];
                state->sp = state->sp - 1;
                break;
            default:
                printf("Unknown opcode %x\n", instruction.full_opcode);
                break;
        }
        default:
            printf("Unknown opcode %x\n", instruction.full_opcode);
    }
}