/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extract_token.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amamian <amamian@student.42yerevan.am>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 16:20:55 by amamian           #+#    #+#             */
/*   Updated: 2021/09/27 16:20:56 by amamian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * is run if encountered </>/|
 * IF (need to add something encountered before `token` OR already added
 *     something to `token`)
 * ELSE (haven't added anything to `token` yet + we assume that i == j)
 */
int	ft_extract_redirpipe(const char *line, char **token, t_expandt *e)
{
	if ((e->j != e->i || (*token)[0] != '\0'))
	{
		ft_appendtoken(token, &line[e->i], e->j - e->i, e->expand);
		return (e->j);
	}
	else
	{
		if (!ft_strncmp(&line[e->j], "<<", 2) || !ft_strncmp(&line[e->j],
				">>", 2))
			e->j++;
		ft_appendtoken(token, &line[e->i], e->j - e->i + 1, e->expand);
		return (e->j + 1);
	}
}

/*
 * process a quote that has been encountered (and it's not the very last char
 * in `line`)
 * 
 * IF (found a closing quote)
 * ELSE (if didn't find a closing quote, treat it as a regular char, i.e. just
 *       skip it and later append to `token`)
 */
int	ft_process_tokenquote(const char *line, char **token, char **quote,
	t_expandt *e)
{
	char	*tmp;

	tmp = ft_strchr(&line[e->j + 1], line[e->j]);
	if (tmp)
	{
		if (!**quote)
			ft_appendtoken(quote, "\'", 1, 0);
		ft_appendtoken(token, &line[e->i], e->j - e->i, e->expand);
		e->i = e->j + 1;
		e->j = (tmp - 1) - line;
		if (line[e->j + 1] == '\'')
			e->expand = 0;
		ft_appendtoken(token, &line[e->i], e->j - e->i + 1, e->expand);
		e->expand = 1;
		e->i = e->j + 2;
		e->j = e->i;
		return (1);
	}
	return (0);
}

/*
 * process a token char that is neither a redirection nor a pipe
 *
 * IF (no weird stuff is about to be encountered)
 * ELSE IF (space or \0 is about to be encountered (and not on quote now),
 *          append the last part of token to `token`)
 * ELSE (a quote has been encountered (and it's not the very last char in
 *       `line`))
 */
int	ft_process_tokenchar(const char *line, char **token, char **quote,
	t_expandt *e)
{
	int		ret;

	if (line[e->j] != '\'' && line[e->j] != '\"'
		&& !ft_isspace(line[e->j + 1]) && line[e->j + 1])
			;
	else if ((ft_isspace(line[e->j + 1]) && !(line[e->j] == '\''
				|| line[e->j] == '\"')) || !line[e->j + 1])
		ft_appendtoken(token, &line[e->i], e->j - e->i + 1, e->expand);
	else
	{
		ret = ft_process_tokenquote(line, token, quote, e);
		if (ret)
			return (1);
	}
	return (0);
}

/*
 * e.i -- index of the first char in the part of token about to be appended
          to `token`
 * e.j -- index of the current char
 * e.expand -- flag for expansion; 1 by default, 0 if single quotes encountered
 */
void	ft_extract_token_init(const char *line, char **token, char **quote,
	t_expandt *e)
{
	e->i = 0;
	e->j = 0;
	e->expand = 1;
	while (ft_isspace(line[e->j]))
	{
		e->j++;
		e->i++;
	}
	if (line[e->j])
	{
		*token = ft_strdup("");
		*quote = ft_strdup("");
	}
	else
	{
		*token = NULL;
		*quote = NULL;
	}
}

/*
 * assigns the first token in `line` to `token`
 * returns the index of the first char in `line` after `token`
 */
int	ft_extract_token(const char *line, char **token, char **quote)
{
	t_expandt	e;
	int			cont;

	ft_extract_token_init(line, token, quote, &e);
	while (!ft_isspace(line[e.j]) && line[e.j])
	{
		if (line[e.j] == '<' || line[e.j] == '>' || line[e.j] == '|')
			return (ft_extract_redirpipe(line, token, &e));
		cont = ft_process_tokenchar(line, token, quote, &e);
		if (cont)
			continue ;
		e.j++;
	}
	return (e.j);
}
