
#include "codexion.h"

int	main(int argc, char *argv[])
{
	t_parsed	parsed;
	int			output;

	if (argc != 9)
	{
		write(1, "This program requires exactly 8 arguments.\n", 44);
		output = 1;
	}
	else
	{
		parsed = parsing(&argv[1]);
		if (parsed.success == true)
			output = program_start(parsed);
		else
			output = 2;
	}
	return (output);
}
