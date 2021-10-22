#include <stdlib.h>

void lsh_loop();

int main(int argc, char **argv)
{
	// Load config files

	lsh_loop(); // To be defined

	// Perform shutdown
	return EXIT_SUCCESS;
}

void lsh_loop()
{
	char *line;
	char **args;
	int status;

	do
	{
		printf("> ");
		line = lshReadLine();
		args = lshSplitLine(line);
		status = lshExecute(args);

		free(line);
		free(args);
	} while (status);
}
