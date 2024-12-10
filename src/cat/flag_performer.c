#include "flag_performer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_flags(flags flags, int first_filepath_idx, int argc, char** argv) {
    char line[512] = {0};
    int i = first_filepath_idx;
    while (i < argc) {
        char* state = NULL;
        FILE* f = fopen(argv[i], "r");
        if (!f) {
            perror("cat");
            exit(EXIT_FAILURE);
        }

        while ((state = fgets(line, sizeof(line), f))) {
            if (!has_new_line_char_at_the_end(line) && fpeek(f) == EOF && i + 1 < argc) {
                ++i;
                fclose(f);
                f = read_new_line(line, &i, argc, argv);
            }
            process_flags_on_line(flags, line);
        }

        fclose(f);
        ++i;
    }
}

int has_new_line_char_at_the_end(const char* line) { return line[strlen(line) - 1] == '\n'; }

int fpeek(FILE* f) {
    int c = fgetc(f);
    return ungetc(c, f);
}

FILE* read_new_line(char* line, int* i, int argc, char** argv) {
    FILE* f = fopen(argv[*i], "r");
    if (f == NULL) {
        perror("cat");
        exit(EXIT_FAILURE);
    }
    char l[256] = {0};
    if (fgets(l, sizeof(l), f)) {
        strcat(line, l);

        if (!has_new_line_char_at_the_end(line) && fpeek(f) == EOF && *i + 1 < argc) {
            ++*i;
            fclose(f);
            f = read_new_line(line, i, argc, argv);
        }
    }

    return f;
}

void process_flags_on_line(flags flags, char* line) {
    if (flags.b) process_b_flag_on_line(line);
    if (flags.E) process_E_flag_on_line(line);
    if (flags.n) process_n_flag_on_line(line);
    // if (flags.s) process_s_flag_on_line(line);
    // if (flags.T) process_T_flag_on_line(line);
    // if (flags.v) process_v_flag_on_line(line);
}

void process_b_flag_on_line(const char* line) {
    if (!is_fully_empty_line(line))
        process_n_flag_on_line(line);
    else
        printf("%s", line);
}

int is_fully_empty_line(const char* line) { return line[0] == '\n'; }

void process_E_flag_on_line(const char* line) {
    if (has_new_line_char_at_the_end(line)) {
        print_chars_until_new_line_char(line);
        printf("$\n");
    } else {
        printf("%s", line);
    }
}

void print_chars_until_new_line_char(const char* line) {
    size_t l = strlen(line);
    for (size_t i = 0; i < l && line[i] != '\n'; ++i) printf("%c", line[i]);
}

void process_n_flag_on_line(const char* line) {
    static int nLine = 1;
    printf("%6d\t%s", nLine++, line);
}
