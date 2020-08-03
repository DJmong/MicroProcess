#ifndef _ALRAM_H_
#define _ALRAM_H_




void alram_setup();
void alram_countup();
void alram_change();
void alram_countclear();
void alram_init();

int alram_check(int hour, int minute, int dow);
void alram_set_date_print1(char str[]);
void alram_set_date_print2(char str[]);
void alram_timer_print(char str[]);


#endif
