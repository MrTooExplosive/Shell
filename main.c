#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELM " \t\r\n\a"

int lsh_cd(char **args);
char **lshSplitLine(char *line);
void lsh_loop();
char *lshReadLine();
int lshLaunch(char **args);
int lsh_help(char ** args);
int lsh_exit(char **args);
int lshNumBuiltins();
int lshExecute(char **args);

// Variables for built-in commands
char *builtinStr[] = {"cd", "help", "exit"};
int (*builtinFunc[]) (char **) = { &lsh_cd, &lsh_help, &lsh_exit };

int main(int argc, char **argv)
{
	// Load config files

	lsh_loop();

	// Perform shutdown
	return EXIT_SUCCESS;
}

// Executes a command
int lshExecute(char **args)
{
	// Check if user entered a blank line
	if (args[0] == NULL)
		return 1;

	// Check if user used a built in command
	for (int index = 0; index < lshNumBuiltins(); index++)
	{
		if (strcmp(args[0], builtinStr[index]) == 0)
			return (*builtinFunc[index]) (args);
	}

	return lshLaunch(args);
}

// Gets the number of built-in commands
int lshNumBuiltins()
{
	return sizeof(builtinStr) / sizeof(char *);
}

// Built-in exit command
int lsh_exit(char **args)
{
	return 0;
}

// Built-in help command
int lsh_help(char **args)
{
	printf("Makarius Salib's LSH\n");
	printf("Type program arguments, and hit enter.\n");
	printf("The following are built in:\n");

	// Print each built in argument
	for (int index = 0; index < lshNumBuiltins(); index++)
		printf("   %s\n", builtinStr[index]);

	printf("Use the man command for information on other programs.\n");
	return 1;
}

// Built-in change directory command
int lsh_cd(char **args)
{
	if (args[1] == NULL)  // Check for an argument to cd
		fprintf(stderr, "lsh: expected argument to \"cd\"\n");
	else
	{
		if (chdir(args[1]) != 0)
			perror("lsh");
	}
	return 1;
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

int lshLaunch(char **args)
{
	pid_t pid, wpid;
	int status;

	pid = fork;
	if (pid == 0)
	{  // Child process
		if (execvp(args[0], args) == -1)
			perror("lsh");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0) // Error forking
		perror("lsh");
	else
	{ // Parent process
		do
		{
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}
