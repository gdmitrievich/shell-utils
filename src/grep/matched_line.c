#include "matched_line.h"

#include <stdlib.h>
#include <string.h>

#include "../common/common.h"

void init_matched_line(matched_line* ml) {
    ml->file_name = NULL;
    ml->line_number = 0;
    ml->line = NULL;
}

bool fill_matched_line(matched_line* ml_ptr, size_t line_number, const char* file_name, const char* line) {
    init_matched_line(ml_ptr);
    bool status = true;
    ml_ptr->line_number = line_number;
    if (!append_str(&ml_ptr->file_name, file_name)) status = false;
    if (status && !append_str(&ml_ptr->line, line)) status = false;

    return status;
}

void free_matched_lines(matched_line* m_lines) {
    if (!m_lines) return;

    for (int i = 0; !is_last(&m_lines[i]); ++i) free_matched_line(&m_lines[i]);

    free(m_lines);
}

void free_matched_line(matched_line* m_line) {
    if (!m_line) return;

    free(m_line->file_name);
    free(m_line->line);
}

bool append_matched_line(matched_line** m_lines, const matched_line* src_m_line) {
    if (!src_m_line) return false;

    bool status = true;
    matched_line* p = NULL;
    size_t next_idx = 0;
    if (*m_lines) {
        size_t n = get_matched_lines_count(*m_lines);
        p = (matched_line*)realloc(*m_lines, sizeof(matched_line) * (n + 2));
        next_idx = n;
    } else {
        p = allocate_with_memset(sizeof(matched_line) * 2);
        next_idx = 0;
    }

    if (p) {
        *m_lines = p;
        init_matched_line(*m_lines + next_idx);
        init_matched_line(*m_lines + next_idx + 1);
        status = copy_matched_line(*m_lines + next_idx, src_m_line);
        if (status)
            status = copy_matched_line(*m_lines + next_idx + 1,
                                       &(matched_line){.file_name = NULL, .line = NULL, .line_number = -1});
    } else {
        status = false;
    }

    return status;
}

size_t get_matched_lines_count(const matched_line* m_lines) {
    size_t s = 0;
    while (!is_last(&m_lines[s])) ++s;
    return s;
}

bool copy_matched_line(matched_line* dest, const matched_line* src) {
    bool status = true;
    if (!append_str(&dest->file_name, src->file_name)) status = false;
    if (status && !append_str(&dest->line, src->line)) status = false;
    if (status) dest->line_number = src->line_number;
    return status;
}

int is_last(const matched_line* ml) { return ml->line_number == -1; }

size_t find_idx_of_last_matched_line_with_file_name(const char* file_name, const matched_line* m_lines) {
    if (!file_name) return __SIZE_MAX__;

    size_t first_idx = find_idx_of_first_matched_line_with_file_name(file_name, m_lines);
    size_t i = first_idx;
    if (first_idx != __SIZE_MAX__)
        while (!is_last(&m_lines[i]) && !strcmp(m_lines[first_idx].file_name, m_lines[i].file_name)) ++i;

    return first_idx == __SIZE_MAX__ ? __SIZE_MAX__ : i - 1;
}

size_t find_idx_of_first_matched_line_with_file_name(const char* file_name, const matched_line* m_lines) {
    if (!file_name) return __SIZE_MAX__;

    size_t idx = 0;
    while (!is_last(&m_lines[idx]) && strcmp(file_name, m_lines[idx].file_name)) ++idx;

    return is_last(&m_lines[idx]) ? __SIZE_MAX__ : idx;
}

size_t get_matched_lines_count_on_file(const char* file_name, const matched_line* m_lines) {
    if (!file_name || !m_lines) return __SIZE_MAX__;

    size_t first_idx = find_idx_of_first_matched_line_with_file_name(file_name, m_lines);
    size_t last_idx = find_idx_of_last_matched_line_with_file_name(file_name, m_lines);
    size_t n = 1;
    if (first_idx != __SIZE_MAX__) {
        for (size_t compared = first_idx, i = first_idx + 1; i <= last_idx; ++i) {
            if (m_lines[compared].line_number != m_lines[i].line_number) {
                compared = i;
                ++n;
            }
        }
    }

    return first_idx == __SIZE_MAX__ ? __SIZE_MAX__ : n;
}