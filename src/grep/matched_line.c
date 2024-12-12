#include "matched_line.h"

#include <stdlib.h>

#include "../common/common.h"

void free_matched_lines(matched_line* m_lines) {
    if (!m_lines) return;

    for (int i = 0; m_lines[i].line_number != -1; ++i) free_matched_line(&m_lines[i]);

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
    while (m_lines[s].line_number != -1) ++s;
    return s;
}

void copy_matched_line(matched_line* dest, const matched_line* src) {
    try_append_str(&dest->file_name, src->file_name);
    try_append_str(&dest->line, src->line);
    dest->line_number = src->line_number;
}