/*
Step One
In this step your goal is to write a simple version of wc, let’s call it ccwc 
(cc for Coding Challenges) that takes the command line option -c and outputs 
the number of bytes in a file.
*/

#include <stdio.h>
#include <string.h>

void print_usage() {
    printf("Usage: ccwc -c <filename>\n");
    printf("Counts the number of bytes in the specified file.\n");
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        print_usage();
        return 1;
    } else if (argc == 2 && argv[1][0] == '-') {
        print_usage();
        return 1;        
    }

    int i = 1;
    int byte_count_flag = 0;
    int byte_count = 0;

    while (i < argc) {

        if (argv[i] != NULL && argv[i][0] == '-') {
            if (strcmp(argv[i], "-c") == 0)
                byte_count_flag = 1;
            else {
                print_usage();
                return 1;
            }
        } else if (argv[i] != NULL) {
            const char *filename = argv[i];
            FILE *file = fopen(filename, "rb");
            if (!file) {
                perror("Error opening file");
                return 1;
            }

            int ch;
            while ((ch = fgetc(file)) != EOF)
                byte_count++;

            printf("%10d", byte_count);
            printf("  %-s\n", filename);

        } else {
            print_usage();
        }
        i++;
    }
    return 0;
}   