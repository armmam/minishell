# minishell
School 42 project "minishell" implementation (no bonus, only mandatory part).
## Notes
* Before compiling, please specify the correct path to your homebrew location in the Makefile's LDFLAGS and CPPFLAGS variables.
* If you want to see more in-line comments that may make it clearer for you what each function does, check out the `with-comments` branch of this repository.
  * Be aware that this branch is a bit outdated and contains some errors (concerning subtleties about `ft_exit()`, `ft_pwd()`, `ft_export()` behavior,  and expansion of non-existent environment variables).
## Known Issues
* The shell doesn't increment the `SHLVL` environment variable.
* When located in a deleted folder, `ft_cd()` aborts so `cd` doesn't work with absolute paths because `getcwd()` fails.
* `echo $_$"HOME"` prints a `$` which shouldn't be printed.
## Credits
aisraely: execution, piping, forking, signal handling, built-ins

amamian: parsing, heredoc receipt, redirections, built-ins, debugging
## Contacts
aisraely: alexanderisraelyan@gmail.com
