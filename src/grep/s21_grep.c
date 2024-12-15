#include <stdio.h>

#include "cmd_args_data.h"
#include "cmd_args_reader.h"
#include "matched_line.h"
#include "output.h"
#include "search.h"

#include <errno.h>

int main(int argc, char** argv) {
    cmd_args_data cad;
	init_cad(&cad);
	bool status = set_retrieved_cmd_arg_data(&cad, argc, argv);
	if (status) {
		matched_line* m_lines = get_matched_lines(&cad);

		output(m_lines, &cad);

		free_matched_lines(m_lines);
	}
    free_cad(cad);
	if (!status && errno != 0) print_error("grep", NULL);
    return 0;
}
