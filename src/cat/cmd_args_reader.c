#include "cmd_args_reader.h"

#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct option option;

#include "../common/common.h"
#include "flags.h"

bool set_cmd_arg_flags(flags* f, int argc, char** argv) {
    option long_options[] = {{"number-nonblank", no_argument, NULL, 'b'},
                             {"number", no_argument, NULL, 'n'},
                             {"squeeze-blank", no_argument, NULL, 's'},
                             {0, 0, 0, 0}};

	bool has_error = false;
    int flag = 0;
    int longind = 0;
    init_flags(f);
    while ((flag = getopt_long(argc, argv, "beEnstT", long_options, &longind)) != -1) {
        switch (flag) {
            case 'b':
                f->b = 1;
                break;
            case 'e':
                f->E = 1;
                f->v = 1;
                break;
            case 'E':
                f->E = 1;
                break;
            case 'n':
                f->n = 1;
                break;
            case 's':
                f->s = 1;
                break;
            case 't':
                f->T = 1;
                f->v = 1;
                break;
            case 'T':
                f->T = 1;
                break;
            case '?':
            default:
                print_error("cat", "Invalid option");
				has_error = true;
                break;
        }
    }

    return !has_error;
}

bool set_idx_of_first_filepath(int* idx, int argc, char** argv) {
    while (getopt(argc, argv, "beEnstT") != -1) {
    }
	bool has_error = false;
    if (argc == 1 || optind == argc)
	{
		print_error("cat", "You should specify at least one file");
		has_error = true;
	} else {
		*idx = optind;
	}
    return !has_error;
}