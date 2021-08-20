# Try to break minishells

## Unity

`ruby generate_test_runner.rb '/Users/amamian/coding/ecole/projects/minishell/gh-repo/test/test_tokenize.c' '/Users/amamian/coding/ecole/projects/minishell/gh-repo/test/test_tokenize_runner.c'`

`gcc test/test_tokenize.c test/test_tokenize_runner.c test/unity.c ft_tokenize.c libft/libft.a ft_environ.c -g && ./a.out`

add `-g` to libft Makefile


 ## Condition matrix for ft_extracttoken
```
           +1: ' '     +1: \0         !
\'           3            2           3
\"           3            2           3
!            2            2           1
```