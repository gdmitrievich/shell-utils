#ifndef MATCHED_LINE_H
#define MATCHED_LINE_H

typedef struct {
    char* file_name;
    int line_number;
    char* line;
} matched_line;

void free_matched_lines(matched_line* m_lines);
void free_matched_line(matched_line* m_line);

#endif  // MATCHED_LINE_H