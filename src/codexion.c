
#include "../codexion.h"

void	join_threads(t_program_info prog_info, t_containers containers)
{
	int	index;

	index = 0;
	if (containers.t_coders)
	{
		while (index < prog_info.coders_info.number_of_coders)
			pthread_join(containers.t_coders[index++], NULL);
	}
	pthread_join(containers.monitor, NULL);
}

void	free_mallocs(t_program_info prog_info, t_containers containers)
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
	if (containers.t_coders)
		free(containers.t_coders);
}

int	program_end(int error, t_program_info prog_info, t_containers containers)
{
	pthread_cond_broadcast(&prog_info.start_cond);
	join_threads(prog_info, containers);
	free_mallocs(prog_info, containers);
	return (error);
}

int	start_threads(t_program_info *prog_info,
		t_containers *containers, t_monitor *monitor)
{
	int	index;

	index = 0;
	while (index < prog_info->coders_info.number_of_coders)
	{
		if (pthread_create(&containers->t_coders[index],
				NULL, coder_thread, &containers->coders[index]))
			return (4);
		index++;
	}
	if (pthread_create(&containers->monitor, NULL, monitor_thread, monitor))
		return (5);
	usleep(50000);
	pthread_cond_broadcast(&prog_info->start_cond);
	return (0);
}

int	program_start(t_parsed parsed)
{
	t_program_info	prog_info;
	t_containers	containers;
	t_monitor		monitor;

	instantiate_program(parsed, &prog_info, &containers, &monitor);
	if (prog_info.state == ERROR)
		return (program_end(3, prog_info, containers));
	gettimeofday(&prog_info.program_start, NULL);
	pthread_cond_init(&prog_info.start_cond, NULL);
	if (start_threads(&prog_info, &containers, &monitor))
		return (program_end(4, prog_info, containers));
	return (program_end(0, prog_info, containers));
}
