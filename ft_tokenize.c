#include "minishell.h"

int		ft_envlen(char *line)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(line[i]) || line[i] == '_'))
		return (i);
	while (ft_isalnum(line[++i]) || line[i] == '_')
		;
	return (i);
}

// takes a string and substitutes every environ variable encountered. frees the original string, allocates memory
// u can use ft_getenv to get the needed var
char	*ft_refineline(char *line)
{
	char	*ptr, *prefix, *postfix, *env, *val;
	int		env_len;

	if (!line)
		return (NULL);
	ptr = line;
	while ((ptr = ft_strchr(ptr, '$')))
	{
		if ((env_len = ft_envlen(ptr + 1)))
		{
			env = ft_substr(ptr, 1, env_len);
			val = ft_strdup(ft_getenv(env));
			free(env);
			prefix = ft_substr(line, 0, ptr - line);
			postfix = ft_strdup(ptr + env_len + 1);
		}
		else if (*(ptr + 1) == '?')
		{
			val = ft_itoa(g_data.status); // statuses cannot be greater than MAX_INT, right?
			prefix = ft_substr(line, 0, ptr - line);
			postfix = ft_strdup(ptr + 2);
		}
		else // just a regular dollar sign, do nothing
		{
			ptr++;
			continue ;
		}
		ptr = val ? ft_strjoin3(prefix, val, postfix) : ft_strjoin(prefix, postfix); // this block of code will only execute if $HOME or $?
		free(line);
		line = ptr;
		ptr += ft_strlen(prefix) + ft_strlen(val); // to avoid double expansion
		free(prefix);
		free(postfix);
		free(val);
	}
	return (line);
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

/*
 * assigns the first token in `line` to `token`
 * returns the index of the first char in `line` after `token`
 */
int		ft_extract_token(const char *line, char **token, char **quote)
{
	size_t	i, j, expand;
	char	*tmp;

	i = 0; // index of the first char in the part of token about to be appended to `token`
	j = 0; // index of the current char
	expand = 1; // flag for expansion; 1 by default, 0 if single quotes encountered
	tmp = NULL;
	while (ft_isspace(line[j]))
	{
		j++;
		i++;
	}
	if (line[j])
	{
		*token = ft_strdup("");
		*quote = ft_strdup("");
	}
	else
	{
		*token = NULL;
		*quote = NULL;
	}
	while (!ft_isspace(line[j]) && line[j])
	{
		if ((line[j] == '<' || line[j] == '>' || line[j] == '|') && (j != i || (*token)[0] != '\0')) // encountered </>/| but also need to add something encountered before `token` OR already added something to `token`
		{
			ft_appendtoken(token, &line[i], j - i, expand); // append whatever has not been appended yet
			return (j); // </> will be included in the next call to this function, not this one
		}
		else if (line[j] == '<' || line[j] == '>' || line[j] == '|') // encountered </>/| and haven't added anything to `token` yet
		{ // here we assume that i == j
			if (!ft_strncmp(&line[j], "<<", 2) || !ft_strncmp(&line[j], ">>", 2))
				j++;
			ft_appendtoken(token, &line[i], j - i + 1, expand);
			return (j + 1);
		}
		if (line[j] != '\'' && line[j] != '\"' && !ft_isspace(line[j + 1]) && line[j + 1]) // no weird stuff is about to be encountered
			 ;
		else if ((ft_isspace(line[j + 1]) && !(line[j] == '\'' || line[j] == '\"')) || !line[j + 1]) // space or \0 is about to be encountered (and not on quote now), append the last part of token to `token`
			ft_appendtoken(token, &line[i], j - i + 1, expand);
		else // a quote has been encountered (and it's not the very last char in `line`)
		{
			tmp = ft_strchr(&line[j + 1], line[j]); // try to find a closing quote
			if (tmp) // found a closing quote
			{
				if (!**quote)
					ft_appendtoken(quote, "\'", 1, 0);
					// *quote = ft_strjoinsafe(quote, ft_strdup("\'"));
				ft_appendtoken(token, &line[i], j - i, expand); // first safe everything (that was not already saved) up to the quote (not including) into `token`
				i = j + 1; // update the beginning of the part of token about to be appended to `token` (set it to the first char after the opening quote)
				j = (tmp - 1) - line; // index of the char before the closing quote
				if (line[j + 1] == '\'') // if a single quote has been encountered
					expand = 0; // no expansion for this part of the token
				ft_appendtoken(token, &line[i], j - i + 1, expand); // extract everything between quotes, expand environment variables if double quotes, append everything between quotes to `token`
				expand = 1; // expand next parts of the token by default
				i = j + 2; // index of the char after the closing quote
				j = i;
				continue ;
			}
			// else, if didn't find a closing quote, treat it as a regular char, i.e. just skip it and later append to `token`
		}
		j++;
	}
	return (j);
}

// PLEASE MERGE THIS ONE WITH ft_parse_commands SO YOU'RE ABLE TO CORRECTLY
// REMOVE ()S AND SET RESPECTIVE COMMAND'S cond FIELD.
// removes ""s, ()s and other trash, uses ft_refineline on the arguments of commands
t_tokens	*ft_tokenize(const char *line)
{
	size_t	i;
	char	*token;
	char	*quote;
	// t_darr	*tokens;
	// t_darr	*quotes;
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
		{
			ft_darrpushback(ret->tokens, token);
			ft_darrpushback(ret->quotes, quote);
		}
	}
	ft_darrpushback(ret->tokens, NULL); // null-terminate the matrix
	ft_darrpushback(ret->quotes, NULL); // null-terminate the matrix
	return (ret);
}
