#include <stdio.h>
#include <time.h>

char get_choice();
char get_time();
time_t get_current_time();

time_t schedule();
void list();
void cancel();
void exit_program();

void empty_stdin();

struct AlarmStruct;

void alarm_system();