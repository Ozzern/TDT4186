#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

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
    char *strings[init_size];
    char **result = strings;
    int i = 0;
    while (token != NULL)
    {
        result[i] = token;
        printf("%s\n", token);
        i++;
        token = strtok(NULL, delimiters);
    }
    result[i] = NULL;

    return result;
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
            printf("%s", user_input);
            // TODO:  parse user input .fork here, execute command in child process (check PID) using exec(3) or maybe execvp
            // parsing
            char **parsed_input = parse_input(user_input);
            
        }
    }
    return 0;
}