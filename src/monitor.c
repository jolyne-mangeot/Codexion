
#include "../codexion.h"

static t_program_state	burn_out(t_coder *coder)
{
	coder->state = BURNOUT;
	display_coder_state(coder);
	pthread_mutex_unlock(&coder->mutex);
	return (PROG_BURNOUT);
}

static t_program_state	check_coders(t_monitor	*monitor)
{
	t_coder	*coder;
	int		burnout_cd;
	int		done_count;
	int		index;

	burnout_cd = monitor->prog_info->coders_info.time_to_burnout;
	done_count = 0;
	index = 0;
	while (index < monitor->prog_info->coders_info.number_of_coders)
	{
		coder = &monitor->containers->coders[index];
		pthread_mutex_lock(&coder->mutex);
		if (coder->state == DONE)
			done_count++;
		else if (coder->state != COMPILING
			&& milliseconds(coder->last_compile) >= burnout_cd)
			return (burn_out(coder));
		pthread_mutex_unlock(&coder->mutex);
		index++;
	}
	if (done_count == monitor->prog_info->coders_info.number_of_coders)
		return (SUCCESS);
	return (RUNNING);
}

static void	check_dongles(t_monitor	*monitor)
{
	t_dongle	*dongle;
	int			index;

	index = 0;
	while (index < monitor->prog_info->coders_info.number_of_coders)
	{
		dongle = &monitor->containers->dongles[index];
		pthread_mutex_lock(&dongle->mutex);
		if (dongle->state == COOLDOWN && milliseconds(dongle->cd_start)
			>= monitor->prog_info->dongles_cooldown)
			dongle->state = FREE;
		pthread_mutex_unlock(&dongle->mutex);
		index++;
	}
}

void	*monitor_thread(void *data)
{
	t_monitor	*self;

	self = (t_monitor *)data;
	jumpstart_thread(&self->prog_info->print_mutex,
		&self->prog_info->start_cond);
	usleep(100);
	while (check_prog_state(self->prog_info) == RUNNING)
	{
		check_dongles(self);
		set_prog_state(self->prog_info, check_coders(self));
	}
	return (NULL);
}
