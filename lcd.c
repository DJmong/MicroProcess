#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

// define LCD address
#define	LCD_PORT		PORTC
#define 	LCD_DDR		DDRC

// internal function
static void checkbusy(void);
static void write_command(char command);
static void write_data(char ch);



// LcdInit( ) : LCD Display 초기화


void LcdInit(void){
	LCD_DDR = 0xFF;
	_delay_ms(15);
	write_command(0x38);
	_delay_ms(5);
	write_command(0x38);
	_delay_ms(1);
	write_command(0x38);

	LcdCommand(FUNSET);
	LcdCommand(DISP_OFF);
	LcdCommand(ALLCLR);
	LcdCommand(ENTMOD);
	LcdCommand(DISP_ON);
}





// Put a command to the LCD
void LcdCommand(char command){
	checkbusy();
	write_command(command);
	if (command == ALLCLR || command == HOME)
		_delay_ms(2);			// 명령실행시간 1.52msec
}

// Put a character to the LCD
void LcdPutchar(char ch){
	checkbusy();
	write_data(ch);
}

// Put a string to the LCD
void LcdPuts (char* str){
	while(*str){
		LcdPutchar(*str);
		str++;
	}
}




// Move cursor
void LcdMove(char line, char pos){
	pos = (line << 6) + pos;		// DDRAM 주소 계산 
	pos |= 0x80;			// DDRAM 주소 설정 명령 (LCD 명령표 참조)
	LcdCommand(pos);
}
// write command interface
static void write_command(char command){
	char temp;

	// output upper nibble
	temp = (command & 0xF0) | 0x04; 	// RS=0, RW=0, E=1
	LCD_PORT = temp;
	LCD_PORT = temp & ~0x04;		// E=0

	// output lower nibble
	temp = (command << 4) | 0x04; 	// RS=0, RW=0, E=1
	LCD_PORT = temp;
	LCD_PORT = temp & ~0x04;		// E=0
}




// write data interface
static void write_data(char ch){
	unsigned char temp;

	// output upper nibble
	temp = (ch & 0xF0) | 0x05; 		// RS=1, RW=0, E=1
	LCD_PORT = temp;
	LCD_PORT = temp & ~0x04;		// E=0

	// output lower nibble
	temp = (ch << 4) | 0x05; 		// RS=1, RW=0, E=1
	LCD_PORT = temp;
	LCD_PORT = temp & ~0x04;		// E=0
}

// Use delay instead of BF check
static void checkbusy(){
	_delay_ms(1);
}

