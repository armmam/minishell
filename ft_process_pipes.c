/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_pipes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 13:40:23 by amamian           #+#    #+#             */
/*   Updated: 2021/09/27 13:40:24 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * check whether </>>/> is the last token in the command OR encountered an
 * unquoted redireciton operator after another one
 */
int	ft_islastredir_tworedirs(char ***token, char ***quote)
{
	if (token == NULL || *token == NULL || **token == NULL
		|| ((*token)[0][0] == '|' && (*token)[0][1] == '\0')
			|| (quote && *quote && **quote && !***quote
				&& (!ft_strcmp(**token, "<<") || !ft_strcmp(**token, ">>")
					|| !ft_strcmp(**token, "<") || !ft_strcmp(**token, ">"))))
		return (1);
	return (0);
}

/*
 * move  to the next token/quote
 */
void	ft_next_tokenquote(char ***token, char ***quote)
{
	(*token)++;
	(*quote)++;
}

/*
 * checks whether the current token is `|` and the next one is not
 */
int	ft_currpipe_nextnot(char ***token)
{
	if (*token && **token && !ft_strcmp(**token, "|")
		&& (*(*token + 1) != NULL && ft_strcmp(*(*token + 1), "|")))
		return (1);
	return (0);
}

/*
 * checks whether both the current and the next ones are `|`
 * or the current one is `|` and the next one is null
 */
int	ft_currpipe_nextpipenull(char ***token)
{
	if (*token && **token && !ft_strcmp(**token, "|")
		&& (*(*token + 1) == NULL || !ft_strcmp(*(*token + 1), "|")))
		return (1);
	return (0);
}

/*
 * processes pipes:
 *    if the current one is `|` and the next one is not
 *    if both the current and the next ones are `|`
 */
int	ft_process_pipes(char ***token, char ***quote, t_darr **args)
{
	if (ft_currpipe_nextnot(token))
		ft_next_tokenquote(token, quote);
	else if (ft_currpipe_nextpipenull(token))
	{
		ft_error(**token, "syntax error");
		ft_darrclear(*args);
		g_data.status = 258;
		return (1);
	}
	return (0);
}
