#ifndef MATCHED_LINE_H
#define MATCHED_LINE_H

#include <stddef.h>

// Last item in the sequence has -1 value at the 'line_number' field.
typedef struct {
    char* file_name;
    int line_number;
    char* line;
} matched_line;

void init_matched_line(matched_line* ml);

void free_matched_lines(matched_line* m_lines);
void free_matched_line(matched_line* m_line);

void try_append_matched_line(matched_line** m_lines, const matched_line* src_m_line);
size_t get_matched_lines_count(const matched_line* m_lines);
void copy_matched_line(matched_line* dest, const matched_line* src);

size_t get_count_of_files_with_at_least_one_matched_line(const matched_line* m_lines);
int is_last(const matched_line* ml);

const char* get_next_file_name(const char* file_name, const matched_line* m_lines);
size_t find_idx_of_last_matched_line_with_file_name(const char* file_name, const matched_line* m_lines);
size_t find_idx_of_first_matched_line_with_file_name(const char* file_name, const matched_line* m_lines);

size_t get_matched_lines_count_on_file(const char* file_name, const matched_line* m_lines);

#endif  // MATCHED_LINE_H