
#include "../codexion.h"

void	instantiate_coders(t_parsed parsed,
			t_program_info *prog_info, t_containers *containers)
{
	int		index;
	t_coder	coder;

	index = 0;
	while (index < parsed.number_of_coders)
	{
		coder = containers->coders[index];
		coder.index = index + 1;
		coder.state = WAITING;
		coder.prog_info = prog_info;
		if (index == 0)
			coder.r_dongle = &containers->dongles[parsed.number_of_coders - 1];
		else
			coder.r_dongle = &containers->dongles[index - 1];
		coder.l_dongle = &containers->dongles[index];
		coder.current_cooldown = 0;
	}
}

int	instantiate_dongles(t_parsed parsed, t_containers *containers)
{
	int			index;
	t_dongle	dongle;

	index = 0;
	while (index < parsed.number_of_coders)
	{
		dongle = containers->dongles[index];
		dongle.index = index + 1;
		dongle.state = FREE;
		dongle.r_coder = &containers->coders[index];
		if (index == parsed.number_of_coders - 1)
			dongle.l_coder = &containers->coders[0];
		else
			dongle.l_coder = &containers->coders[index + 1];
		dongle.queue[0] = NULL;
		dongle.queue[1] = NULL;
		dongle.cooldown = parsed.dongle_cooldown;
		if (pthread_mutex_init(&dongle.mutex, NULL))
			return (1);
	}
	return (0);
}

void	instantiate_coders_info(t_parsed parsed, t_coder_info *coders_info)
{
	coders_info->number_of_coders = parsed.number_of_coders;
	coders_info->cooldowns[0] = parsed.time_to_compile;
	coders_info->cooldowns[1] = parsed.time_to_debug;
	coders_info->cooldowns[2] = parsed.time_to_refactor;
	coders_info->time_to_burnout = parsed.time_to_burnout;
	coders_info->compiles = parsed.number_of_compiles_required;
}

void	instantiate_program(t_parsed parsed,
			t_program_info *prog_info, t_containers *containers)
{
	prog_info->state = ERROR;
	if (pthread_mutex_init(&prog_info->print_mutex, NULL))
		return ;
	instantiate_coders_info(parsed, &prog_info->coders_info);
	prog_info->scheduler = parsed.scheduler;
	containers->coders = malloc(sizeof(t_coder) * parsed.number_of_coders);
	if (!containers->coders)
		return ;
	containers->dongles = malloc(sizeof(t_coder) * parsed.number_of_coders);
	if (!containers->dongles)
		return ;
	instantiate_coders(parsed, prog_info, containers);
	if (instantiate_dongles(parsed, containers))
		return ;
	prog_info->state = RUNNING;
}
