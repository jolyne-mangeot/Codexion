
#include "../codexion.h"

void	jumpstart_thread(pthread_mutex_t *start_mutex,
			pthread_cond_t *start_cond)
{
	pthread_mutex_lock(start_mutex);
	pthread_cond_wait(start_cond, start_mutex);
	pthread_mutex_unlock(start_mutex);
}

void	update_timeval(struct timeval *time, pthread_mutex_t struct_mutex)
{
	pthread_mutex_lock(&struct_mutex);
	gettimeofday(time, NULL);
	pthread_mutex_unlock(&struct_mutex);
}

t_program_state	check_prog_state(t_program_info *prog_info)
{
	t_program_state	output;

	pthread_mutex_lock(&prog_info->state_mutex);
	output = prog_info->state;
	pthread_mutex_unlock(&prog_info->state_mutex);
	return (output);
}

void	set_prog_state(t_program_info *prog_info, t_program_state state)
{
	pthread_mutex_lock(&prog_info->state_mutex);
	prog_info->state = state;
	pthread_mutex_unlock(&prog_info->state_mutex);
}

int	milliseconds(struct timeval start)
{
	struct timeval	now;
	int				seconds;
	int				nanoseconds;

	gettimeofday(&now, NULL);
	seconds = (int)(now.tv_sec - start.tv_sec) * 1000;
	nanoseconds = (int)(now.tv_usec - start.tv_usec) / 1000;
	return (seconds + nanoseconds);
}
