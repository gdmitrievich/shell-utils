#ifndef REGEX_MATCHES_H
#define REGEX_MATCHES_H

#include <stddef.h>

typedef struct {
    int number;
    char* content;
} matched_line;

typedef struct {
    char* name;
    matched_line* matched_lines;
} matched_file;

typedef struct {
    matched_file* matched_files;
} regex_matches;

void init_ml(matched_line* ml);
void init_mf(matched_file* mf);

void init_rm(regex_matches* rm);
void free_rm(regex_matches rm);
void free_mf(matched_file file);
void free_ml(matched_line line);

void try_append_to_matched_lines(matched_line** m_lines, const matched_line* ml);
// m_lines - pointer to the first item of matched_line array.
// Searchs until the last item found. Last item - it's an item with the -1 value in number field.
size_t get_matched_lines_count(const matched_line* m_lines);
void copy_matched_line(matched_line* dest, const matched_line* src);

void try_append_to_matched_files(matched_file** m_files, const matched_file* mf);
// m_files - pointer to the first item of matched_file array.
// Searchs until the last item found. Last item - it's an item with the NULL value in matched_lines field.
size_t get_matched_files_count(const matched_file* m_files);
void copy_matched_file(matched_file* dest, const matched_file* src);

#endif  // REGEX_MATCHES_H