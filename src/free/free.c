/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpuiu <cpuiu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 00:22:57 by marvinleibe       #+#    #+#             */
/*   Updated: 2024/04/25 21:42:03 by cpuiu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "free.h"
#include "minishell.h"

void	ft_free_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i] != NULL)
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	ft_free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->data)
	{
		free(ast->data);
		ast->data = NULL;
	}
	if (ast->redirection_file)
	{
		free(ast->redirection_file);
		ast->redirection_file = NULL;
	}
	if (ast->redirections)
	{
		free(ast->redirections);
		ast->redirections = NULL;
	}
	free_ast(ast->left);
	free_ast(ast->right);
	free(ast);
	ast = NULL;
}
