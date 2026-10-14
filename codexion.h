
#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <time.h>

typedef enum e_program_state
{
	RUNNING,
	SUCCESS,
	PROG_BURNOUT,
	ERROR
}	t_program_state;

typedef enum e_dongle_state
{
	FREE,
	TAKEN,
	COOLDOWN
}	t_dongle_state;

typedef enum e_coder_state
{
	WAITING,
	COMPILING,
	DEBUGGING,
	REFACTORING,
	DONE,
	BURNOUT
}	t_coder_state;

typedef enum e_schedule_mode
{
	EDF,
	FIFO
}	t_schedule_mode;

typedef struct s_parsed
{
	bool			success;
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	t_schedule_mode	scheduler;
}	t_parsed;

typedef struct s_coder_info
{
	int	number_of_coders;
	int	cooldowns[3]; // 0 compile, 1 debug, 2 refactor
	int	time_to_burnout;
	int	compiles;
}	t_coder_info;

typedef struct s_program_info
{
	t_program_state	state;
	pthread_mutex_t	state_mutex;

	pthread_cond_t	start_cond;
	pthread_mutex_t	print_mutex;
	struct timeval	program_start;
	t_coder_info	coders_info;
	int				dongles_cooldown;
	t_schedule_mode	scheduler;
}	t_program_info;

typedef struct s_dongle	t_dongle;

typedef struct s_coder
{
	t_coder_state	state;
	pthread_mutex_t	mutex; // only for state and last_compile
	t_program_info	*prog_info;
	int				index;

	t_dongle		*r_dongle;
	t_dongle		*l_dongle;

	struct timeval	last_compile;
	struct timeval	cd_start;
}	t_coder;

struct s_dongle
{
	t_dongle_state	state;
	pthread_mutex_t	mutex;
	int				index;

	t_coder			*r_coder;
	t_coder			*l_coder;
	int				queue[2];

	struct timeval	cd_start;
};

typedef struct s_containers
{
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		*t_coders;
	pthread_t		monitor;
}	t_containers;

typedef struct s_monitor
{
	t_program_info	*prog_info;
	t_containers	*containers;
}	t_monitor;

t_parsed		parsing(char *argv[8]);

int				program_start(t_parsed parsed);
void			instantiate_program(t_parsed parsed, t_program_info *prog_info,
					t_containers *containers, t_monitor *monitor);

void			*monitor_thread(void *data);
void			*coder_thread(void *data);

void			jumpstart_thread(pthread_mutex_t *start_mutex,
					pthread_cond_t *start_cond);
void			update_timeval(struct timeval *time,
					pthread_mutex_t *struct_mutex);
t_program_state	check_prog_state(t_program_info *prog_info);
void			set_prog_state(
					t_program_info *prog_info, t_program_state state);
int				milliseconds(struct timeval start);

void			display_success(pthread_mutex_t *mutex);
void			display_coder_state(t_coder *coder);
void			display_dongle_state(t_coder *coder, t_dongle *dongle);

#endif
