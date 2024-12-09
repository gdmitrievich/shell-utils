#include "cmd_args_reader.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

typedef struct option option;

#include "flags.h"

flags get_cmd_arg_flags(int argc, char** argv) {
    option long_options[] = {{"number-nonblank", no_argument, NULL, 'b'},
                             {"number", no_argument, NULL, 'n'},
                             {"squeeze-blank", no_argument, NULL, 's'},
                             {0, 0, 0, 0}};

    int flag = 0;
    int longind = 0;
    flags f;
    init_flags(&f);
    while ((flag = getopt_long(argc, argv, "beEnstT", long_options, &longind)) != -1) {
        switch (flag) {
            case 'b':
                f.b = 1;
                break;
            case 'e':
                f.E = 1;
                f.v = 1;
                break;
            case 'E':
                f.E = 1;
                break;
            case 'n':
                f.n = 1;
                break;
            case 's':
                f.s = 1;
                break;
            case 't':
                f.T = 1;
                f.v = 1;
                break;
            case 'T':
                f.T = 1;
                break;
            case '?':
            default:
                printf("ERROR: Invalid option!\n");
                exit(1);
                break;
        }
    }

    return f;
}

int get_idx_of_first_filepath(int argc, char** argv) {
    while (getopt(argc, argv, "beEnstT") != -1) {}
    if (argc == 1 || optind == argc) {
		perror("cat: You should specify at least one filepath!");
        exit(EXIT_FAILURE);
    }

    return optind;
}