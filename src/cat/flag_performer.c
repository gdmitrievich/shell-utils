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

int has_new_line_char_at_the_end(const char* line) { return is_new_line_char(line[strlen(line) - 1]); }

int is_new_line_char(char ch) { return ch == '\n'; }

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
    if (flags.v) process_v_flag_on_line(line);
    if (flags.b) process_b_flag_on_line(line);
    if (flags.E) process_E_flag_on_line(line);
    if (flags.n) process_n_flag_on_line(line);
    if (flags.s) process_s_flag_on_line(line);
    if (flags.T) process_T_flag_on_line(line);
}

void process_b_flag_on_line(const char* line) {
    if (!is_fully_empty_line(line))
        process_n_flag_on_line(line);
    else
        printf("%s", line);
}

int is_fully_empty_line(const char* line) { return is_new_line_char(line[0]); }

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
    for (size_t i = 0; i < l && !is_new_line_char(line[i]); ++i) printf("%c", line[i]);
}

void process_n_flag_on_line(const char* line) {
    static int nLine = 1;
    printf("%6d\t%s", nLine++, line);
}

void process_s_flag_on_line(const char* line) {
    static int n = 0;
    if (is_fully_empty_line(line) && n == 0) {
        printf("\n");
        n++;
    } else if (!is_fully_empty_line(line)) {
        n = 0;
        printf("%s", line);
    }
}

void process_T_flag_on_line(const char* line) {
    size_t l = strlen(line);
    for (size_t i = 0; i < l; ++i) {
        if (is_tab(line[i]))
            printf("^I");
        else
            printf("%c", line[i]);
    }
}

int is_tab(char ch) { return ch == '\t'; }

void process_v_flag_on_line(char* line) {
    if (!line) return;

    char* new_line = calloc(strlen(line) * 4 + 1, sizeof(char));
    if (!new_line) {
        perror("cat");
        exit(EXIT_FAILURE);
    }

    size_t l = strlen(line);
    for (size_t i = 0; i < l; ++i) {
        unsigned char c = line[i];
        if (is_new_line_char(c) || is_tab(c))
            strcat_formated_char_as_str(new_line, "%c", c);
        else if (c == 127)
            strcat_formated_char_as_str(new_line, "^%c", c - 64);
        else if (c < 32)
            strcat_formated_char_as_str(new_line, "^%c", c + 64);
        else if (c < 128)
            strcat_formated_char_as_str(new_line, "%c", c);
        else if (c < 160)
            strcat_formated_char_as_str(new_line, "M-^%c", c - 64);
        else if (c < 255)
            strcat_formated_char_as_str(new_line, "M-%c", c - 128);
        else
            strcat_formated_char_as_str(new_line, "M-%c", c - 192);
    }

    strncpy(line, new_line, strlen(new_line) + 1);
    free(new_line);
}

void strcat_formated_char_as_str(char* dest, const char* format, unsigned char ch) {
    char str[5] = {0};
    snprintf(str, sizeof(str), format, ch);
    strcat(dest, str);
}