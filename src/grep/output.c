#include "output.h"

#include <stdio.h>

void output(const matched_line* m_lines, const cmd_args_data cmd) {
    size_t s = get_matched_lines_count(m_lines);
    for (size_t i = 0; i < s; ++i) {
        printf("%s:%d: %s\n", m_lines[i].file_name, m_lines[i].line_number, m_lines[i].line);
    }
    if (cmd.flags.s == 0) {
    }
}