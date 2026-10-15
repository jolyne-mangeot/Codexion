
#include "codexion.h"

static void	print_end_message(int output)
{
	if (output == 0)
		printf("The simulation was a success\n");
	else if (output == 1)
		printf("This program requires exactly 8 arguments.\n");
	else if (output == 2)
		printf("There was an error during parsing of arguments.\n");
	else if (output == 3)
		printf("The instantiation of structures and memory allocations %s",
			"was unsuccessful.\n");
	else if (output == 4)
			printf("There was an error during the lauching of threads.\n");
}

int	main(int argc, char *argv[])
{
	t_parsed	parsed;
	int			output;

	if (argc != 9)
		output = 1;
	else
	{
		parsed = parsing(&argv[1]);
		if (parsed.success == true)
			output = program_start(parsed);
		else
			output = 2;
	}
	print_end_message(output);
	return (output);
}
