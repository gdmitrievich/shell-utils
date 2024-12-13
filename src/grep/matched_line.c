#include "matched_line.h"

#include <stdlib.h>
#include <string.h>

#include "../common/common.h"

void init_matched_line(matched_line* ml) {
    ml->file_name = NULL;
    ml->line_number = 0;
    ml->line = NULL;
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

void try_append_matched_line(matched_line** m_lines, const matched_line* src_m_line) {
    if (!src_m_line) return;

    if (*m_lines) {
        size_t n = get_matched_lines_count(*m_lines);
        *m_lines = try_reallocate_memory("grep", *m_lines, sizeof(matched_line) * (n + 2));
        init_matched_line(*m_lines + n);
        init_matched_line(*m_lines + n + 1);
        copy_matched_line(*m_lines + n, src_m_line);
        copy_matched_line(*m_lines + n + 1,
                          &(matched_line){.file_name = NULL, .line = NULL, .line_number = -1});
    } else {
        *m_lines = try_allocate_memory("grep", sizeof(matched_line) * 2);
        copy_matched_line(*m_lines, src_m_line);
        copy_matched_line(*m_lines + 1, &(matched_line){.file_name = NULL, .line = NULL, .line_number = -1});
    }
}

size_t get_matched_lines_count(const matched_line* m_lines) {
    size_t s = 0;
    while (!is_last(&m_lines[s])) ++s;
    return s;
}

void copy_matched_line(matched_line* dest, const matched_line* src) {
    try_append_str(&dest->file_name, src->file_name);
    try_append_str(&dest->line, src->line);
    dest->line_number = src->line_number;
}

size_t get_count_of_files_with_at_least_one_matched_line(const matched_line* m_lines) {
    if (!m_lines) return __SIZE_MAX__;

    size_t n = 0;
    const char* prev_file_name = NULL;
    for (size_t i = 0; !is_last(&m_lines[i]); ++i) {
        if (!prev_file_name || strcmp(prev_file_name, m_lines[i].file_name)) {
            n++;
            prev_file_name = m_lines[i].file_name;
        }
    }

    return n;
}

int is_last(const matched_line* ml) { return ml->line_number == -1; }

const char* get_next_file_name(const char* file_name, const matched_line* m_lines) {
    if (!file_name) return m_lines[0].file_name;

    size_t idx = find_idx_of_last_matched_line_with_file_name(file_name, m_lines);
    return idx == __SIZE_MAX__ || is_last(&m_lines[idx + 1]) ? NULL : m_lines[idx + 1].file_name;
}

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