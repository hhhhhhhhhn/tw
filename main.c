#include <stdlib.h>
#include <ncurses.h>
#include <stdio.h>
#include <locale.h>
#include "cmdline.h"

#define D_SIZE 8

int main(int argc, char** argv) {
	struct gengetopt_args_info args;
	cmdline_parser(argc, argv, &args);
	int temp_len = args.display_arg;
	char* path = args.output_arg;

	FILE* file_ptr = fopen(path, "w");
	if (file_ptr == NULL) {
		fprintf(stderr, "could not open file \"%s\"\n", path);
		return 1;
	}

	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	char* output = malloc(sizeof(char) * D_SIZE);
	if(output == NULL) exit(1);
	int output_used = 0;
	int output_len = D_SIZE;

	char* temp = malloc(sizeof(char) * temp_len + sizeof(char)); // for \0
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
				if(temp_used != 0) temp_used--;
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
		temp[temp_used] = '\0';
		printw(temp);
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
	fputs(output, file_ptr);
	fclose(file_ptr);
}
