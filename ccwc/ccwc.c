/*
Step One
In this step your goal is to write a simple version of wc, let’s call it ccwc 
(cc for Coding Challenges) that takes the command line option -c and outputs 
the number of bytes in a file.
*/

/*
Step Two
In this step your goal is to support the command line option -l that outputs the number of lines in a file.
If you’ve done it right your output should match this:
>ccwc -l test.txt
    7145 test.txt
*/

#include <stdio.h>
#include <string.h>

typedef struct {
    long bytes;
    long lines;
} Count;

typedef struct {
    int bytes;
    int lines;
} Flag;

void print_usage() {
    printf("Usage: ccwc -c -l <filename>\n");
    printf("Counts the number of bytes in the specified file.\n");
}

void count_file(FILE *fp, Count *count) {
    count->bytes = 0;
    count->lines = 0;

    int ch;

    while ((ch = fgetc(fp)) != EOF) {
        
        count->bytes++;
        if (ch == '\n') {
            count->lines++;
        }
    }
}

void print_count(Count count, Flag flag) {
    if (flag.bytes == 1)
        printf("%8ld", count.bytes);
    if (flag.lines == 1)
        printf("%8ld", count.lines);
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
    Flag flag = {0,0};
    Count count = {0,0};

    while (i < argc) {

        if (argv[i] != NULL && argv[i][0] == '-') {
            if (strcmp(argv[i], "-c") == 0)
                flag.bytes = 1;
            else if (strcmp(argv[i], "-l") == 0)
                flag.lines = 1;
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

            count_file(file, &count);
            print_count(count, flag);

            printf("  %-s\n", filename);


        } else {
            print_usage();
        }
        i++;
    }
    return 0;
}   