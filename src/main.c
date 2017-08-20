#include <ncurses.h>
#include <stdio.h>
#include "Chip8State.h"
#include "decoder.h"

int main(int argc, char **argv)
{
	//open the cartridge file
	FILE *cart;
	if (argc >= 2)
	{
		cart = fopen(argv[1], "rb");
	}
	else
	{
		printf("usage Chip8 PATH_TO_GAME\n");
		return -1;
	}

	//create and initialize the chip8 vm state and close the file
	chip8_state_t state;
	chip8_state_init(&state);
	chip8_load_cartridge(&state, cart);
	fclose(cart);

	uint8_t current_opcode = decoder_get_current_opcode(&state);
	while (current_opcode != 0) {
		decoder_execute_instraction(decoder_opcode_to_instruction(current_opcode));
		current_opcode = decoder_get_current_opcode(&state);
		state.pc = state.pc + 2;
	}

	return 0;
}