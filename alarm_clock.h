#include <stdio.h>
#include <time.h>

char get_choice();
char get_time();
time_t get_current_time();

void schedule();
void list();
int cancel();
void exit_program();

int kill_zombies();

void empty_stdin();

struct AlarmStruct;

void alarm_system();
void delete_from_array();

const int MAX_ALARMS;