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

char **parse_input(char *orig_str, int *input_size)
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
    while (token != NULL)
    {
        result[i] = token;
        i++;
        token = strtok(NULL, delimiters);
    }
    result[i] = NULL;
    *input_size = i;
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

// char *read_file(char *filename)
// {
//     char *buffer = 0;
//     long length;
//     FILE *f = fopen(filename, "rb");

//     if (f)
//     {
//         fseek(f, 0, SEEK_END);
//         length = ftell(f);
//         fseek(f, 0, SEEK_SET);
//         buffer = malloc(length);
//         if (buffer)
//         {
//             fread(buffer, 1, length, f);
//         }
//         fclose(f);
//     }

//     return buffer;
// }

// int write_to_file(char *filename, char *content) //not quite working, only wirtes the first character og content
// {
//     FILE *f = fopen(filename, "wb");
//     size_t length = sizeof(*content);
//     size_t num_of_bytes;

//     if (f)
//     {
//         num_of_bytes = fwrite(content, 1, length, f);
//         fclose(f);
//         if (num_of_bytes != length)
//         {
//             printf("Error while writing from file: %s \n", filename);
//             return -1;
//         }
//         else
//         {
//             return 1;
//         }
//     }
//     else
//     {
//         puts("Error while opening file to read! \n");
//     }
// }

// maybe this should return 0 or 1 based on if it found any < or >, then separate funcion can do the removing
char **check_if_IO(char **parsed_input, int input_size)

{
    char **cleaned_parsed_input;
    cleaned_parsed_input = malloc(input_size * sizeof(char *));
    int k = 0;
    printf("Checking for I/O redirections\n");
    for (int idx = 0; idx < input_size; ++idx)
    {
        char *str = parsed_input[idx];
        if (!strcmp(str, "<"))
        {
            printf("Read found\n");
            // Read from file here
            // Use Dup2() to set fd of next element as input
            if (idx + 1 < input_size)
            {
                printf("Redirecting to %s\n", parsed_input[idx + 1]);
                int fd = open(parsed_input[idx + 1], O_RDONLY);
                dup2(fd, STDIN_FILENO);
                ++idx;
            }
        }
        else if (!strcmp(str, ">"))
        {
            printf("Write found\n");
            // Write to file here
            // Use Dup2() to set fd of next element as output
            if (idx + 1 < input_size)
            {
                printf("Redirecting to %s\n", parsed_input[idx + 1]);
                int fd = open(parsed_input[idx + 1], O_RDONLY);
                dup2(fd, STDOUT_FILENO);
                ++idx;
            }
        }
        else
        {
            cleaned_parsed_input[k] = malloc(strlen(str));
            strcpy(cleaned_parsed_input[k], str);
            ++k;
        }
        // printf("%s\n", str);
    }
    return cleaned_parsed_input;
}
// todo er å fjerne < > og fila som kommer etter fra det som blir sent
// dette står for remove redirection tokens, dvs <, > og fila som kommer etter
//Funker ikke fordi memeory allocation er feil, må nok malloce per string
char remove_rd_tokens(char **array_of_tokens)
{
    int length = sizeof(array_of_tokens) / sizeof(array_of_tokens[0]);
    char **result = malloc(length);
    int i;
    int count;

    for (i = 0; i < length - 1; i++)
    {
        char *str = array_of_tokens[i];
        if (strcmp(str, ">") && strcmp(str, "<"))
        {
            count++;
            result[i] = str;
            
        }
        else
        {
            // dont add to result and skip next one as well
            i++;
        }
    }
    printf("%s\n", result[0]);
    return result;
}

int main()
{
    // bæsj testing som ikke funker
    // char *input[5] = {"hallo", "bajs", "<", "nice", "ok"};
    // char **output = remove_rd_tokens(input);

    // printf("I main:  %s \n", input[1]);
    // for (int i = 0; i < 5; i++)
    // {
    //     printf("[%2d]: %s\n", i + 1, input[i]);
    // }

    // for (int j = 0; j < 3; j++)
    // {
    //     printf("[%2d]: %s\n", j + 1, output[j]);
    // }

    int running = 1;
    char *user_input;
    long unsigned int user_input_size = 4096;
    int bytes_read;
    user_input = (char *)malloc(user_input_size);
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
            // TODO:  parse user input. Fork here, execute command in child process (check PID) using exec(3) or maybe execvp
            // parsing

            // removes trailing newlines
            int *parsed_input_length;
            user_input[strcspn(user_input, "\r\n")] = 0;
            char **parsed_input = parse_input(user_input, parsed_input_length);
            parsed_input = check_if_IO(parsed_input, *parsed_input_length);
            // compares first word in input with "cd". Returns 0 if they're equal
            if (strcmp(parsed_input[0], "cd"))
            {
                int status = 0;
                // fork to create a child that can execute the command
                pid_t child_PID = fork();
                if (child_PID < 0)
                {
                    puts("ERROR when forking!\n");
                }
                else if (child_PID == 0)
                {
                    // in child
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
                    printf("Exit status [%s] = %d\n", user_input, status);
                    // printf(" Child procces with PID: %d finished with status: %d  \n", wpid, status);
                }
            }
            // here we cd to another directory
            else
            {
                char *path = parsed_input[1];
                int cd_error;
                cd_error = chdir(path);
            }
        }
        // free(user_input);
        //  må kanksje free det som ble malloca inni parse_input også
    }
    return 0;
}