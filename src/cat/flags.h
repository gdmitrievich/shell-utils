#ifndef FLAGS
#define FLAGS

#include "../common/common.h"

typedef struct flags {
  int b, E, n, s, T, v;
} flags;

void init_flags(flags* f);
bool is_at_least_one_flag_set(const flags* f);

#endif  // FLAGS