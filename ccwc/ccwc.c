/*
Step One
In this step your goal is to write a simple version of wc, let’s call it ccwc 
(cc for Coding Challenges) that takes the command line option -c and outputs 
the number of bytes in a file.
*/
/*
Step Two
In this step your goal is to support the command line option -l that outputs 
the number of lines in a file.
If you’ve done it right your output should match this:
>ccwc -l test.txt
    7145 test.txt
*/
/*
Step Three
In this step your goal is to support the command line option -w that outputs 
the number of words in a file. If you’ve done it right your output should match this:
>ccwc -w test.txt
   58164 test.txt
*/
/*
Step Four
In this step your goal is to support the command line option -m that outputs the number 
of characters in a file. If the current locale does not support multibyte characters this 
will match the -c option.
For this one your answer will depend on your locale, so if can, use wc itself and compare the output to your solution:
*/
/*
Step Five
In this step your goal is to support the default option - i.e. no options are provided, 
which is the equivalent to the -c, -l and -w options. If you’ve done it right your output 
should match this:
>ccwc test.txt
    7145   58164  342190 test.txt
*/
/*
The Final Step
In this step your goal is to support being able to read from standard input if no filename 
is specified. If you’ve done it right your output should match this:
>cat test.txt | ccwc -l
    7145
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <wchar.h>

typedef struct {
    long bytes;
    long lines;
    long words;
    long chars;
} Count;

typedef struct {
    int bytes;
    int lines;
    int words;
    int chars;
} Flag;

void print_usage() {
    printf("Usage: ccwc -c -l -w -m <filename> <filename>\n");
    printf("Counts the number of bytes, lines, words, characters in the specified file.\n");
}

void count_file(FILE *fp, Count *count) {

    setlocale(LC_CTYPE, "");
    mbstate_t ps;
    memset(&ps, 0, sizeof(ps));
    
    count->bytes = 0;
    count->lines = 0;
    count->words = 0;
    count->chars = 0;

    int ch;
    int inside_word =0;

    while ((ch = fgetc(fp)) != EOF) {
        
        count->bytes++;
        unsigned char byte = (unsigned char)ch;

        wchar_t wc;
        size_t ret = mbrtowc(&wc, (const char *)&byte, 1, &ps);
        if (ret == (size_t)-2) {
            continue;
        } else if (ret == (size_t)-1) {
            memset(&ps, 0, sizeof(ps));
        } else {
            count->chars++;
        }
        if (!mbsinit(&ps)) {
            count->chars++;
            memset(&ps, 0, sizeof(ps));
        }

        if (ch == '\n') {
            count->lines++;
        }

        if (isspace(ch)) {
            if (inside_word == 1)
                inside_word = 0;
        } else {
            if (inside_word == 0) {
                inside_word = 1;
                count->words++;
            }
                
        }

    }
}

void print_count(Count count, Flag flag) {
    int no_flags = flag.bytes || flag.lines || flag.words || flag.chars;
    if (!no_flags || flag.bytes == 1)
        printf("%8ld", count.bytes);
    if (!no_flags || flag.lines == 1)
        printf("%8ld", count.lines);
    if (!no_flags || flag.words == 1)
        printf("%8ld", count.words);
    if (flag.chars == 1)
        printf("%8ld", count.chars);
} 

int main(int argc, char *argv[]) {

    int i = 1;
    Flag flag = {0, 0, 0, 0};
    Count count = {0, 0, 0, 0};

    while (i <= argc) {

        if (argv[i] != NULL && argv[i][0] == '-') {
            if (strcmp(argv[i], "-c") == 0)
                flag.bytes = 1;
            else if (strcmp(argv[i], "-l") == 0)
                flag.lines = 1;
            else if (strcmp(argv[i], "-w") == 0)
                flag.words = 1;
            else if (strcmp(argv[i], "-m") == 0)
                flag.chars = 1;
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
            count_file(stdin, &count);
            print_count(count, flag);
            printf("\n");            
        }
        i++;
    }
    return 0;
}   