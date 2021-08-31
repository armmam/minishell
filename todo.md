# Annihilation list

## basic stuff

```
minishell$ echo>>4
```

```
minishell$ echo >>5
```

```
minishell$ echo 4|wc -l
```

```
minishell$ echo "4 " |wc -l
```

```
minishell$ < text.txt | cat
```

should quit immediately, cat should receive no input

## reading from and to the same file

```
minishell$ <file2 grep a >file2
```

must empty file2 considering that it has lines containing a character
our minishell fails to do so

## grep and file redirections

```
minishell$ <file2 grep a >file2
```

must empty file2 considering that it has lines containing a character
our minishell fails to do so

## heredocs with quotes

```
bash-3.2$ << hiand cat
> $HOME
> hiand
/Users/arman
```

```
bash-3.2$ << hi"and" cat
> $HOME
> hiand
$HOME
```

```
bash-3.2$ << 'hi'and cat
> $HOME
> hiand
$HOME
```

```
bash-3.2$ << 1 << "'2'" cat
> and
> the
> 1
> $HOME
> 2
> '2'
$HOME
2
```

```
bash-3.2$ << 1 << and'then 'one cat
> and
> then
> 1
> one
> andthen one
one
```

```
bash-3.2$ << 1 << "'2'" cat
> and
> then
> 1
> $HOME
> one
>
```

```
bash-3.2$ << 1 << and'then 'one cat
> and
> then
> 1
> $HOME
> andthen one
$HOME
```

## heredoc

```
minishell$ hey |
```

shouldn't segfault.

```
bash-3.2$ <<
bash: syntax error near unexpected token `newline'
bash-3.2$

```
bash-3.2$ << hi
> $HOME
> hi
bash-3.2$
```

```
bash-3.2$ << hi | ls
> $HOME
> hi
Makefile                ft_builtins2.c          ft_heredoc.o            ft_tokenize.c           minishell
README.md               ft_builtins2.o          ft_interpret.c          ft_tokenize.o           minishell.h
a.out                   ft_environ.c            ft_interpret.o          libft                   test
ft_builtins1.c          ft_environ.o            ft_parse_commands.o     main.c                  tests.md
ft_builtins1.o          ft_execute.o            ft_signal.o             main.o                  todo.md
```

```
bash-3.2$ touch a | << hi cat
>                           [PRESS CTRL+C HERE] 
bash-3.2$ echo $?
1
```

## echo and wc

```
bash-3.2$ echo "1" | wc -c
       2
```	   

## OLDPWD

$OLDPWD shouldn't be inherited! immediately entering minishell and running cd - should result in OLDPWD not set prompt

## Exports

```
bash-3.2$ export ll=11
bash-3.2$ export ll=11
bash-3.2$ export ll=11
```

## Recursive minishell call

```
minishell$ ./minishell
```

## Exit status and signals

`WEXITSTATUS()` macro should be used to retrieve the true exit status

if the process exits due to a signal, then the exit status must be `WTERMSIG(g_data.status) + 128`

`sleep 10 | sleep 10 | sleep 10` followed by a `Ctrl-\`

## `cd`

```
bash-3.2$ cd ../../../../../../../../../../../../../../
bash-3.2$ pwd
/
```

## Basic tests with child processes

```
bash-3.2$ export ARG=ee
bash-3.2$ echo $ARG
ee
```
```
bash-3.2$ echo blah | export ABC=abc
bash-3.2$ echo $ABC
```
```
bash-3.2$ ls | exit
```

## Heredocs with inputs from files

```
bash-3.2$ echo "hiiii hellloooo worldecho hello" > 1
bash-3.2$ <1 grep << hey hiiii
> asadadasdasd
> asdadasdasd
> hiiii THIS WAS INSIDE THE HEREDOC!!!!
> hey
hiiii THIS WAS INSIDE THE HEREDOC!!!!
```

## Heredocs with no commands

```
bash-3.2$ << v | cat
> asdasd
> asfasf
> v
```

```
bash-3.2$ << 1 << 2 | cat
> skdfj
> 1
> djkflsl
> sdkf
> 2
```

```
bash-3.2$ echo shine | << v | cat
> sdkfl
> v
```

## Several heredocs

Ctrl-D-ing duing the receipt of a multiple heredoc shouldn't quit heredoc immediately; it must skip only one heredoc
say if you have 5 heredocs then you can quit only by Ctrl-D-ing 5 times

<details>
<summary>Several heredocs</summary>
<p>

```
bash-3.2$ << 1 << 2 cat
> blah
> and
> 1
> then
> now
> 2
then
now
```

```
bash-3.2$ << 1 << 2 cat
> blah
> and
> 2
>
```

```
bash-3.2$ << 1 << 2 << 3 cat
> and
> then
> 2
> one day
> 3
> you
> 1
> find
> 2
> ten
> years
> 2
> have
> got
> behind
> 3
ten
years
2
have
got
behind
```

```
bash-3.2$ << hey cat > TEST | << hello grep hey
> djaisdaisjdasd
> hello
> asdasdasd
> hey
> now we talkin
> heyooo
> hey!!
> hello
heyooo
hey!!
```

</p>
</details>

## Piping

```
bash-3.2$ | wc
bash: syntax error near unexpected token `|'
```

```
bash-3.2$ ls | | wc
bash: syntax error near unexpected token `|'
```

## Opening files

check permissions: they should be `644`.

## ft_tokenize
The naïve approach: use space symbols as delimiters while treating several successive space symbols as one delimiter

Improvement: treat quotes: everything between them is one token
 - upon encountering a quote, try to find the closing quote after it
   - if failed, then treat this quote as a regular character
   - if successful, then treat everything between them as one token
     - `'`: just remove the quotes
     - `"`: remove the quotes while expanding variables starting with `$`

<details>
<summary><tt>'</tt> bash-inputting</summary>
<p>

```
bash-3.2$ echo '      hi there' '     and this as well'
      hi there      and this as well
```
```
bash-3.2$ touch blah'   'and'   'this.txt
bash-3.2$ ls
blah   and   this.txt
```

```
bash-3.2$ Isaac grep " a "
bash: Isaac: command not found
```

</p>
</details>

<details>
<summary><tt>"</tt> bash-inputting</summary>
<p>

```
bash-3.2$ " blah $HOME hi" blah
bash:  blah /Users/amamian hi: No such file or directory
```

</p>
</details>

`ft_tokenize` shouldn't deal with boolean operators and parentheses, it should treat `(` as invalid input

<details>
<summary>Run these tests</summary>
<p>

```
minishell$ echo''two three
minishell$ echo"$HOME "two three
minishell$     "e"cho hello
```

</p>
</details>

## How to handle boolean operators and parentheses
`minishell`'s command execution logic has to be modified so that execution of commands is made recursively based on boolean operators present in `line` provided to `minishell`.

Pseudocode of how it should look like:
```
ft_exec
	find a boolean operator
	IF no boolean operators THEN
		status = execute the command
		return status
	status = ft_exec the expression to the left of the boolean operator
	IF operator is and AND status is fail
		return status
	status = ft_exec the expression to the right of the boolean operator
	return status
```
(note: this `ft_exec` has nothing to do with `ft_exec` in `ft_interpret.c`)

Treat expressions with more than two boolean operators as follows:

```
 A  &&  B  &&  C  &&  D
(A) && (B  &&  C  &&  D)
       (B) && (C  &&  D)
	          (C) && (D)
```

### Find a boolean operator
First try to find a boolean operator outside of parentheses by skipping expressions in parentheses: `paren_num++` whenever `(` is encoutered, `paren_num--` whenever `)` is encountered; stop as soon as `paren_num` is `0` and any boolean expression is encountered





F && ((D && E) || ((A || B) && C)) || (G && H)


<details>
<summary>Some shell behavior</summary>
<p>

```
bash-3.2$ echo $HOME
/Users/amamian
```
```
bash-3.2$ echo '$HOME'
$HOME
```
```
bash-3.2$ echo "$HOME"
/Users/amamian
```
```
bash-3.2$ echo $HOMEE
bash-3.2$
```
```
bash-3.2$ echo $HOMEE'andthen'$HOME
andthen/Users/amamian
```
```
bash-3.2$ echo $HOMEE 'andthen' $HOME
andthen /Users/amamian
```
```
bash-3.2$ echo $$
23298 [current pid]
```
```
bash-3.2$ echo $
$
```
```
bash-3.2$ echo $$$
23298$
```
```
bash-3.2$ echo $$$$
2329823298
```

valid chars that env var can
                             -consist of and can begin with: upper/lowercase letters, underscores
                             -consist of                   : numbers

Double expansions do not take place:
```
bash-3.2$ tar_tar='$tur'
bash-3.2$ echo $tar_tar
$tur
```

If heredoc is non-quoted, then env vars inside of it are ALWAYS expanded, no matter whether they themselves are quoted or not:
```
bash-3.2$ cat << END
> $HOME
> END
/Users/amamian
```
```
bash-3.2$ cat << END
> "$HOME"
> END
"/Users/amamian"
```
```
bash-3.2$ cat << END
> '$HOME'
> END
'/Users/amamian'
```
</p>
</details>


<details>
<summary>Irrelevant notes</summary>
<p>

## Skip invalid inputs
Count the number of quotes, if odd then print out "Invalid input" and wait for the next input

### How to count the number of quotes
```
bash-3.2$ echo 'blah "blah'"
> 
bash-3.2$ echo 'blah "blah'
blah "blah
bash-3.2$ echo "blah 'blah"
blah 'blah
bash-3.2$ echo "blah 'blah"'
> 
bash-3.2$ 'echo'""

```
Keep track of the number of quotes with `s_quotes` and `d_quotes`. Then, if `'` (`"`) is encountered, increment `s_quotes` (`d_quotes`) and stop paying attention to `"`s (`'`s) as long as another `'` (`"`) is not encountered. If, by the time `line` is over, if the value of either of `s_quotes` and `d_quotes` is odd, then the input is invalid.

</p>
</details>

<details>
<summary>Comments to ft_refineline</summary>
<p>

```
$HOME -> /Users/arman
'$HOME' -> $HOME
"$HOME" -> /Users/arman
$HOMEE -> (nothing)
$HOMEE'andthen'$HOME -> andthen/Users/arman
$HOMEE 'andthen' $HOME -> andthen /Users/arman
$$ -> current pid (e.g. 23298)
$ -> $
$$$ -> 23298$
$$$$ -> 2329823298

valid chars that env var can
                             -consist of and can begin with: upper/lowercase letters, underscores
                             -consist of                   : numbers

Double expansions do not take place:
bash-3.2$ tar_tar='$tur'
bash-3.2$ echo $tar_tar
$tur

If heredoc is non-quoted, then env vars inside of it are ALWAYS expanded, no matter whether they themselves are quoted or not:
bash-3.2$ cat << END
> $HOME
> END
/Users/arman
bash-3.2$ cat << END
> "$HOME"
> END
"/Users/arman"
bash-3.2$ cat << END
> '$HOME'
> END
'/Users/arman'
```

</p>
</details>

<details>
<summary>Comments to ft_tokenize</summary>
<p>

```
// anything in double parentheses is not executed and its return status is set to false (e.g. `((pwd)) && ls` doesn't print out anything)
//
// execution stops as soon as the global state is true or false (e.g. `pwd || ls` only executes `pwd`, `((pwd)) && ls` doesn't print out anything)
```
</p>
</details>