
#include "../codexion.h"

bool	is_digit(char *string)
{
	bool	digit;

	digit = true;
	while (*string)
	{
		if (*string < 48 || *string > 57)
			digit = false;
		string++;
	}
	return (digit);
}

void	create_parsed(t_parsed *parsed, char *argv[8])
{
	parsed->number_of_coders = atoi(argv[0]);
	parsed->time_to_burnout = atoi(argv[1]);
	parsed->time_to_compile = atoi(argv[2]);
	parsed->time_to_debug = atoi(argv[3]);
	parsed->time_to_refactor = atoi(argv[4]);
	parsed->number_of_compiles_required = atoi(argv[5]);
	parsed->dongle_cooldown = atoi(argv[6]);
	parsed->scheduler = strlen(argv[7]) - 3;
}

t_parsed	parsing(char *argv[8])
{
	t_parsed	parsed;
	int			index;

	index = 0;
	while (index < 7)
	{
		parsed.success = is_digit(argv[index]);
		if (parsed.success == false)
		{
			printf("Argument %i with value %s could not be parsed.\n",
				index, argv[index]);
			return (parsed);
		}
		index++;
	}
	parsed.success = false;
	if (strcmp("FIFO", argv[7]) && strcmp("EDF", argv[7]))
		return (parsed);
	parsed.success = true;
	create_parsed(&parsed, argv);
	return (parsed);
}
