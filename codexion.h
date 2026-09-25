
#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <unistd.h>

typedef enum e_schedule_mode
{
	EDF,
	FIFO
}	t_schedule_mode;

typedef enum e_simulation_state
{
	RUNNING,
	SUCCESS,
	BURNOUT,
	ERROR
}	t_simulation_state;

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

t_parsed	parsing(char *argv[8]);

#endif
