
#include "../codexion.h"

static void	set_coder_state(t_coder *coder, t_coder_state state)
{
	pthread_mutex_lock(&coder->mutex);
	coder->state = state;
	display_coder_state(coder);
	pthread_mutex_unlock(&coder->mutex);
}

static int	pass_state(t_coder *coder)
{
	int	remaining_cd;

	pthread_mutex_lock(&coder->mutex);
	remaining_cd = coder->prog_info->coders_info.cooldowns[coder->state - 1];
	pthread_mutex_unlock(&coder->mutex);
	while (remaining_cd > 0)
	{
		if (remaining_cd > 100)
			usleep(100000);
		else
			usleep(remaining_cd * 1000);
		remaining_cd -= 100;
		if (check_prog_state(coder->prog_info) != RUNNING)
			return (1);
	}
	return (0);
}

static int	compile(t_coder *coder)
{
	register_in_queues(coder);
	while (!check_dongles_priority(coder))
	{
		usleep(1);
		if (check_prog_state(coder->prog_info) != RUNNING)
			return (1);
	}
	set_coder_state(coder, COMPILING);
	return (0);
}

static void	start_coding(t_coder *coder)
{
	int	compilations;

	compilations = 0;
	while (compilations < coder->prog_info->coders_info.compiles)
	{
		if (check_prog_state(coder->prog_info) != RUNNING)
			return ;
		set_coder_state(coder, WAITING);
		if (compile(coder))
			return ;
		if (pass_state(coder))
			return ;
		compilations++;
		put_down_dongles(coder);
		update_timeval(&coder->last_compile, &coder->mutex);
		set_coder_state(coder, DEBUGGING);
		if (pass_state(coder))
			return ;
		set_coder_state(coder, REFACTORING);
		if (pass_state(coder))
			return ;
	}
	set_coder_state(coder, DONE);
}

void	*coder_thread(void *data)
{
	t_coder	*coder;

	coder = (t_coder *)data;
	jumpstart_thread(&coder->prog_info->print_mutex,
		&coder->prog_info->start_cond);
	update_timeval(&coder->last_compile, &coder->mutex);
	set_coder_state(coder, WAITING);
	start_coding(coder);
	return (NULL);
}
