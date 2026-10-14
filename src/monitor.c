
#include "../codexion.h"

void	*monitor_thread(void *data)
{
	t_monitor	*self;

	self = (t_monitor *)data;
	jumpstart_thread(&self->prog_info->print_mutex,
		&self->prog_info->start_cond);
	return (data);
}
