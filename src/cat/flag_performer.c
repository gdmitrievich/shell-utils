#include "flag_performer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool process_flags(flags flags, int first_filepath_idx, int argc, char** argv) {
    int i = first_filepath_idx;
    bool status = true;
    while (status && i < argc) {
        FILE* f = fopen(argv[i], "r");
        if (f) {
    		char* line = NULL;
    		size_t line_len = 0;
            while (status && (fgetdyns(&line, &line_len, f))) {
                if (!has_new_line_char_at_the_end(line) && fpeek(f) == EOF && i + 1 < argc) {
                    fclose(f);
                    f = NULL;
                    ++i;
                    f = read_line_in_new_file(&line, &i, argc, argv);
                    if (!f) break;
                }
                status = process_flags_on_line(flags, line);
				// free(line);
				// line = NULL;
            }

            fclose(f);
        } else {
            print_error("cat", argv[i]);
        }
        ++i;
    }

    return status;
}

int fpeek(FILE* f) {
    if (!f) return -1;

    int c = fgetc(f);
    return ungetc(c, f);
}

FILE* read_line_in_new_file(char** line, int* i, int argc, char** argv) {
    bool status = true;
    FILE* f = fopen(argv[*i], "r");
    if (f) {
        char* l = NULL;
        size_t line_len = 0;
        if (fgetdyns(&l, &line_len, f)) {
            if (!append_str(line, l)) status = false;

            if (status && !has_new_line_char_at_the_end(*line) && fpeek(f) == EOF && *i + 1 < argc) {
                fclose(f);
                f = NULL;
                ++*i;
                f = read_line_in_new_file(line, i, argc, argv);
            }
        }
		//free(l);
    } else {
        print_error("cat", argv[*i]);
        f = NULL;
    }

    return status ? f : NULL;
}

bool process_flags_on_line(flags flags, char* line) {
    if (!line) return false;

    bool status = true;
    if (flags.v) status = process_v_flag_on_line(line);
    if (status) {
        if (flags.b) process_b_flag_on_line(line);
        if (flags.E) process_E_flag_on_line(line);
        if (flags.n) process_n_flag_on_line(line);
        if (flags.s) process_s_flag_on_line(line);
        if (flags.T) process_T_flag_on_line(line);
    }
	if (!is_at_least_one_flag_set(&flags))
		printf("%s", line);

    return status;
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

bool process_v_flag_on_line(char* line) {
    if (!line) return false;

    bool has_error = false;
    char* ptr = (char*)allocate_with_memset(strlen(line) * 4 + 1);
    if (ptr) {
        char* new_line = ptr;
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
    } else {
        has_error = true;
    }

    return !has_error;
}

void strcat_formated_char_as_str(char* dest, const char* format, unsigned char ch) {
    char str[5] = {0};
    snprintf(str, sizeof(str), format, ch);
    strcat(dest, str);
}