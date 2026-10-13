
#include "../codexion.h"

int	program_start(t_parsed parsed)
{
	t_containers	containers;
	t_program_info	prog_info;

	instantiate_program(parsed, &prog_info, &containers);
	gettimeofday(&prog_info.program_start, NULL);
}
