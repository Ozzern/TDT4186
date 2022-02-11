#include <stdio.h>
#define __USE_XOPEN
#include <time.h>
#include <stdlib.h>
#include "alarm_clock.h"

void empty_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

// Prompts the user for a choice and returns it
char get_choice() {
    printf("Please enter \"s\" (schedule), \"l\" (list), \"c\" (cancel), \"x\" (exit): ");
    char * line;
    char first_letter;
    line = malloc(sizeof(char)*256);
    if (fgets(line, sizeof(line), stdin)) {
        first_letter = line[0];
    }
    return first_letter;
}

// Function to schedule an alarm
time_t schedule(struct AlarmStruct *p) {
    printf("When would you like to schedule an alarm (yyyy-mm-dd hh:mm:ss)? ");
    char line[19];
    struct tm timeinfo;
    time_t rawtime;
    for(int i=0; i < 19; i++){
        line[i] = getchar();
    }
    empty_stdin();
    strptime(line, "%Y-%m-%d %H:%M:%S", &timeinfo);
    rawtime = mktime(&timeinfo);
    time_t current_time = get_current_time();
    double time_diff = difftime(rawtime, current_time);
    printf("Scheduling alarm in %i seconds\n", (int) time_diff);
    p->epoch_time = rawtime;
    p->childPID = 1;
}

// Function to display a list
void list(struct AlarmStruct[]*p) {
    char buff[19];
    for(int i=0; i < n; i++){
        /*
        if(alarms[i]->rawtime > 0){
            strftime(buff, 19, "%Y-%m-%d %H:%M:%S", localtime(&alarms[i].rawtime));
            printf("Alarm %d: %s\n", (i+1), buff);
        */
        }
    }
}

// Function to cancel to choice
void cancel() {
    printf("Cancel\n");
}

// Function to exit the program
void exit_program() {
    printf("Exiting, goodbye!\n");
    exit(0);
}

const int n = 100;
// struct tm times[n];

time_t get_current_time() {
    time_t current_time;
    time(&current_time);
    return current_time;
}

struct AlarmStruct{
    time_t epoch_time;
    pid_t childPID;
};

void alarm_system() {
    struct AlarmStruct * alarms[5];
    for(int i=0; i < 5; i++){
        alarms[i] = malloc(sizeof(struct AlarmStruct));
    }
    struct AlarmStruct *(*p)[] = &alarms;
    int alarm_index = 0;

    time_t current_time;
    struct tm * current_time_info;

    time(&current_time);
    current_time_info = localtime(&current_time);

    printf("Welcome to the alarm clock! It is currently %s", asctime(current_time_info));
    char choice;
    int flag = 1;
    while(flag){
        choice = get_choice();
        switch (choice)
        {
        case 's':
            schedule((*p)[alarm_index]);
            alarm_index = alarm_index + 1;
            break;
        case 'l':
            list((*p));
            break;
        case 'c':
            cancel();
            break;
        case 'x':
            flag = 0;           // not necessary as exit_program() calls exit()
            exit_program();
            break;
        default:
            printf("Choice was not one of the four (s, l, c or x). Please try again!\n");
        }
    }
}

// The main function
int main() {
    alarm_system();
//    struct tm new_time;
//    char * string_time;
//    string_time = malloc(sizeof(char)*256);
//    strptime("01", "%d", &timeinfo);
//    strftime(string_time, 256, "%d", &new_time)
    return 0;
}