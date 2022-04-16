#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "dll.h"

int print_working_dir()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s: ", cwd);
    }
    else
    {
        perror("Could not get working directory");
        return 1;
    }
    return 0;
}

char **parse_input(char *orig_str, char** input_file, char** output_file)
{
    char delimiters[] = " \t";
    // makes copy of str so original is not altered
    char *str;
    str = strdup(orig_str);
    char *token = strtok(str, delimiters);

    int init_size = strlen(str);
    // char *strings[init_size];
    char **result = malloc(init_size);
    int i = 0;
    int next_is_read = 0;
    int next_is_write = 0;
    while (token != NULL)
    {
        if(next_is_read){
            next_is_read = 0;
            *input_file = token;
        }
        else if(next_is_write){
            next_is_write = 0;
            *output_file = token;
        }
        else if(!strcmp(token, "<")){
            next_is_read = 1;
        }
        else if(!strcmp(token, ">")){
            next_is_write = 1;
        }
        else{
            result[i] = token;
            i++;
        }
        token = strtok(NULL, delimiters);
    }
    result[i] = NULL;
    return result;
}

int set_input(char* filename) {
    int fd = open(filename, O_RDONLY);
    return dup2(fd, STDIN_FILENO);
}

int set_output(char* filename) {
    int fd = open(filename, O_RDWR | O_CREAT | O_APPEND, 0644); // CHMOD 644, everyone can open the file
    return dup2(fd, STDOUT_FILENO);
}

struct process_info {
    struct list_head head;
    pid_t pid;
    char* command;
};

void print_procs(struct process_info *process_list, struct list_head head) {
    struct process_info *elem = process_list;
    elem = (struct process_info *) elem->head.next;
    int wstatus;
    int end_PID;
    while(process_list != elem) {
        end_PID = waitpid(elem->pid, &wstatus, WNOHANG|WUNTRACED);
        if(end_PID){
            printf("Exit status [%s] = %d\n", elem->command, wstatus);
            // TODO: DELETE FROM LIST AFTER PROCESS HAS EXITED
            // list_del((struct list_head *)elem);
            // list_empty(&head);
        }
        elem = (struct process_info *) elem->head.next;
    }
}

int main()
{
    int running = 1;
    char *user_input;
    long unsigned int user_input_size = 4096;
    int bytes_read;
    user_input = (char *)malloc(user_input_size);
    char* input_file = NULL;
    char* output_file = NULL;
    struct list_head head;
    list_init(&head);

    while (running)
    {
        // TODO: LOOP THROUGH LINKED LIST, COLLECT ZOMBIES HERE
        print_procs((struct process_info *) &head, head);

        print_working_dir();
        bytes_read = getline(&user_input, &user_input_size, stdin);
        if (bytes_read == -1)
        {
            printf("Error when reading input\n");
        }
        else if(bytes_read == 1){
            printf("Input empty\n");
        }
        else
        {
            user_input[strcspn(user_input, "\r\n")] = 0;
            // TODO: REMOVE TRAILING WHITESPACES HERE
            const char final_letter = user_input[strlen(user_input)-1];
            if(final_letter == '&'){
                user_input[strlen(user_input) - 1] = '\0';
            }
            input_file = NULL;
            output_file = NULL;
            char **parsed_input = parse_input(user_input, &input_file, &output_file);
            // compares first word in input with "cd"
            // here we cd
            if (!strcmp(parsed_input[0], "cd"))
            {
                char *path = parsed_input[1];
                int cd_error;
                cd_error = chdir(path);
            }
            // here we dont cd
            else
            {
                int status = 0;
                // fork to create a child that can execute the command
                pid_t process_PID = fork();
                if (process_PID < 0)
                {
                    puts("ERROR when forking!\n");
                }
                else if (process_PID == 0)
                {
                    // in child
                    if(input_file){
                        set_input(input_file);
                    }
                    if(output_file){
                        set_output(output_file);
                    }
                    status = execvp(parsed_input[0], parsed_input);

                    if (status == -1)
                    {
                        printf("ERROR while using execvp\n");
                    }

                    _exit(status);
                }
                else
                {
                    // in parent

                    if(final_letter == '&'){
                        // Add process to linked list
                        struct process_info *new_process;
                        new_process = malloc(sizeof(struct process_info));
                        new_process->pid = process_PID;
                        new_process->command = malloc(strlen(user_input));
                        new_process->command = user_input;
                        list_add(&(new_process->head), &head);
                    }
                    else {
                        pid_t wpid = waitpid(process_PID, &status, 0);
                        printf("Exit status [%s] = %d\n", user_input, status);
                    }
                }
            }
        }
    }
    return 0;
}