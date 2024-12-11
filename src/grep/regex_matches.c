#include "regex_matches.h"

#include <stdlib.h>
#include <string.h>

#include "../common/common.h"

void init_ml(matched_line* ml) {
    ml->content = NULL;
    ml->number = 0;
}

void init_mf(matched_file* mf) {
    mf->matched_lines = NULL;
    mf->name = NULL;
}

void init_rm(regex_matches* rm) { rm->matched_files = NULL; }

void free_rm(regex_matches rm) {
    if (rm.matched_files == NULL) return;

    for (int i = 0; &rm.matched_files[i] != NULL; i++) free_mf(rm.matched_files[i]);

    free(rm.matched_files);
}

void free_mf(matched_file file) {
    free(file.name);

    if (file.matched_lines != NULL) {
        for (int i = 0; file.matched_lines[i].number != -1; i++) {
            free_ml(file.matched_lines[i]);
        }
        free(file.matched_lines);
    }
}

void free_ml(matched_line line) { free(line.content); }

void try_append_to_matched_lines(matched_line** mlines, const matched_line* ml) {
    if (!ml) return;

    if (*mlines) {
        size_t n = get_matched_lines_count(*mlines);
        *mlines = try_reallocate_memory("grep", *mlines, sizeof(matched_line) * (n + 1),
                                        sizeof(matched_line) * (n + 2));
        copy_matched_line((*mlines) + n, ml);
        copy_matched_line((*mlines) + n + 1, &(matched_line){.content = NULL, .number = -1});
    } else {
        *mlines = try_allocate_memory("grep", sizeof(matched_line) * 2);
        copy_matched_line((*mlines), ml);
        copy_matched_line((*mlines) + 1, &(matched_line){.content = NULL, .number = -1});
    }
}

size_t get_matched_lines_count(const matched_line* m_lines) {
    size_t s = 0;
    while (m_lines[s].number != -1) {
        ++s;
    }
    return s;
}

void copy_matched_line(matched_line* dest, const matched_line* src) {
    dest->number = src->number;
    try_append_str(&dest->content, src->content);
}

void try_append_to_matched_files(matched_file** m_files, const matched_file* mf) {
    if (!mf) return;

    if (*m_files) {
        size_t n = get_matched_files_count(*m_files);
        *m_files =
            try_reallocate_memory("grep", *m_files, sizeof(matched_file) * n, sizeof(matched_file) * (n + 1));
        copy_matched_file(&(*m_files)[n - 1], mf);
        copy_matched_file(&(*m_files)[n], &(matched_file){.name = NULL, .matched_lines = NULL});
    } else {
        *m_files = try_allocate_memory("grep", sizeof(matched_file) * 2);
        copy_matched_file(&(*m_files)[0], mf);
    }
}

size_t get_matched_files_count(const matched_file* m_files) {
    size_t s = 0;
    while (m_files[s].matched_lines != NULL) ++s;
    return s;
}

void copy_matched_file(matched_file* dest, const matched_file* src) {
    try_append_str(&(dest->name), src->name);
    size_t s = get_matched_lines_count(src->matched_lines);
    for (size_t i = 0; i < s; ++i) {
        try_append_to_matched_lines(&(dest->matched_lines), src->matched_lines);
    }
}