#include "cmd_args_reader.h"

#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct option option;

#include "cmd_args_data.h"

bool set_retrieved_cmd_arg_data(cmd_args_data* cad, int argc, char** argv) {
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
    while (status && (flag = getopt_long(argc, argv, ":e:ivclnhsf:o", long_options, NULL)) != -1) {
        switch (flag) {
            case 'e':
                cad->flags.e = 1;
                status = append_str(&cad->pattern, optarg);
                break;
            case 'i':
                cad->flags.i = 1;
                break;
            case 'v':
                cad->flags.v = 1;
                break;
            case 'c':
                cad->flags.c = 1;
                break;
            case 'l':
                cad->flags.l = 1;
                break;
            case 'n':
                cad->flags.n = 1;
                break;
            case 'h':
                cad->flags.h = 1;
                break;
            case 's':
                cad->flags.s = 1;
                break;
            case 'f':
                cad->flags.f = 1;
                status = append_str(&cad->pattern_file, optarg);
                break;
            case 'o':
                cad->flags.o = 1;
                break;
            case ':':
                print_error("grep", "You need to specify an argument");
                status = false;
                break;
            case '?':
            default:
                print_error("grep", "Invalid option");
                status = false;
                break;
        }
    }

    if (status && optind == argc) {
        print_error("grep", "You should specify at least one file");
        status = false;
    } else if (status) {
        int n_files = argc - optind + 1;
		char** ptr = (char**)allocate_with_memset(sizeof(char*) * n_files + 1);
		if (ptr) {
			cad->search_files = ptr;
			for (int i = optind, j = 0; status && i < argc; ++i, ++j)
				status = append_str(&(cad->search_files[j]), argv[i]);
			if (status) cad->search_files[n_files] = NULL;
		} else {
			status = false;
		}
    }

    return status;
}