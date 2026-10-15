
#include "../codexion.h"

static int	update_dongles_queues(t_schedule_mode mode, t_dongle *dongle)
{
	t_coder	*temp;

	if (dongle->queue[0] == NULL)
	{
		dongle->queue[0] = dongle->queue[1];
		dongle->queue[1] = NULL;
	}
	else if (dongle->queue[1] != NULL && mode == EDF)
	{
		pthread_mutex_lock(&dongle->queue[0]->mutex);
		pthread_mutex_lock(&dongle->queue[1]->mutex);
		if (milliseconds(dongle->queue[0]->last_compile)
			< milliseconds(dongle->queue[1]->last_compile))
		{
			temp = dongle->queue[0];
			dongle->queue[0] = dongle->queue[1];
			dongle->queue[1] = temp;
		}
		pthread_mutex_unlock(&dongle->queue[1]->mutex);
		pthread_mutex_unlock(&dongle->queue[0]->mutex);
	}
	if (dongle->state != FREE)
		return (0);
	return (dongle->queue[0]->index);
}

void	put_down_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->r_dongle->mutex);
	pthread_mutex_lock(&coder->l_dongle->mutex);
	coder->r_dongle->state = COOLDOWN;
	coder->l_dongle->state = COOLDOWN;
	gettimeofday(&coder->r_dongle->cd_start, NULL);
	gettimeofday(&coder->l_dongle->cd_start, NULL);
	pthread_mutex_unlock(&coder->l_dongle->mutex);
	pthread_mutex_unlock(&coder->r_dongle->mutex);
}

bool	check_dongles_priority(t_coder *coder)
{
	bool	dongles_free;

	dongles_free = true;
	pthread_mutex_lock(&coder->r_dongle->mutex);
	pthread_mutex_lock(&coder->l_dongle->mutex);
	if (update_dongles_queues(coder->prog_info->scheduler, coder->r_dongle)
		!= coder->index)
		dongles_free = false;
	if (update_dongles_queues(coder->prog_info->scheduler, coder->l_dongle)
		!= coder->index)
		dongles_free = false;
	if (dongles_free)
	{
		display_dongle_state(coder, coder->r_dongle);
		coder->r_dongle->state = TAKEN;
		coder->r_dongle->queue[0] = NULL;
		display_dongle_state(coder, coder->l_dongle);
		coder->l_dongle->state = TAKEN;
		coder->l_dongle->queue[0] = NULL;
	}
	pthread_mutex_unlock(&coder->r_dongle->mutex);
	pthread_mutex_unlock(&coder->l_dongle->mutex);
	return (dongles_free);
}

static void	register_in_queue(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	if (dongle->queue[0] == NULL || dongle->queue[0] == coder)
		dongle->queue[0] = coder;
	else
		dongle->queue[1] = coder;
	pthread_mutex_unlock(&dongle->mutex);
}

void	register_in_queues(t_coder *coder)
{
	register_in_queue(coder, coder->l_dongle);
	register_in_queue(coder, coder->r_dongle);
}
