#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

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

char **parse_input(char str[])
{
    char delimiters[] = " \t";
    char *token = strtok(str, delimiters);

    int init_size = strlen(str);
    // char *strings[init_size];
    char **result = malloc(init_size);
    int i = 0;
    while (token != NULL)
    {
        result[i] = token;
        i++;
        token = strtok(NULL, delimiters);
    }
    result[i] = NULL;

    return result;
}

// tror vi må gjøre dette før vi parser inputen, men når jeg prøvde å teste det så fikk jeg segmentation fault
char *trim_trailing_whitespace(char *str)
{
    int index, i;

    /* Set default index */
    index = -1;

    /* Find last index of non-white space character */
    i = 0;
    while (str[i] != '\0')
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            index = i;
        }

        i++;
    }

    /* Mark next character to last non-white space character as NULL */
    str[index + 1] = '\0';
    return str;
}

int main()
{

    int running = 1;
    char *user_input;
    long unsigned int user_input_size = 1024;
    int bytes_read;
    user_input = (char *)malloc(user_input_size);
    while (running)
    {
        print_working_dir();
        bytes_read = getline(&user_input, &user_input_size, stdin);
        if (bytes_read == -1)
        {
            printf("Error when reading input");
        }
        else
        {
            // TODO:  parse user input .fork here, execute command in child process (check PID) using exec(3) or maybe execvp
            // parsing
            char **parsed_input = parse_input(user_input);

            int status = 0;
            // fork to create a child that can execute the command
            pid_t child_PID = fork();
            if (child_PID < 0)
            {
                puts("ERROR when forking!");
            }
            else if (child_PID == 0)
            {
                // in child
                puts("in child");
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
                pid_t wpid = waitpid(child_PID, &status, 0);
                printf(" Child procces with PID: %d finished with status: %d  \n", wpid, status);
            }
        }
        free(user_input);
        //  må kanksje free det som ble malloca inni parse_input også
    }
    return 0;
}