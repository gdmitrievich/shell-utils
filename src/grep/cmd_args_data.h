#ifndef CMD_ARGS_DATA
#define CMD_ARGS_DATA

typedef struct {
    int e;  // Has arg.
    int i;
    int v;
    int c;
    int l;
    int n;
    int h;
    int s;
    int f;  // Has arg.
    int o;
} flags;

typedef struct {
    flags flags;

    char* pattern;  // Both -e and -f file.
    char* pattern_file;

    char** argv;
    int n_search_files;
} cmd_args_data;

void init_cad(cmd_args_data* cad);
void free_cad(cmd_args_data cad);

#endif  // CMD_ARGS_DATA