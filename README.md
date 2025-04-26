# minishell

**A large-scale project aiming to recreate a basic shell, handling command execution, piping, redirections, built-in commands, and environment variable management.**

## Objectives
- Parse and execute shell commands with support for redirections (`<`, `>`, `>>`, `<<`) and piping (`|`).
- Implement built-in commands (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`) according to Bash behavior.
- Manage environment variables and variable expansion (`$VAR`, `$?`).
- Handle signals gracefully (`CTRL+C`, `CTRL+D`, `CTRL+\`) to mimic real shell behavior.
- Ensure proper memory management and protect against resource leaks.

## Skills developed
- Advanced parsing techniques (tokenization, syntax validation, AST construction if applicable).
- Deep understanding of Unix system calls: `fork`, `execve`, `pipe`, `dup2`, `signal`, etc.
- Managing multiple processes, file descriptors, and memory in a complex environment.
- Writing a highly modular, robust, and norm-compliant C application simulating a real-world shell.
