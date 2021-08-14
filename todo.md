`minishell`'s command execution logic has to modified so that execution of commands is made recursively based on boolean operators present in `line` provided to `minishell`.

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

`find a boolean operator`: first try to find a boolean operator outside of parentheses by skipping expressions in parentheses: `paren_num++` whenever `(` is encoutered, `paren_num--` whenever `)` is encountered; stop as soon as `paren_num` is `0` and any boolean expression is encountered





F && ((D && E) || ((A || B) && C)) || (G && H)
