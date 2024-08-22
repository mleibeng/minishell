/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 19:42:46 by cpuiu             #+#    #+#             */
/*   Updated: 2024/04/26 15:10:06 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "environment.h"
#include "errors.h"
#include "executor.h"
#include "free.h"
#include "minishell.h"
#include "redirection.h"
#include "utilities.h"

char	*find_command_path(char *command, char **paths)
{
	char	*full_path;
	int		i;

	if (ft_strchr(command, '/') != NULL)
	{
		if (!access(command, X_OK))
			return (ft_strdup(command));
		else
			return (NULL);
	}
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin_free(full_path, command, 1);
		if (!access(full_path, X_OK))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

void	handle_specific_error(char **args, int *exit_status)
{
	(void)exit_status;
	if (ft_strcmp(args[0], ".") == 0)
	{
		if (args[1] == NULL)
		{
			printf("minishell: .: filename argument required\n");
			printf(".: usage: . filename [arguments]\n");
			exit(2);
		}
		else
		{
			printf("minishell: command not found\n");
			exit(127);
		}
	}
	if (ft_strcmp(args[0], "..") == 0)
	{
		printf("minishell: command not found\n");
		exit(127);
	}
}

char	*get_command_path(char *command, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*command_path;

	path_env = ft_getenv("PATH", envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	command_path = find_command_path(command, paths);
	ft_free_split(paths);
	return (command_path);
}

void	execute_command_from_path(char **args, char *command_path,
		int *exit_status, char **envp)
{
	if (args[0] == NULL || *args[0] == '\0')
		exit(0);
	handle_specific_error(args, exit_status);
	if (execve(command_path, args, envp) == -1)
	{
		ft_fprintf(STDERR_FILENO, "execve failed: %s\n", strerror(errno));
		free(command_path);
		exit(126);
	}
	free(command_path);
}

int	wait_and_update_status(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (WTERMSIG(status));
	else
		return (1);
}
