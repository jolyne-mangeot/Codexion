
#include "../codexion.h"

void	jumpstart_thread(pthread_mutex_t *start_mutex,
			pthread_cond_t *start_cond)
{
	pthread_mutex_lock(start_mutex);

	pthread_cond_wait(start_cond, start_mutex);
	pthread_mutex_unlock(start_mutex);
}
