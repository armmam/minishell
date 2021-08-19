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
	t_dmtx	*expected;

	expected = ft_dmtxnew(10);
	ft_dmtxpushback(expected, "<<");
	ft_dmtxpushback(expected, "an");
	TEST_ASSERT_EQUAL_STRING(expected->ptr[0], "<<");
	TEST_ASSERT_EQUAL_STRING(expected->ptr[1], "an");
	TEST_ASSERT_EQUAL_STRING_ARRAY(expected->ptr, ft_tokenize("a   <<an    "), 2);
}
