#include "flags.h"

void init_flags(flags* f) { *f = (flags){0, 0, 0, 0, 0, 0}; }

bool is_at_least_one_flag_set(const flags* f) {
	return f->b || f->E || f->n || f->s || f->T || f->v;
}