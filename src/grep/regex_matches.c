#include "regex_matches.h"

#include <stdlib.h>
#include <string.h>

void init_rm(regex_matches* rm) { rm->matched_files = NULL; }

void free_rm(regex_matches rm) {
    if (rm.matched_files == NULL) return;

    for (int i = 0; &rm.matched_files[i] != NULL; i++) free_mf(&rm.matched_files[i]);

    free(rm.matched_files);
}

void free_mf(matched_file* file) {
    if (file == NULL) return;

    free(file->name);

    if (file->matched_lines != NULL) {
        for (int i = 0; file->matched_lines[i].number != -1; i++) free_ml(&(file->matched_lines[i]));
        free(file->matched_lines);
    }

    free(file);
}

void free_ml(matched_line* line) {
    if (line == NULL) return;

    free(line->content);
    free(line);
}