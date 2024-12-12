#include "matched_line.h"

#include <stdlib.h>

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