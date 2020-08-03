#include "calander.h"
#include <stdio.h>
#define year 2019
#define month 1
#define day 1

typedef enum bool{false, true}bool;

bool _yun(int y);
static void _calculate();
void day_update();
void yoil(char dow[]);
int mon[]={31,28,31,30,31,30,31,31,30,31,30,31};
int date[3]={year, month, day};
int yun=0;
long today=0;



static void _calculate(){


	if(yun==1){
		mon[1]=29;
	}
	else{
		mon[1]=28;
	}
	if(mon[date[1]-1]<date[2]){
		date[2]=1;
		calander_month_up();
	}

	if(date[1]>12){
		calander_year_up();
		date[1]=1;
	}
	day_update();
	
}

bool _yun(int y){
	
	if((y%400)==0){
		return true;
	}
	else if((y%100)==0){
		return false;
	}
	else if((y%4)==0){
		return true;
	}
	else {
		return false;
	}
}
void calander_init(){
	yun=_yun(year);

	
}

void day_update(){
	int temp=0;


	for(int i=year;i<date[0];i++){
		if(_yun(i)){
			temp+=366;
		}
		else{
			temp+=365;
		}
	}

	for(int i=month;i<date[1];i++){
		temp+=mon[i-1];
	}

	temp+= date[2];

	today=temp+1;
	today%=7;
}

void calander_date_up(){
	date[2]++;
	_calculate();
}
void calander_month_up(){
	date[1]++;
	_calculate();
}
void calander_year_up(){
	date[0]++;
	yun=_yun(date[0]);
	_calculate();
}
void yoil(char dow[]){
	char dayOf[7][3]={
		"Sun","Mon","Tue","Wed","Thu","Fri","Sat"
	};
	for(int i=0;i<3;i++){
		dow[i]=dayOf[today][i];
	}
}
void calander_print(char array[]){	
	char dow[3];
	
	_calculate();
	yoil(dow);
	sprintf(array, "%4d.%2d.%2d  %c%c%c", date[0], date[1], date[2], dow[0],dow[1],dow[2]);
}

int calander_get_dow(){
	return today;
}
