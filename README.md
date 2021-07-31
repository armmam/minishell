# minishell
School 42's project "minishell" implementation.
## Changelog
07/26/21:
Alexander
1) created a global structure holding the data
2) implemented basic command execution and the shell loop

07/27/21:
Alexander
1) shell history is now working
2) created an enum for dereferencing builtins

07/28/21:
Alexander
1) a rough draft of piping and executing was implemented

07/29/21:
Alexander
1) implemented `cd`
2) `env` now doesn't print uninitialized environment variables

07/30/21:
Alexander
1) ton of compiler-time errors fixes
2) heredoc implementation
3) better forking for built-ins