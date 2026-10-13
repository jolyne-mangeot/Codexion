
#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

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
	pthread_mutex_t	print_mutex;
	struct timeval	program_start;
	t_coder_info	coders_info;
	t_schedule_mode	scheduler;
}	t_program_info;

typedef struct s_dongle	t_dongle;

typedef struct s_coder
{
	t_coder_state	state;
	t_program_info	*prog_info;
	int				index;

	t_dongle		*l_dongle;
	t_dongle		*r_dongle;

	int				current_cooldown;
}	t_coder;

struct s_dongle
{
	t_dongle_state	state;
	int				index;

	t_coder			*l_coder;
	t_coder			*r_coder;
	t_coder			*queue[2];
	pthread_mutex_t	mutex;

	int				cooldown;
};

typedef struct s_containers
{
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		*t_coders;
	pthread_t		*t_dongles;
	pthread_t		*monitor;
}	t_containers;

t_parsed	parsing(char *argv[8]);

int			program_start(t_parsed parsed);
void		instantiate_program(t_parsed parsed,
				t_program_info *prog_info, t_containers *containers);

#endif
