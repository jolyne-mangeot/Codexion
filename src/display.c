
#include "../codexion.h"

void	display_coder_state(t_coder *coder)
{
	char	*message;

	if (coder->state == WAITING)
		return ;
	if (coder->state == COMPILING)
		message = "is compiling";
	else if (coder->state == DEBUGGING)
		message = "is debugging";
	else if (coder->state == REFACTORING)
		message = "is refactoring";
	else if (coder->state == DONE)
		message = "is done with their compilations";
	else if (coder->state == BURNOUT)
		message = "burnt out";
	pthread_mutex_lock(&coder->prog_info->print_mutex);
	printf("\t%ims\tCoder %i %s\n",
		milliseconds(coder->prog_info->program_start), coder->index, message);
	pthread_mutex_unlock(&coder->prog_info->print_mutex);
}

void	display_dongle_state(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&coder->prog_info->print_mutex);
	printf("\t%ims\tCoder %i has taken dongle %i\n",
		milliseconds(coder->prog_info->program_start),
		coder->index, dongle->index);
	pthread_mutex_unlock(&coder->prog_info->print_mutex);
}
