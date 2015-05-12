
#include "os.h"
#include "read.h"

void main(void) {
	/* read_string will initialize itself */
	
    for(;;){
		char inp[100];
		read_string(inp, 100);
		
		cli();
		uint16_t prev_col = display.foreground;
		display.foreground = GRAY;
		display_string(inp);
		display.foreground = prev_col;
		sei();
	}
}