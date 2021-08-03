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

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char	*temp, ret;

	if (!s1 || !s2 || !s3)
		return (NULL);
	if (!(temp = ft_strjoin(s1, s2)))
		return (NULL);
	ret = ft_strjoin(temp, s3);
	free(temp);
	return (ret);

}

// PLEASE MERGE THIS ONE WITH ft_parsecommands SO YOU'RE ABLE TO CORRECTLY
// REMOVE ()S AND SET RESPECTIVE COMMAND'S cond FIELD.
// removes ""s, ()s and other trash, uses ft_refineline on the arguments of commands
char	**ft_tokenize(char *line)
{
	(void)line;
	//
	return (NULL);
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
