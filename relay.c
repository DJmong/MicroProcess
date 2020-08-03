#include <avr/io.h>

#define relay_set DDRB
#define relay_in PORTB
#define off relay_in&=0xFE;
#define on relay_in|=0x01;

void relay_init(){
	relay_set|=0x01;
}

void relay_on(){
	on;
}
void relay_off(){
	off;
}


