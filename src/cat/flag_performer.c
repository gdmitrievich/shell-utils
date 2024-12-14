#include "flag_performer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/common.h"

void process_flags(flags flags, int first_filepath_idx, int argc, char** argv) {
    char line[1024] = {0};
    int i = first_filepath_idx;
    while (i < argc) {
        FILE* f = fopen(argv[i], "r");
        if (f) {
            char* read = NULL;
            while ((read = fgets(line, sizeof(line), f))) {
                if (!has_new_line_char_at_the_end(line) && fpeek(f) == EOF && i + 1 < argc) {
                    fclose(f);
                    f = NULL;
                    ++i;
                    f = read_line_in_new_file(line, &i, argc, argv);
                    if (!f) break;
                }
                process_flags_on_line(flags, line);
            }

            fclose(f);
        } else {
            print_error("cat", argv[i]);
        }
        ++i;
    }
}

int fpeek(FILE* f) {
    if (!f) return -1;

    int c = fgetc(f);
    return ungetc(c, f);
}

FILE* read_line_in_new_file(char* line, int* i, int argc, char** argv) {
    FILE* f = fopen(argv[*i], "r");
    if (f) {
        char l[1024] = {0};
        if (fgets(l, sizeof(l), f)) {
            strcat(line, l);

            if (!has_new_line_char_at_the_end(line) && fpeek(f) == EOF && *i + 1 < argc) {
                fclose(f);
                f = NULL;
                ++*i;
                f = read_line_in_new_file(line, i, argc, argv);
            }
        }
    } else {
        print_error("cat", argv[*i]);
		f = NULL;
    }

    return f;
}

void process_flags_on_line(flags flags, char* line) {
    if (!line) return;

    if (flags.v) process_v_flag_on_line(line);
    if (flags.b) process_b_flag_on_line(line);
    if (flags.E) process_E_flag_on_line(line);
    if (flags.n) process_n_flag_on_line(line);
    if (flags.s) process_s_flag_on_line(line);
    if (flags.T) process_T_flag_on_line(line);
}

void process_b_flag_on_line(const char* line) {
    if (!line) return;

    if (!is_fully_empty_line(line))
        process_n_flag_on_line(line);
    else
        printf("%s", line);
}

int is_fully_empty_line(const char* line) { return is_new_line_char(line[0]); }

void process_E_flag_on_line(const char* line) {
    if (!line) return;

    if (has_new_line_char_at_the_end(line)) {
        print_chars_until_new_line_char(line);
        printf("$\n");
    } else {
        printf("%s", line);
    }
}

void print_chars_until_new_line_char(const char* line) {
    if (!line) return;

    size_t l = strlen(line);
    for (size_t i = 0; i < l && !is_new_line_char(line[i]); ++i) printf("%c", line[i]);
}

void process_n_flag_on_line(const char* line) {
    if (!line) return;

    static int nLine = 1;
    printf("%6d\t%s", nLine++, line);
}

void process_s_flag_on_line(const char* line) {
    if (!line) return;

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
    if (!line) return;

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

    char* new_line = (char*)try_allocate_memory("cat", strlen(line) * 4 + 1);

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