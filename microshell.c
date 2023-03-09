#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int ft_putstr_fd(char *str, char *arg)
{
	while (*str)
		write(2, str++, 1);
	if (arg)
		while (*arg)
			write(2, arg++, 1);
	write(2, "\n", 1);
	return (1);
}

int ft_execute(char *argv[], int index, int tmp_fd, char *env[])
{
	argv[index] = NULL;
	dup2(tmp_fd, STDIN_FILENO);
	close(tmp_fd);
	execve(argv[0], argv, env);
	return (ft_putstr_fd("error: cannot execute ", argv[0]));
}

int main(int argc, char *argv[], char *env[])
{
	int index;
	int fd[2];
	int tmp_fd;
	(void) argc;

	index = 1;
	tmp_fd = dup(STDIN_FILENO);
	while (argv[index] && argv[index + 1])
	{
		if (!strcmp(argv[index], ";") || !strcmp(argv[index], "|"))
		{
			index++;
			break ;
		}
		if (strcmp(argv[index], "cd") == 0) // cd found
		{
			index++;
			if (!argv[index])
			{
				ft_putstr_fd("error: cd: arguments", NULL);
				break ;
			}
			if (chdir(argv[index]) != 0)
				ft_putstr_fd("error: cd: cannot change directory to ", argv[index]);
		}
		else if (index != 0 && (argv[index] == NULL || strcmp(argv[index], ";") == 0)) // exec in stdout
		{
			if (fork() == 0)
			{
				if (ft_execute(argv, index, tmp_fd, env))
					return (1);
			}
			else
			{
				close(tmp_fd);
				while (waitpid(-1, NULL, WUNTRACED) != -1)
					;
				tmp_fd = dup(STDIN_FILENO);
			}
		}
		else if (index != 0 && strcmp(argv[index], "|") == 0) // pipe found
		{
			pipe(fd);
			if (fork() == 0)
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				if (ft_execute(argv, index, tmp_fd, env))
					return (1);
			}
			else
			{
				close(fd[1]);
				close(tmp_fd);
				tmp_fd = fd[0];
			}
		}
		index++;
	}
	close(tmp_fd);
	return (0);
}