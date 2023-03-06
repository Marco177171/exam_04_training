#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#define EXIT 0

typedef struct s_cmd {
	char **args;
	int length;
	int type;
	int pipes[2];
	struct s_cmd *previous;
	struct s_cmd *next;
} t_cmd;

int ft_strlen(char *str)
{
	int index;

	index = 0;
	while(str[index])
		index++;
	return (index);
}

char *ft_strjoin(char *s1, char *s2)
{
	char *result;
	int length;
	int i;
	int j;

	length = ft_strlen(s1) + ft_strlen(s2) + 1;
	result = malloc (sizeof(char) * length);
	result[length] = '\0';
	if (!result)
		return (NULL);
	i = -1;
	while (s1[++i])
		result[i] = s1[i];
	j = -1;
	while (s2[++j])
		result[i++] = s2[j];
	result[i] = '\0';
	return (result);
}

int ft_error(char *error, char *arg, int exit)
{
	char *message;
	//EXIT exit;

	if (arg)
	{
		message = ft_strjoin(error, arg);
		write(2, message, ft_strlen(message));
		write(2, "\n", 1);
		free(message);
		//EXIT = exit;
		return (EXIT);
	}
	else
	{
		write(2, error, ft_strlen(error));
		write(2, "\n", 1);
		free(error);
		//EXIT = exit;
		return (EXIT);
	}
}

int ft_cd(char *argv[])
{
	int index;

	index = 2;
	if (!argv[2])
		return (0);
	if (argv[2] && chdir(argv[2]) == 0) // l'indirizzo esiste
	{
		write(1, "changed\n", 8);
		return (EXIT);
	}
	else
		return (ft_error("error: cd: cannot change directory to ", argv[2], 1));
	return (EXIT);
}

int main(int argc, char *argv[], char **env)
{
	t_cmd *cmds;
	int index;
	int ret;

	ret = 0;
	cmds = NULL;
	if (argv[1])
	{
		index = 1;
		if (strncmp(argv[1], "cd", 3) == 0)
			return (ft_cd(argv));
		else
			return (execve(argv[1], (char *const*) argv[2], env));
	}
	exit(EXIT);
	return (EXIT);
}
