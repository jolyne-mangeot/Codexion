
#include "../codexion.h"

void	display_coder_state(t_coder *coder)
{
	char	*message;

	if (coder->state == WAITING)
		return ;
	if (coder->state == COMPILING)
		message = "\033[1m\033[38;5;160mis compiling\033[0m";
	else if (coder->state == DEBUGGING)
		message = "\tis debugging";
	else if (coder->state == REFACTORING)
		message = "\t\tis refactoring";
	else if (coder->state == DONE)
		message = "\033[1mis done with their compilations\033[0m";
	else if (coder->state == BURNOUT)
		message = "burnt out";
	pthread_mutex_lock(&coder->prog_info->print_mutex);
	printf("\t\033[3m%ims\033[0m\tCoder \033[1m%i\033[0m %s\n",
		milliseconds(coder->prog_info->program_start), coder->index, message);
	pthread_mutex_unlock(&coder->prog_info->print_mutex);
}

void	display_dongle_state(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&coder->prog_info->print_mutex);
	printf("\t\033[3m%ims\033[0m\tCoder \033[1m%i\033[0m\t"
		"\033[3mhas taken dongle %i\033[0m\n",
		milliseconds(coder->prog_info->program_start),
		coder->index, dongle->index);
	pthread_mutex_unlock(&coder->prog_info->print_mutex);
}
