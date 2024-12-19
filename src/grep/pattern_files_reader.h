#ifndef PATTERN_FILES_READER_H
#define PATTERN_FILES_READER_H

#include <stdio.h>

#include "../common/common.h"

bool set_regexes_retrieved_from_files(char** patterns_ptr,
                                      char** pattern_files);
bool set_regexes_retrieved_from_file(char** patterns_ptr, FILE* fp);

#endif  // PATTERN_FILES_READER_H