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

/*
 * this function replaces two lines in ft_extracttoken:
 * tmp = ft_substr(line, i, j - i + 1);
 * *token = ft_strjoinsafe(token, &tmp);
*/
void	ft_appendtoken(char **token, char *new, size_t len)
{
	char	*tmp;

	tmp = ft_substr(new, 0, len);
	*token = ft_strjoinsafe(token, &tmp);
}

/*
 * assigns the first token in `line` to `token`
 * returns the index of the first char in `line` after `token`
 */
int		ft_extracttoken(const char *line, char **token)
{
	size_t	i, j;
	char	*tmp, *match;

	i = 0; // index of the first char in the part of token about to be appended to `token`
	j = 0; // index of the current char
	tmp = NULL;
	*token = ft_strdup("");
	while (ft_isspace(line[j++]))
		i++;
	while (!ft_isspace(line[j]) && line[j])
	{
		if (line[j] != '\'' && line[j] != '\"' && !ft_isspace(line[j + 1]) && line[j + 1]) // no weird stuff is about to be encountered
			 ;
		else if (ft_isspace(line[j + 1]) || !line[j + 1]) // space or \0 is about to be encountered, append the last part of token to `token`
			ft_appendtoken(token, &line[i], j - i + 1);
		else // a quote has been encountered (and it's not the very last char in `line`)
		{
			tmp = ft_strchr(&line[j + 1], line[j]); // try to find a closing quote
			if (tmp) // found a closing quote
			{
				ft_appendtoken(token, &line[i], j - i); // first safe everything (that was not already saved) up to the quote (not including) into `token`
				i = j + 1; // update the beginning of the part of token about to be appended to `token` (set it to the first char after the opening quote)
				j = (tmp - 1) - line; // index of the char before the closing quote
				tmp = ft_substr(line, i, j - i + 1); // extract everything between quotes
				if (line[j + 1] == '\"') // if a double quote has been encountered
					tmp = ft_refineline(tmp); // then expand environment variables
				*token = ft_strjoinsafe(token, &tmp); // append everything between quotes to `token`
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

// PLEASE MERGE THIS ONE WITH ft_parsecommands SO YOU'RE ABLE TO CORRECTLY
// REMOVE ()S AND SET RESPECTIVE COMMAND'S cond FIELD.
// removes ""s, ()s and other trash, uses ft_refineline on the arguments of commands
//
// anything in double parentheses is not executed and its return status is set to false (e.g. `((pwd)) && ls` doesn't print out anything)
//
// execution stops as soon as the global state is true or false (e.g. `pwd || ls` only executes `pwd`, `((pwd)) && ls` doesn't print out anything)
char	**ft_tokenize(const char *line)
{
	size_t	i;
	char	*token;
	t_dmtx	*tokens;

	tokens = ft_dmtxnew(0);
	i = 0;
	while (line[i])
	{
		i += ft_extracttoken(&line[i], &token);
		ft_dmtxpushback(tokens, token);
	}
	ft_dmtxpushback(tokens, NULL); // null-terminate the matrix
	return (tokens->ptr);
}

// takes a string and substitutes every environ variable encountered. frees the original string, allocates memory
// u can use ft_getenv to get the needed var
//
// $HOME -> /Users/arman
// '$HOME' -> $HOME
// "$HOME" -> /Users/arman
// $HOMEE -> (nothing)
// $HOMEE'andthen'$HOME -> andthen/Users/arman
// $HOMEE 'andthen' $HOME -> andthen /Users/arman
// $$ -> current pid (e.g. 23298)
// $ -> $
// $$$ -> 23298$
// $$$$ -> 2329823298
//
// valid chars that env var can
//                              -consist of and can begin with: upper/lowercase letters, underscores
//                              -consist of                   : numbers
//
// Double expansions do not take place:
// bash-3.2$ tar_tar='$tur'
// bash-3.2$ echo $tar_tar
// $tur
//
// If heredoc is non-quoted, then env vars inside of it are ALWAYS expanded, no matter whether they themselves are quoted or not:
// bash-3.2$ cat << END
// > $HOME
// > END
// /Users/arman
// bash-3.2$ cat << END
// > "$HOME"
// > END
// "/Users/arman"
// bash-3.2$ cat << END
// > '$HOME'
// > END
// '/Users/arman'
char	*ft_refineline(char *line)
{
	char	*ptr, *prefix, *postfix, *env, *val;
	int		env_len;

	if (!line)
		return (NULL);
	ptr = line;
	while (ptr = ft_strchr(ptr, '$'))
	{
		if ((env_len = ft_envlen(ptr + 1)))
		{
			env = ft_substr(ptr, 1, env_len);
			val = ft_getenv(env);
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
		if (val)
			free(val);
		free(prefix);
		free(postfix);
	}
	return (line);
}
