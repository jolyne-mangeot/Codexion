
#include "../codexion.h"

static void	set_coder_state(t_coder *coder, t_coder_state state)
{
	pthread_mutex_lock(&coder->mutex);
	coder->state = state;
	pthread_mutex_unlock(&coder->mutex);
}

static void	start_coding(t_coder *coder)
{
	int	compilations;

	compilations = 0;
	while (compilations < coder->prog_info->coders_info.compiles)
	{
		if (check_prog_state(coder->prog_info) != RUNNING)
			return ;
		update_timeval(&coder->last_compile, &coder->mutex);
		usleep(10);
		compilations++;
	}
	set_coder_state(coder, DONE);
	if (check_prog_state(coder->prog_info) == RUNNING
		|| check_prog_state(coder->prog_info) == SUCCESS)
		display_coder_state(coder);
}

void	*coder_thread(void *data)
{
	t_coder	*self;

	self = (t_coder *)data;
	jumpstart_thread(&self->prog_info->print_mutex,
		&self->prog_info->start_cond);
	update_timeval(&self->last_compile, &self->mutex);
	start_coding(self);
	return (NULL);
}
