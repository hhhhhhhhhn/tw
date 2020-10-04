#include <stdlib.h>
#include <ncurses.h>
#include <stdio.h>

#define PATH "/tmp/tw"
#define D_SIZE 8

int main(int argc, char** argv) {
	int temp_len;
	if(argc == 1 || (temp_len = atoi(argv[1])) < 1) {
		temp_len = 10;
	}

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	char* output = malloc(sizeof(char) * D_SIZE);
	if(output == NULL) exit(1);
	int output_used = 0;
	int output_len = D_SIZE;

	char* temp = malloc(sizeof(char) * temp_len);
	if(temp == NULL) exit(1);
	int temp_used = 0;

	int chr;

	while((chr = getch()) != 27) { // Esc
		switch(chr) {
			case '\n':
				while(output_len < output_used + temp_used + 1) {
					output_len *= 2;
					output = realloc(output, output_len * sizeof(char));
				}
				for(int i = 0; i < temp_used; i++) {
					output[output_used] = temp[i];
					output_used++;
				}
				output[output_used] = '\n';
				output_used++;
				temp_used = 0;
				break;
			case KEY_BACKSPACE:
				temp_used--;
				break;
			case KEY_UP:
			case KEY_DOWN:
			case KEY_LEFT:
			case KEY_RIGHT:
				break;
			default:
				if(temp_used == temp_len) {
					if(output_len + 1 < output_used) {
						output_len *= 2;
						output = realloc(output, output_len * sizeof(char));
					}
					output[output_used] = temp[0];
					output_used++;
					for(int i = 0; i < temp_used-1; i++) temp[i] = temp[i + 1];
					temp[temp_len - 1] = chr;
				}
				else {
					temp[temp_used] = chr;
					temp_used++;
				}
				break;
		}
		move(0, 0);
		clrtoeol();
		for(int i = 0; i < temp_used; i++) addch(temp[i]);
		refresh();
	}
	while(output_len < output_used + temp_used + 1) {
		output_len *= 2;
		output = realloc(output, output_len * sizeof(char));
	}
	for(int i = 0; i < temp_used; i++) {
		output[output_used] = temp[i];
		output_used++;
	}
	endwin();
	free(temp);
	if(output_len == output_used) {
		output_len++;
		output = realloc(output, output_len * sizeof(char));
	}
	output[output_used] = '\0';
	FILE* file_ptr = fopen(PATH, "w");
	fputs(output, file_ptr);
	fclose(file_ptr);
}
