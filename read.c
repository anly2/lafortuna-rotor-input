#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>
#include "os.h"

#define INPUT_BUFFER_LEN 100


int collect_delta(int);
int check_switches(int);


bool read_initialized = false;

void read_init(void) {
	if (read_initialized)
		return;
		
    os_init();
	
    os_add_task( collect_delta,   10, 1);
    os_add_task( check_switches,  10, 1);
	
    sei();
	
	read_initialized = true;
}


bool volatile input_enabled = false;
bool volatile input_finalized = false;
int input_index = 0;
char input_text[INPUT_BUFFER_LEN];

uint16_t draw_x = 0;
uint16_t draw_y = 0;
uint16_t prev_x, prev_y;

char* to_str(char c) {
	static char s[2];
	s[0] = c;
	s[1] = '\0';
	return s;
}

void draw_char(char c) {
	input_text[input_index] = c;
	display_string_xy(to_str(c), draw_x, draw_y);
}


char* alphabet = "abcdefghijklmnopqrstuvwxyz0123456789";
int alphabet_length = 36;

int cycle_chars_index = 0;

void cycle_chars_next() {
	int i = (cycle_chars_index + 1) % alphabet_length;
	draw_char(alphabet[i]);
	cycle_chars_index = i;
}

void cycle_chars_prev() {
	int i = (cycle_chars_index - 1);
	if (i < 0)
		i = alphabet_length - 1;
		
	draw_char(alphabet[i]);
	cycle_chars_index = i;
}

void advance() {
	/* add a check if INPUT_BUFFER_LEN is exceeded */

	display_string_xy(to_str(input_text[input_index]), draw_x, draw_y);
	draw_x = display.x;
	draw_y = display.y;
	input_index++;
	char c = alphabet[cycle_chars_index];
	draw_char(c);
}



char* read_string(char* input, int len) {
	if (!read_initialized)
		read_init();
		
	input_enabled = true;
	
	prev_x = display.x;
	prev_y = display.y;
	draw_char(alphabet[cycle_chars_index]);
	
	while (!input_finalized) {
		_delay_ms(5);
	}
	
	int i;
	int l = (len < input_index)? len : input_index;
	for (i=0; i<l-1; i++)
		input[i] = input_text[i];
	input[i] = '\0';
	
	/*input_text = "";*/
	input_index = 0;
	cycle_chars_index = 0;
	input_finalized = false;
	
	return input;
}

void complete() {
	if (!input_enabled)
		return;
	
	cli();
	input_enabled = false;
	
	display.x = prev_x;
	display.y = prev_y;
	int i;
	for (i=0; i<=input_index; i++)
		display_char(' ');
	
	display.x = prev_x;
	display.y = prev_y;
	
	input_text[++input_index] = '\0';
	input_finalized = true;
	sei();
}



int collect_delta(int state) {
	int delta = os_enc_delta();
	
	/* add easing */
	
	if (delta > 0)
		cycle_chars_next();
	else
	if (delta < 0)
		cycle_chars_prev();
	
	return state;
}


int check_switches(int state) {
	if (get_switch_press(_BV(SWE))) {
		advance();
	}
		
	if (get_switch_press(_BV(SWS))) {
		draw_char(' ');
		advance();
	}
	
	if (get_switch_short(_BV(SWC))) {
		complete();
	}
	
	/* add "back" / "left" */
	
	return state;	
}

