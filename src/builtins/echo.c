/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 05:16:28 by mleibeng          #+#    #+#             */
/*   Updated: 2024/04/23 21:40:45 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "builtins.h"

int	is_option(t_ast *arg)
{
	if ((arg->type == AST_WORD || arg->type == AST_SINGLEQUOTED_WORD
			|| arg->type == AST_DOUBLEQUOTED_WORD) && arg->data[0] == '-'
		&& arg->data[1] && ft_isalpha(arg->data[1]))
	{
		return (1);
	}
	return (0);
}

void	check_flag_echo(t_ast **arg, int *print_newline)
{
	const char	*options;
	int			contains_n_only;

	options = (*arg)->data + 1;
	contains_n_only = 1;
	while (*options)
	{
		if (*options != 'n')
		{
			contains_n_only = 0;
			break ;
		}
		options++;
	}
	if (contains_n_only)
	{
		*print_newline = 0;
		while ((*arg)->right && (*arg)->right->type == AST_WHITESPACE)
			*arg = (*arg)->right;
		*arg = (*arg)->right;
	}
}

void	print_ast_arguments(t_ast *arg, char *exit_stat)
{
	char	*var_name;

	if (arg->type == AST_WORD || arg->type == AST_SINGLEQUOTED_WORD)
		ft_putstr_fd(arg->data, STDOUT_FILENO);
	else if (arg->type == AST_DOUBLEQUOTED_WORD)
		process_double_quoted_str(arg->data, exit_stat);
	else if (arg->type == AST_VARIABLE)
	{
		var_name = arg->data + 1;
		var_name = getenv(var_name);
		if (var_name)
			ft_putstr_fd(var_name, STDOUT_FILENO);
	}
	else if (arg->type == AST_SPECIAL_PARAM)
		ft_putstr_fd(exit_stat, STDOUT_FILENO);
}

void	process_echo_argument(t_ast **arg, int *exit_status, char *exit_stat,
		int *first_arg)
{
	if ((*arg)->type == AST_SPECIAL_PARAM)
		ft_printf("%d", *exit_status);
	else if ((*arg)->type != AST_WHITESPACE && (*arg)->type != AST_REDIRECTION)
	{
		if (!*first_arg)
			ft_putchar_fd(' ', STDOUT_FILENO);
		print_ast_arguments(*arg, exit_stat);
		*first_arg = 0;
	}
	*arg = (*arg)->right;
}

int	execute_echo(t_ast *args, int *exit_status)
{
	t_ast	*arg;
	char	*exit_stat;
	int		print_newline;
	int		first_arg;

	first_arg = 1;
	print_newline = 1;
	exit_stat = ft_itoa(*exit_status);
	arg = args->right;
	while (arg && arg->type == AST_WHITESPACE)
		arg = arg->right;
	if (arg && is_option(arg))
		check_flag_echo(&arg, &print_newline);
	while (arg)
		process_echo_argument(&arg, exit_status, exit_stat, &first_arg);
	if (print_newline)
		ft_putendl_fd("", STDOUT_FILENO);
	free(exit_stat);
	return (0);
}
