#include "flag_performer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool process_flags(flags flags, int first_filepath_idx, int argc, char** argv) {
    int i = first_filepath_idx;
    bool status = true;
    while (status && i < argc) {
        FILE* f = fopen(argv[i], "r");
        if (f) {
            char* line = NULL;
            size_t line_len = 0;
            while (status && f && (fgetdyns(&line, &line_len, f))) {
                if (!has_new_line_char_at_the_end(line, line_len) && fpeek(f) == EOF && i + 1 < argc) {
                    fclose(f);
                    f = NULL;
                    ++i;
                    f = read_line_in_new_file(&line, &line_len, &i, argc, argv);
                }
                status = process_flags_on_line(flags, &line, line_len);
                free(line);
                line = NULL;
            }

            if (f) fclose(f);
        } else {
            print_error("cat", argv[i]);
        }
        ++i;
    }

    return status;
}

int has_new_line_char_at_the_end(const char* line, size_t len) { return is_new_line_char(line[len - 1]); }

int fpeek(FILE* f) {
    if (!f) return -1;

    int c = fgetc(f);
    return ungetc(c, f);
}

FILE* read_line_in_new_file(char** line, size_t* line_len_ptr, int* i, int argc, char** argv) {
    bool status = true;
    FILE* f = fopen(argv[*i], "r");
    if (f) {
        char* l = NULL;
        size_t line_len = 0;
        if (fgetdyns(&l, &line_len, f)) {
            if (!append_str(line, l)) status = false;
            if (status) *line_len_ptr += line_len;
            if (status && !has_new_line_char_at_the_end(*line, *line_len_ptr) && fpeek(f) == EOF &&
                *i + 1 < argc) {
                fclose(f);
                f = NULL;
                ++*i;
                f = read_line_in_new_file(line, line_len_ptr, i, argc, argv);
            }
        }
        free(l);
    } else {
        print_error("cat", argv[*i]);
        f = NULL;
        status = false;
    }

    return status ? f : NULL;
}

bool process_flags_on_line(flags flags, char** line_ptr, size_t line_len) {
    if (!*line_ptr) return false;

    bool status = true;
    if (flags.v) status = process_v_flag_on_line(line_ptr, &line_len);
    if (status) {
        if (flags.b) process_b_flag_on_line(*line_ptr, line_len);
        if (flags.E) process_E_flag_on_line(*line_ptr, line_len);
        if (flags.n) process_n_flag_on_line(*line_ptr, line_len);
        if (flags.s) process_s_flag_on_line(*line_ptr, line_len);
        if (flags.T) process_T_flag_on_line(*line_ptr, line_len);
    }
    if (!is_at_least_one_flag_set(&flags)) write(STDOUT_FILENO, *line_ptr, line_len);

    return status;
}

void process_b_flag_on_line(const char* line, size_t line_len) {
    if (!line) return;

    if (!is_fully_empty_line(line))
        process_n_flag_on_line(line, line_len);
    else
        write(STDOUT_FILENO, line, line_len);
}

int is_fully_empty_line(const char* line) { return is_new_line_char(line[0]); }

void process_E_flag_on_line(const char* line, size_t line_len) {
    if (!line) return;

    if (has_new_line_char_at_the_end(line, line_len)) {
        print_chars_until_new_line_char(line, line_len);
        write(STDOUT_FILENO, "$\n", 2);
    } else {
        write(STDOUT_FILENO, line, line_len);
    }
}

void print_chars_until_new_line_char(const char* line, size_t line_len) {
    if (!line) return;

    for (size_t i = 0; i < line_len && !is_new_line_char(line[i]); ++i) write(STDOUT_FILENO, &line[i], 1);
}

void process_n_flag_on_line(const char* line, size_t line_len) {
    if (!line) return;

    static int nLine = 1;
    const int SPACES_COUNT = 6 + 1;
    char str[line_len + SPACES_COUNT + 1];
    memset(str, 0, sizeof(str));
    snprintf(str, sizeof(str), "%6d\t", nLine++);
    memcpy(str + SPACES_COUNT, line, line_len + 1);
    write(STDOUT_FILENO, str, line_len + SPACES_COUNT);
}

void process_s_flag_on_line(const char* line, size_t line_len) {
    if (!line) return;

    static int n = 0;
    if (is_fully_empty_line(line) && n == 0) {
        n++;
        write(STDOUT_FILENO, "\n", 1);
    } else if (!is_fully_empty_line(line)) {
        n = 0;
        write(STDOUT_FILENO, line, line_len);
    }
}

void process_T_flag_on_line(const char* line, size_t line_len) {
    if (!line) return;

    for (size_t i = 0; i < line_len; ++i) {
        if (is_tab(line[i]))
            write(STDOUT_FILENO, "^I", 2);
        else
            write(STDOUT_FILENO, &line[i], 1);
    }
}

int is_tab(char ch) { return ch == '\t'; }

bool process_v_flag_on_line(char** line_ptr, size_t* line_len_ptr) {
    if (!*line_ptr) return false;

    bool has_error = false;
    char* ptr = (char*)allocate_with_memset(*line_len_ptr * 4 + 1);
    if (ptr) {
        char* new_line = ptr;
        for (size_t i = 0; i < *line_len_ptr; ++i) {
            unsigned char c = (*line_ptr)[i];
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
                strcat_formated_char_as_str(new_line, "M-^%c", c - 192);
        }

        *line_len_ptr = strlen(new_line);
        free(*line_ptr);
        *line_ptr = new_line;
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