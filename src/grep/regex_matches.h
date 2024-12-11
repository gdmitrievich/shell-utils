#ifndef REGEX_MATCHES
#define REGEX_MATCHES

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

void init_rm(regex_matches* rm);
void free_rm(regex_matches rm);
void free_mf(matched_file* file);
void free_ml(matched_line* line);

#endif  // REGEX_MATCHES