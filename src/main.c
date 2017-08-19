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

	opcode_t test_opcode = {.bits = 0xABCD};
	printf(
		"I = %x, X = %x, Y = %x, J = %x, NNN = %x, KK = %x \n",
		test_opcode.I,
		test_opcode.X,
		test_opcode.Y,
		test_opcode.J,
		test_opcode.NNN,
		test_opcode.KK);

	chip8_state_free(&state);
	return 0;
}