#include <stdio.h>

#include "cmd_args_data.h"
#include "cmd_args_reader.h"

int main(int argc, char** argv) {
    cmd_args_data cad = retrieve_cmd_arg_data(argc, argv);
    printf("e: %d, ", cad.flags.e);
    printf("i: %d, ", cad.flags.i);
    printf("v: %d, ", cad.flags.v);
    printf("c: %d, ", cad.flags.c);
    printf("l: %d, ", cad.flags.l);
    printf("n: %d, ", cad.flags.n);
    printf("h: %d, ", cad.flags.h);
    printf("s: %d, ", cad.flags.s);
    printf("f: %d, ", cad.flags.f);
    printf("o: %d\n", cad.flags.o);
    printf("pattern: %s\n", cad.pattern);
    printf("pattern_file: %s\n", cad.pattern_file);
    for (int i = 0; i < cad.n_search_files; ++i) printf("pattern: %s\n", cad.search_files[i]);
    return 0;
}
