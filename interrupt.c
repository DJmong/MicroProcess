#include <avr/io.h>

void interrupt_init(){
	EIMSK|= 1<<INT0;
	EIMSK|= 1<<INT1;
	EIMSK|= 1<<INT2;
	EIMSK|= 1<<INT4;
	EICRA |= (1<<ISC01)|(0<<ISC00);
	EICRA |= (1<<ISC11)|(0<<ISC10);
	EICRA |= (1<<ISC21)|(0<<ISC20);
	EICRB |= (1<<ISC41)|(0<<ISC40);
}
