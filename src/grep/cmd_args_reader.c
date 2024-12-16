#include "cmd_args_reader.h"

#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct option option;

#include "cmd_args_data.h"
#include "grep_common.h"

bool set_retrieved_cmd_arg_data(cmd_args_data* cad, int argc, char** argv) {
    bool status = set_cmd_arg_flags(cad, argc, argv);
    if (status) status = set_cmd_arg_files(cad, argc, argv);
    return status;
}

bool set_cmd_arg_flags(cmd_args_data* cad, int argc, char** argv) {
    option long_options[] = {{"regexp", required_argument, 0, 'e'},
                             {"ignore-case", no_argument, 0, 'i'},
                             {"invert-match", no_argument, 0, 'v'},
                             {"count", no_argument, 0, 'c'},
                             {"files-with-matches", no_argument, 0, 'l'},
                             {"line-number", no_argument, 0, 'n'},
                             {"no-filename", no_argument, 0, 'h'},
                             {"no-messages", no_argument, 0, 's'},
                             {"file", required_argument, 0, 'f'},
                             {"only-matching", no_argument, 0, 'o'},
                             {0, 0, 0, 0}};
    int flag = 0;
    bool status = true;
    // ':' as the first char in optstring allow getopt to return ':' instead of '?' to indicate a missing
    // option argument.
    while (status && (flag = getopt_long(argc, argv, ":e:ivclnhsf:o", long_options, NULL)) != -1) {
        if (flag == 'e') {
            cad->flags.e = 1;
            if (!add_pattern_to_patterns_string(&cad->patterns, optarg)) status = false;
        } else if (flag == 'i') {
            cad->flags.i = 1;
        } else if (flag == 'v') {
            cad->flags.v = 1;
        } else if (flag == 'c') {
            cad->flags.c = 1;
        } else if (flag == 'l') {
            cad->flags.l = 1;
        } else if (flag == 'n') {
            cad->flags.n = 1;
        } else if (flag == 'h') {
            cad->flags.h = 1;
        } else if (flag == 's') {
            cad->flags.s = 1;
        } else if (flag == 'f') {
            cad->flags.f = 1;
            if (!add_str_to_str_arr_dynamically(&cad->pattern_files, optarg)) status = false;
        } else if (flag == 'o') {
            cad->flags.o = 1;
        } else if (flag == ':') {
            print_error("grep", "You need to specify an argument");
            status = false;
        } else if (flag == '?') {
            print_error("grep", "Invalid option");
            status = false;
        }
    }
	return status;
}

bool set_cmd_arg_files(cmd_args_data* cad, int argc, char** argv) {
	bool status = true;
    if (optind == argc) {
        print_error("Usage", "grep [OPTION]... PATTERNS [FILE]...");
        status = false;
    } else {
        if (!cad->flags.e && !cad->flags.f)
            if (!add_pattern_to_patterns_string(&cad->patterns, argv[optind++])) status = false;

        if (status && optind == argc) {
            print_error("grep", "You should specify at least one pattern");
            print_error("Usage", "grep [OPTION]... PATTERNS [FILE]...");
            status = false;
        }

        for (int i = optind; status && i < argc; ++i)
            if (!add_str_to_str_arr_dynamically(&cad->search_files, argv[i])) status = false;
    }
	return status;
}