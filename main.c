#include <stdio.h>
#include <stdlib.h>
#define LSH_RL_BUFSIZE 1024

void lsh_loop();
char *lshReadLine();

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

char *lshReadLine()
{
	int bufsize = LSH_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;

	if (!buffer)
	{
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		// Reads a character
		c = getchar();
		if (c == EOF || c == '\n') // Replaces end of file or newline with a null character then returns
		{
			buffer[position] = '\0';
			return buffer;
		}
		else
			buffer[position] = c;  // Put the character into the buffer
		position++;

		// Reallocate if needed
		if (position >= bufsize)
		{
			bufsize += LSH_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if (!buffer)
			{
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}
