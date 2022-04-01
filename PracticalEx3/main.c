#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

int print_working_dir() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s: ", cwd);
    } else {
        perror("Could not get working directory");
        return 1;
    }
    return 0;
}

int main(){
    int running = 1;
    char* user_input;
    long unsigned int user_input_size = 1024;
    int bytes_read;
    user_input = (char *) malloc (user_input_size);
    while(running) {
        print_working_dir();
        bytes_read = getline(&user_input, &user_input_size, stdin);
        if(bytes_read == -1) {
            printf("Error when reading input");
        }
        else {
            printf("%s", user_input);
        }

    }
	return 0;
}