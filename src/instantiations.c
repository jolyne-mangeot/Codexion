
#include "../codexion.h"

static int	instantiate_coders(t_parsed parsed,
			t_program_info *prog_info, t_containers *containers)
{
	int		index;
	t_coder	*coder;

	index = 0;
	while (index < parsed.number_of_coders)
	{
		coder = &containers->coders[index];
		coder->state = WAITING;
		if (pthread_mutex_init(&coder->mutex, NULL))
			return (1);
		coder->prog_info = prog_info;
		coder->index = index + 1;
		if (index == 0)
			coder->r_dongle = &containers->dongles[parsed.number_of_coders - 1];
		else
			coder->r_dongle = &containers->dongles[index - 1];
		coder->l_dongle = &containers->dongles[index];
		gettimeofday(&coder->last_compile, NULL);
		gettimeofday(&coder->cd_start, NULL);
		index++;
	}
	return (0);
}

static int	instantiate_dongles(t_parsed parsed, t_containers *containers)
{
	int			index;
	t_dongle	*dongle;

	index = 0;
	while (index < parsed.number_of_coders)
	{
		dongle = &containers->dongles[index];
		dongle->state = FREE;
		if (pthread_mutex_init(&dongle->mutex, NULL))
			return (1);
		dongle->index = index + 1;
		dongle->r_coder = &containers->coders[index];
		if (index == parsed.number_of_coders - 1)
			dongle->l_coder = &containers->coders[0];
		else
			dongle->l_coder = &containers->coders[index + 1];
		dongle->queue[0] = 0;
		dongle->queue[1] = 0;
		gettimeofday(&dongle->cd_start, NULL);
		index++;
	}
	return (0);
}

static void	instantiate_coders_info(t_parsed parsed, t_coder_info *coders_info)
{
	coders_info->number_of_coders = parsed.number_of_coders;
	coders_info->cooldowns[0] = parsed.time_to_compile;
	coders_info->cooldowns[1] = parsed.time_to_debug;
	coders_info->cooldowns[2] = parsed.time_to_refactor;
	coders_info->time_to_burnout = parsed.time_to_burnout;
	coders_info->compiles = parsed.number_of_compiles_required;
}

void	instantiate_program(t_parsed parsed, t_program_info *prog_info,
			t_containers *containers, t_monitor *monitor)
{
	prog_info->state = ERROR;
	if (pthread_mutex_init(&prog_info->state_mutex, NULL))
		return ;
	if (pthread_mutex_init(&prog_info->print_mutex, NULL))
		return ;
	instantiate_coders_info(parsed, &prog_info->coders_info);
	prog_info->dongles_cooldown = parsed.dongle_cooldown;
	prog_info->scheduler = parsed.scheduler;
	containers->coders = malloc(sizeof(t_coder) * parsed.number_of_coders);
	if (!containers->coders)
		return ;
	containers->dongles = malloc(sizeof(t_dongle) * parsed.number_of_coders);
	if (!containers->dongles)
		return ;
	containers->t_coders = malloc(sizeof(pthread_t) * parsed.number_of_coders);
	if (!containers->t_coders)
		return ;
	if (instantiate_coders(parsed, prog_info, containers))
		return ;
	if (instantiate_dongles(parsed, containers))
		return ;
	monitor->prog_info = prog_info;
	monitor->containers = containers;
	prog_info->state = RUNNING;
}
