
#include "../codexion.h"

void	display_coder_state(t_coder *coder)
{
	char	*message;

	if (coder->state == COMPILING)
		message = "is compiling";
	else if (coder->state == DEBUGGING)
		message = "is debugging";
	else if (coder->state == REFACTORING)
		message = "is refactoring";
	else if (coder->state == BURNOUT)
		message = "burnt out";
	printf("\t%ims\tCoder %i %s\n",
		milliseconds(coder->prog_info->program_start), coder->index, message);
}

void	display_dongle_state(t_coder *coder, t_dongle *dongle)
{
	printf("\t%ims\tCoder %i has taken dongle %i\n",
		milliseconds(coder->prog_info->program_start),
		coder->index, dongle->index);
}
