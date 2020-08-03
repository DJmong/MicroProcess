#include "alram.h"
#include <stdio.h>
#include <string.h>
int alram_time[2]={0,0}; 
int alram_set=0;
int alram_day[7]={0,0,0,0,0,0,0};
int count=0;
char dayOf[][2]={
		"S","M","T","W","T","F","S"
	};

static void alram_minute_up();
static void alram_hour_up();


void alram_setup(){
	alram_set^=1;
}

void alram_countup(){
	if(++count>8) count=0;
}

void alram_change(){
	if(count<7) alram_day[count]^=0x01;
	else if(count==7) alram_hour_up();
	else if(count==8) alram_minute_up();
	
}

void alram_countclear(){
	count=0;
}


int alram_check(int hour, int minute, int dow){
	if(alram_set==1){
		if(alram_day[dow]){
			if(hour==alram_time[0]){
				if(minute==alram_time[1]){
					return 1;
				}
			}
		}		
	}
	return 0;
}

void alram_set_date_print1(char str[]){
	str[0]='\0';
	int hour, minute;
	char ampm='A';
	if(count<7){
		sprintf(str, "alram ");
		for(int i=0;i<7;i++){
			strcat(str, dayOf[i]);
		}
		strcat(str, "   ");
	}
	else{
		
		if(alram_time[0]>11) ampm='P';
		
		hour=alram_time[0]%12;
		if(!(hour)) hour=12;
		minute=alram_time[1];
		sprintf(str, "time %cM %d%d:%d%d", ampm, hour/10, hour%10, minute/10, minute%10);
		if(count==7) strcat(str, "* ");
		else if(count==8) strcat(str, " *");
		else strcat(str, "  ");
	}
}

void alram_set_date_print2(char str[]){
	str[0]='\0';
	if(count<7){
		if(alram_set) sprintf(str, "set   ");
		else sprintf(str, "off   ");
		for(int i=0;i<7;i++){
			if(alram_day[i]) strcat(str, "O");
			else if(i==count) strcat(str, "*");
			else strcat(str, " ");
		}
		strcat(str, "   ");
	}
	else{
		sprintf(str, "date ");
		for(int i=0;i<7;i++){
			if(alram_day[i]==1){
				strcat(str, dayOf[i]);
			}
			else{
				strcat(str, " ");
			}
		}
		strcat(str, "   ");
	}
	
}


static void alram_hour_up(){
	alram_time[0]++;
	if(alram_time[0]>23){
		alram_time[0]=0;
	}
}

static void alram_minute_up(){
	alram_time[1]++;
	if(alram_time[1]>59){
		alram_time[1]=0;
	}
}
