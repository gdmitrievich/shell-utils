#ifndef CMD_ARGS_DATA
#define CMD_ARGS_DATA

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} flags;

typedef struct {
  flags flags;

  char* patterns;  // The first non option arg, if there are no -e or -f
                   // options, should be considered as pattern.
  char** pattern_files;

  char** search_files;
} cmd_args_data;

void init_cad(cmd_args_data* cad);
void free_cad(cmd_args_data cad);
void free_arr_of_strings(char** str_arr);

#endif  // CMD_ARGS_DATA