#include "libft.h"

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char	*temp;
	char	*ret;

	if (!s1 || !s2 || !s3)
		return (NULL);
	if (!(temp = ft_strjoin(s1, s2)))
		return (NULL);
	ret = ft_strjoin(temp, s3);
	free(temp);
	return (ret);
}
