#include "regex_matches.h"

#include <string.h>

void init_regex_matches(regex_matches* rm) {
    for (size_t i = 0; i < sizeof(rm->matched_files); ++i) {
        for (size_t j = 0; j < sizeof(rm->matched_files[0].matched_lines); ++j) {
            matched_line* m_l = &rm->matched_files[i].matched_lines[j];
            char* l_content = m_l->content;
            size_t s = sizeof(l_content);
            memset(l_content, 0, s);
            m_l->number = 0;
        }

        char* f_name = rm->matched_files[i].name;
        size_t s = sizeof(f_name);
        memset(f_name, 0, s);
    }
}