#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

int ft_strlen(char *str)
{
	int index;

	index = 0;
	while (str[index])
		index++;
	return (index);
}

void ft_error(char *str)
{
	write(2, str, ft_strlen(str));
}

int main(int argc, char *argv[], char *env[])
{
	int pipe[2];
	int exit_status;
	int pipe_count;
	int index;
	(void) argc;

	pipe[0] = dup(STDIN_FILENO);
	pipe[1] = dup(STDOUT_FILENO);
	exit_status = 0;
	pipe_count = 1;
	index = 1;
	while (argv[index])
	{
		if (strcmp(argv[index], "|") == 0 || strcmp(argv[index], ";") == 0)
			pipe_count++;
		index++;
	}
	index = 1;
	while (argv[index])
	{
		if (strcmp(argv[index], "|") == 0 || strcmp(argv[index], ";") == 0)
		{
			index++;
			pipe_count--;
		}
		// se pipe count torna a 1 -> fd = STDOUT
		else if (strcmp(argv[index], "cd") == 0)
		{
			index++;
			if (!argv[index])
				ft_error("error: cd: bad argument\n");
			else if (chdir(argv[index]) == -1)
			{
				ft_error("error: cd: cannot chage dir to ");
				ft_error(argv[index]);
				ft_error("\n");
			}
		}
		else if (execve(argv[index], &argv[index], env) == -1)
		{
			ft_error("error: cannot execute ");
			ft_error(argv[index]);
			ft_error("\n");
			index++;
		}
		index++;
	}
	return (exit_status);
}