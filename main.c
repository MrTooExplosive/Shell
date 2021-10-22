#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELM " \t\r\n\a"

char **lshSplitLine(char *line);
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

char **lshSplitLine(char *line)
{
	int bufsize = LSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens)
	{
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, LSH_TOK_DELIM);
	while (token != NULL)
	{
		tokens[position] = token;
		position++;

		if (position >= bufsize)
		{
			bufsize += LSH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens)
			{
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, LSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}
