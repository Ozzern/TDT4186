#include <stdio.h>
#define __USE_XOPEN
#include <time.h>
#include <stdlib.h>
#include "alarm_clock.h"
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

struct AlarmStruct{
    time_t epoch_time;
    pid_t childPID;
};

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
void schedule(struct AlarmStruct *p) {
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
    if(time_diff < 0) {
        printf("You cannot set an alarm for the past\n");
    }
    else {
        printf("Scheduling alarm in %i seconds\n", (int) time_diff);
        p->childPID = fork();
        // printf("%i\n", p->childPID);
        if(p->childPID == 0){
            sleep(time_diff);
            printf("RING!");
            exit(0);
        }
        p->epoch_time = rawtime;
    }
}

// Function to display a list
void list(struct AlarmStruct ** p, int n) {
    char buff[256];
    int no_alarms = 1;
    for(int i=0; i < n; i++) {
        if(p[i]->epoch_time > 0){
            strftime(buff, 256, "%c", localtime(&p[i]->epoch_time));
            printf("Alarm %d: %s\n", (i+1), buff);
            no_alarms = 0;
        }
    }
    if(no_alarms) {
        printf("You don't have any alarms scheduled at the moment\n");
    }
}

// Function to cancel to choice
int cancel(struct AlarmStruct ** p, int n, int*alarm_index) {
    printf("What alarm would you like to cancel? ");
    int alarm_cancel = getchar() - 49;
    empty_stdin();
    if(alarm_cancel >= *alarm_index) {
        printf("You cannot cancel alarm %d, you only have %d alarms\n", (alarm_cancel + 1), *alarm_index);
        return 0;
    }
    else{
        // printf("CHILD PID: %i\n", p[alarm_cancel]->childPID);
        int val = kill(p[alarm_cancel]->childPID, SIGKILL);
        if(val == 0) {
        // printf("%i", val);
            *alarm_index = *alarm_index - 1;
            delete_from_array(p, alarm_cancel, n);
            return 1;
        }
    }
    return 0;
}

// Function to exit the program
void exit_program() {
    printf("Exiting, goodbye!\n");
    exit(EXIT_SUCCESS);
}

const int MAX_ALARMS = 5;

time_t get_current_time() {
    time_t current_time;
    time(&current_time);
    return current_time;
}

void delete_from_array(struct AlarmStruct ** p, int index_remove, int length) {
    for(int i=index_remove; i < length - 1; i++){
        p[i] = p[i+1];
    }
    // p[length]->epoch_time = 0;
    // p[length]->childPID = 0;
    printf("Successfully deleted alarm %d\n", (index_remove+1));
}

int kill_zombies(struct AlarmStruct ** alarms) {
    int wstatus;
    int end_PID;
    end_PID = waitpid(-1, &wstatus, WNOHANG|WUNTRACED);
    for(int i=0; i < MAX_ALARMS; i++){
        if(alarms[i]->childPID > 0 && alarms[i]->childPID == end_PID) {
            exit(EXIT_SUCCESS);
        }
    }
    return wstatus;
}

void alarm_system() {
    struct AlarmStruct * alarms[MAX_ALARMS];
    for(int i=0; i < MAX_ALARMS; i++){
        alarms[i] = malloc(sizeof(struct AlarmStruct));
        alarms[i]->epoch_time = 0;
        alarms[i]->childPID = 0;
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
    int cancelled;
    while(flag){
        choice = get_choice();
        switch (choice)
        {
        case 's':
            if(alarm_index < MAX_ALARMS){
                schedule((*p)[alarm_index]);
                alarm_index = alarm_index + 1;
            }
            else {
                printf("You cannot schedule more than %d alarms simultaneously. Please cancel an alarm before setting a new one\n", MAX_ALARMS);
            }
            break;
        case 'l':
            list((*p), MAX_ALARMS);
            break;
        case 'c':
            cancelled = cancel((*p), MAX_ALARMS, &alarm_index);
            break;
        case 'x':
            flag = 0;           // not necessary as exit_program() calls exit()
            exit_program();
            break;
        default:
            printf("Choice was not one of the four (s, l, c or x). Please try again!\n");
        }
        kill_zombies(*p);       // kill zombies periodically after each run of the main loop
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
//End of file