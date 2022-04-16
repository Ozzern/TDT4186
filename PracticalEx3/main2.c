#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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


int main()
{
    int running = 1;
    char *user_input;
    long unsigned int user_input_size = 4096;
    int bytes_read;
    user_input = (char *)malloc(user_input_size);
    char* input_file = NULL;
    char* output_file = NULL;
    while (running)
    {
        
        print_working_dir();
        bytes_read = getline(&user_input, &user_input_size, stdin);
        if (bytes_read == -1)
        {
            printf("Error when reading input\n");
        }
        else
        {
            user_input[strcspn(user_input, "\r\n")] = 0;
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
                        puts("ERROR while using execvp");
                    }

                    _exit(status);
                }

                else
                {
                    // in parent
                    pid_t wpid = waitpid(process_PID, &status, 0);
                    printf("Exit status [%s] = %d\n", user_input, status);
                }
            }
        }
    }
    return 0;
}