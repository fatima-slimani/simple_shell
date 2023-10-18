#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

#define MAX_TOKENS 64

void execute_command(char **args)
{
	pid_t child_pid;
	int status;

	child_pid = fork();
	if (child_pid == 0)
	{

		if (execvp(args[0], args) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
	else if (child_pid < 0)
	{
		perror("fork");
	}
	else
	{
		do {
			waitpid(child_pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}

int tokenize(char *input, char *tokens[MAX_TOKENS])
{
	int token_count;

	token_count = 0;
	tokens[token_count] = strtok(input, " \t\n");
	while (tokens[token_count] != NULL)
	{
		token_count++;
		tokens[token_count] = strtok(NULL, " \t\n");
	}
	return (token_count);
}

int main(void)
{
	char *input = NULL;
	size_t input_size = 0;
	char *tokens[MAX_TOKENS];

	while (1)
	{
		printf("#cisfun$ ");
		if (getline(&input, &input_size, stdin) == -1)
			break;

		if (tokenize(input, tokens) <= 0)
			continue;

		if (strcmp(tokens[0], "exit") == 0)
			break;

		execute_command(tokens);
	}

	return (0);
}

