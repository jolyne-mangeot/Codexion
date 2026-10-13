
#include "../codexion.h"

void	program_end(t_program_info prog_info, t_containers containers)
{
	int	index;

	pthread_mutex_destroy(&prog_info.print_mutex);
	if (containers.dongles)
	{
		index = 0;
		while (index < prog_info.coders_info.number_of_coders)
			pthread_mutex_destroy(&containers.dongles[index++].mutex);
		free(containers.dongles);
	}
	if (containers.coders)
		free(containers.coders);
}

int	program_start(t_parsed parsed)
{
	t_program_info	prog_info;
	t_containers	containers;

	instantiate_program(parsed, &prog_info, &containers);
	if (prog_info.state == ERROR)
	{
		program_end(prog_info, containers);
		return (3);
	}
	gettimeofday(&prog_info.program_start, NULL);
	program_end(prog_info, containers);
	return (0);
}
