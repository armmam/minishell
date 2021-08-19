#include "unity.h"
#include "../minishell.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void	test_extracttoken(void)
{
	char	*token;

	token = NULL;

	TEST_ASSERT_EQUAL_INT32(2, ft_extracttoken("<<an", &token));
	TEST_ASSERT_EQUAL_STRING("<<", token); free(token);

	TEST_ASSERT_EQUAL_INT32(4, ft_extracttoken("  <<an", &token));
	TEST_ASSERT_EQUAL_STRING("<<", token); free(token);

	TEST_ASSERT_EQUAL_INT32(3, ft_extracttoken(" >>an", &token));
	TEST_ASSERT_EQUAL_STRING(">>", token); free(token);

	TEST_ASSERT_EQUAL_INT32(2, ft_extracttoken(" >an", &token));
	TEST_ASSERT_EQUAL_STRING(">", token); free(token);

	TEST_ASSERT_EQUAL_INT32(1, ft_extracttoken(">an", &token));
	TEST_ASSERT_EQUAL_STRING(">", token); free(token);

	TEST_ASSERT_EQUAL_INT32(3, ft_extracttoken(" >>an", &token));
	TEST_ASSERT_EQUAL_STRING(">>", token); free(token);
}

void	test_tokenize(void)
{
	t_darr	*expected;

	expected = ft_darrnew(10);
	ft_darrpushback(expected, ft_strdup("<<"));
	ft_darrpushback(expected, ft_strdup("an"));
	TEST_ASSERT_EQUAL_STRING(expected->ptr[0], "<<");
	TEST_ASSERT_EQUAL_STRING(expected->ptr[1], "an");
	TEST_ASSERT_EQUAL_STRING_ARRAY(expected->ptr, ft_tokenize("   <<an    "), 2);
	TEST_ASSERT_EQUAL_STRING_ARRAY(expected->ptr, ft_tokenize(" <<   an    "), 2);
	TEST_ASSERT_EQUAL_STRING_ARRAY(expected->ptr, ft_tokenize(" <<  an"), 2);
	ft_darrclear(expected);

	expected = ft_darrnew(10);
	ft_darrpushback(expected, ft_strdup("|"));
	ft_darrpushback(expected, ft_strdup("|||"));
	TEST_ASSERT_EQUAL_STRING_ARRAY(expected->ptr, ft_tokenize("| |||"), 2);
	TEST_ASSERT_EQUAL_STRING_ARRAY(expected->ptr, ft_tokenize("  \t  | |||"), 2);
	ft_darrclear(expected);

}
