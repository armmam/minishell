/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:20:47 by amamian           #+#    #+#             */
/*   Updated: 2021/09/27 16:20:48 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * returns the length of the name of the env var that is right after a dollar
 */
int	ft_envlen(char *line)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(line[i]) || line[i] == '_'))
		return (i);
	while (ft_isalnum(line[++i]) || line[i] == '_')
		;
	return (i);
}

int	ft_process_dollarsign(t_refine *r, char *line)
{
	int		env_len;
	char	*env;

	env_len = ft_envlen(r->ptr + 1);
	if (env_len)
	{
		env = ft_substr(r->ptr, 1, env_len);
		r->val = ft_strdup(ft_getenv(env));
		free(env);
		r->prefix = ft_substr(line, 0, r->ptr - line);
		r->postfix = ft_strdup(r->ptr + env_len + 1);
	}
	else if (*(r->ptr + 1) == '?')
	{
		r->val = ft_itoa(g_data.status);
		r->prefix = ft_substr(line, 0, r->ptr - line);
		r->postfix = ft_strdup(r->ptr + 2);
	}
	else
	{
		r->ptr++;
		return (1);
	}
	return (0);
}

/*
 * allocates first `len` chars of string `new`,
 * expands its contents if `expand` is set to 1,
 * appends this string to `token`
 * and clears up all intermediary allocations
*/
void	ft_appendtoken(char **token, const char *new, size_t len, int expand)
{
	char	*tmp;

	tmp = ft_substr(new, 0, len);
	if (expand)
		tmp = ft_refineline(tmp);
	*token = ft_strjoinsafe(token, &tmp);
}

void	ft_darrpushback_tokens(t_tokens *ret, char *token, char *quote)
{
	ft_darrpushback(ret->tokens, token);
	ft_darrpushback(ret->quotes, quote);
}

/*
 * removes ""s, ()s and other trash, uses ft_refineline on the arguments of
 * commands
 */
t_tokens	*ft_tokenize(const char *line)
{
	size_t		i;
	char		*token;
	char		*quote;
	t_tokens	*ret;

	ret = ft_calloc(1, sizeof(t_tokens));
	i = 0;
	if (line)
	{
		ret->tokens = ft_darrnew(0);
		ret->quotes = ft_darrnew(0);
	}
	else
		return (NULL);
	while (line[i])
	{
		i += ft_extract_token(&line[i], &token, &quote);
		if (token != NULL && quote != NULL)
			ft_darrpushback_tokens(ret, token, quote);
	}
	ft_darrpushback_tokens(ret, NULL, NULL);
	return (ret);
}
