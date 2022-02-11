#include <stdio.h>
#define __USE_XOPEN
#include <time.h>
#include <stdlib.h>
#include "alarm_clock.h"

void empty_stdin() {
    char c = getchar();
    while (c != '\n' && c != 'EOF') {
        c = getchar();
    }
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

// Function to view schedule
time_t schedule() {
    printf("When would you like to schedule an alarm (yyyy-mm-dd hh:mm:ss)? ");
    char * line;
    struct tm timeinfo;
    time_t rawtime;
    line = malloc(sizeof(char)*256);
    if (fgets(line, sizeof(line), stdin)) {
        strptime("2022-02-10 23:23:23", "%Y-%m-%d %H:%M:%S", &timeinfo);
        rawtime = mktime(&timeinfo);
    }
    time_t current_time = get_current_time();
    double time_diff = difftime(rawtime, current_time);
    printf("Scheduling alarm in %i seconds\n", (int) time_diff);
    return rawtime;
}

// Function to display a list
void list() {
    printf("List\n");
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

void alarm_system() {
    time_t alarms[5];
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
            alarms[alarm_index] = schedule();
            alarm_index = alarm_index + 1;
            break;
        case 'l':
            list();
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