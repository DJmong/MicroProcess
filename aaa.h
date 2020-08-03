#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "interrupt.h"
#include "calander.h"
#include "alram.h"
#include "speaker.h"
#include "relay.h"

#define speaker_set DDRB|=0x80;
#define speaker_in PORTB
#define speaker_off speaker_in&=0x7F;
#define speaker_on speaker_in|=0x80;


unsigned int av_in;
double temperature;
int timerbuffer[3]={0,0,0};
int timercount=0;
int adcounter=0;
int stopwatch_counter=0;
int stopwatch_buffer[3]={0,0,0};
int timer_sw=1;
int watch_mod=0;
int pre_mod=0;
int stopwatch_start=0;
char mod_str[6]="watch";

unsigned char key_in();
void timer_cal();
void timer_init();
void timer_print();
void adc_print();
void  _print();
void stopwatch_print();
