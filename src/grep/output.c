#include "output.h"

#include <stdio.h>

void output(const matched_line* m_lines, const cmd_args_data* cmd) {
	if (!m_lines || !cmd) return;

	size_t n_files = get_count_of_files_with_at_least_one_matched_line(m_lines);
    const flags* f = &cmd->flags;
    if (n_files > 0 && f->l) {
        print_file_names_with_at_least_one_matched_line();
    } else if (n_files > 0) {
        if (f->c) {
			char* file = NULL;
			while ((file = get_next_file(file)) != NULL) {
                printf("%s:", file);
                printf("%d\n", get_matched_lines_count_on_file(file));
			}
        } else {
            size_t n_lines = get_matched_lines_count(m_lines);
            for (size_t i = 0; i < n_lines; ++i) {
                if (n_files > 1 && !f->h) {
                    printf("%s:", m_lines[0].file_name);
                }
                if (f->n) {
                    printf("%d:", m_lines[0].line_number);
                }
                printf("%s\n", m_lines[0].line);
            }
        }
    }
}