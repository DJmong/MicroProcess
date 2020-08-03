#include "aaa.h"

ISR(ADC_vect){
	av_in = ADCW;
	temperature=((double)av_in)/1024.0*100.0;

}

ISR(TIMER0_OVF_vect){
	TCNT0=0;

	if(timer_sw==1) timercount++;
	if(stopwatch_start==1) stopwatch_counter++;
	adcounter++;

	if(stopwatch_counter==4){
		stopwatch_buffer[2]++;
		stopwatch_counter=0;
	}

	if(adcounter==2){
		ADCSR |= 0x40;
		adcounter=0;
	}

	if(timercount==4){
		timerbuffer[2]++;
		timercount=0;
	}
	
	timer_cal();	
}
ISR(INT0_vect){
	_delay_ms(50);
	switch(watch_mod){
		case 0:
			calander_year_up();
		break;
		case 1:
			timerbuffer[0]++;
			timer_cal();
			
		break;
		case 4:
			stopwatch_start^=0x01;
		break;
		case 5:
			alram_change();
		break;
		case 6:
			watch_mod=0;
			break;
	}
	EIFR |= 0 <<INTF0;
}

ISR(INT1_vect){
	_delay_ms(50);
	switch(watch_mod){
		case 0:
			calander_month_up();
		break;
		case 1:
			timerbuffer[1]++;
			timer_cal();
		break;
		case 4:
			if(!(stopwatch_start)){
				stopwatch_start=0;
				stopwatch_buffer[2]=0;
				stopwatch_buffer[1]=0;
				stopwatch_buffer[0]=0;
			}
		break;
		case 5:
			alram_countup();
		break;
		case 6:
			watch_mod=3;
	}
	EIFR |= 0 <<INTF1;
}
ISR(INT2_vect){
	_delay_ms(50);
	if(watch_mod==6) watch_mod=3;
	if(watch_mod==1){
		timer_sw^=0x01;
	}

	else if(watch_mod==0){
		calander_date_up();
	}

	else if(watch_mod==5){
		alram_setup();
	}
	EIFR |= 0 <<INTF2;
}

ISR(INT4_vect){
	_delay_ms(50);

	watch_mod++;
	if(watch_mod==6) watch_mod=3;
	if(watch_mod>5){
		watch_mod=0;
		alram_countclear();
	}
	EIFR |= 0 <<INTF4;
}


void timer_cal(){
	int i;
	for(i=2;i>=0;i--){
		if(i>0 &&timerbuffer[i]>59){
			timerbuffer[i]=0;
			timerbuffer[i-1]++;
		}
		else if(i==0 && timerbuffer[i]>23){
			timerbuffer[0]=0;
			calander_date_up();
		}
	}
	for(i=2;i>=0;i--){
		
		if(stopwatch_buffer[i]>59){
			stopwatch_buffer[i]=0;
			if(i>0) stopwatch_buffer[i-1]++;
		}
	}
}

void timer_init(){
	ASSR |= 1<<AS0;
	TCCR0 = (0<<WGM00)|(0<WGM01);
	TCCR0 |= (0<<CS02)|(1<<CS01)|(0<<CS00); //prescaler 1/64
	TCNT0 = 0x00;
	TIMSK = 1<<TOIE0;
}

void  _print(){
	char array[20];
	if(pre_mod!=watch_mod){
		LcdCommand(ALLCLR);
		pre_mod=watch_mod;
	}


	switch(watch_mod){
		
		case 0:
			sprintf(array, "SET DATE");
			LcdMove(0,0);
			LcdPuts(array);
			calander_print(array);
			LcdMove(1,0);
			LcdPuts(array);
		break;

		case 1:
			sprintf(array, "SET TIME");
			LcdMove(0,0);
			LcdPuts(array);
			timer_print();
		break;
		
		case 2:
			
			calander_print(array);
			LcdMove(0,0);
			LcdPuts(array);
			timer_print();
		break;

		case 3:
			sprintf(array, "TIME");
			LcdMove(0,0);
			LcdPuts(array);
			adc_print();
								
			timer_print();
		break;

		case 4:
			sprintf(array, "STOP");
			LcdMove(0,0);
			LcdPuts(array);
			adc_print();
			stopwatch_print();
		break;

		case 5:
			alram_set_date_print1(array);
			LcdMove(0,0);
			LcdPuts(array);
			alram_set_date_print2(array);
			LcdMove(1,0);
			LcdPuts(array);
		break;
		case 6:

			sprintf(array, "Time's Up!");
			LcdMove(0,0);
			LcdPuts(array);
			while(watch_mod==6) speaker();	
			break;
	
	}
}

void stopwatch_print(){
	int hour, minute, second;
	char array[15];
	hour=stopwatch_buffer[0];
	minute=stopwatch_buffer[1];
	second=stopwatch_buffer[2];

	sprintf(array, "   %d%d:%d%d:%d%d",hour/10, hour%10, minute/10, minute%10, second/10, second%10);
	LcdMove(1,0);
	LcdPuts(array);
}

void timer_print(){
	char ampm='A';
	int hour, minute, second;
	char array[15];
	hour=timerbuffer[0];
	if(hour>11){
		ampm='P';
		if(hour>12){
			hour-=12;
		}
	}
		if(timerbuffer[0]==0) {
			hour=12;
		}
	minute=timerbuffer[1];
	second=timerbuffer[2];

	LcdMove(1,0);
	sprintf(array, "%cM %d%d:%d%d:%d%d", ampm, hour/10, hour%10, minute/10, minute%10, second/10, second%10);
	LcdPuts(array);
}


void adc_print(){
	int itemp;
	char array[15];
	itemp=(int)(temperature * 10.0*5+0.5);
	sprintf(array, "%d.%d %cC", itemp/10,itemp%10,0xDF);
	LcdMove(0,10);
	LcdPuts(array);
}



int main(){
	int pre_alram=0;
	LcdInit();
	timer_init();
	interrupt_init();
	speaker_init();
	relay_init();

	DDRD=0xF0;
	DDRF=0xF7;
	ADMUX=0x03;
	ADCSR=0xCE;

	sei();
	while(1){
		if((alram_check(timerbuffer[0], timerbuffer[1], calander_get_dow()))&&(pre_alram==0)){
			watch_mod=6;
			pre_alram=1;
			relay_on();
		}

		else if((alram_check(timerbuffer[0], timerbuffer[1], calander_get_dow())==0)&&(pre_alram==1)){
			pre_alram=0;
			if(watch_mod==6) watch_mod=3;
			relay_off();
		}
		_print();
		
	}
	
	return 0;
}

