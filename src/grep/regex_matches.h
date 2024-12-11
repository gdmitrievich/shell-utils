#ifndef REGEX_MATCHES
#define REGEX_MATCHES

typedef struct {
    int number;
    char content[1024];
} matched_line;

typedef struct {
    char name[1024];
    matched_line matched_lines[1024];
} matched_file;

typedef struct {
    matched_file matched_files[1024];
} regex_matches;

void init_regex_matches(regex_matches* rm);

#endif  // REGEX_MATCHES