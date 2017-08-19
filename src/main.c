#include <ncurses.h>
#include <src/Chip8State.h>

int main() {
	chip8_state_t state;
	init_chip8_state(&state);
	printf("%d", sizeof(state));
	free_chip8_state(&state);
	return 0;
}