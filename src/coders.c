
#include "../codexion.h"

void	*coder_thread(void *data)
{
	t_coder	*self;

	self = (t_coder *)data;
	jumpstart_thread(&self->prog_info->print_mutex,
		&self->prog_info->start_cond);
	return (NULL);
}
