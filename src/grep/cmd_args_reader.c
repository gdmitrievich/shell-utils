#include "cmd_args_reader.h"

#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct option option;

#include "../common/common.h"
#include "cmd_args_data.h"

cmd_args_data retrieve_cmd_arg_data(int argc, char** argv) {
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
    opterr = 0;
    cmd_args_data f;
    init(&f);
    while ((flag = getopt_long(argc, argv, ":e:ivclnhsf:o", long_options, NULL)) != -1) {
        switch (flag) {
            case 'e':
                f.flags.e = 1;
                strcpy(f.pattern, optarg);
                break;
            case 'i':
                f.flags.i = 1;
                break;
            case 'v':
                f.flags.v = 1;
                break;
            case 'c':
                f.flags.c = 1;
                break;
            case 'l':
                f.flags.l = 1;
                break;
            case 'n':
                f.flags.n = 1;
                break;
            case 'h':
                f.flags.h = 1;
                break;
            case 's':
                f.flags.s = 1;
                break;
            case 'f':
                f.flags.f = 1;
                strcpy(f.pattern_file, optarg);
                break;
            case 'o':
                f.flags.o = 1;
                break;
            case ':':
                print_error("grep", "You need to specify an argument");
                break;
            case '?':
            default:
                print_error("grep", "Invalid option");
                break;
        }
    }

    f.n_search_files = argc - optind;
    for (int i = optind; i < argc; ++i)
        strcat(f.search_files[i], argv[i]);

    return f;
}