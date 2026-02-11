# 42 Minishell

A lightweight Unix shell implementation in C, inspired by Bash. This project recreates core shell functionalities including command execution, pipelines, redirections, environment variables, and built-in commands.

## 📋 Description

**Minishell** is a 42 School project that challenges students to create their own shell. It demonstrates deep understanding of:

- Process creation and management (`fork`, `execve`, `wait`)
- File descriptor manipulation and redirections
- Signal handling (SIGINT, SIGQUIT)
- Parsing and tokenization (lexer + AST)
- Environment variable management
- Built-in command implementation

This shell provides a command-line interface similar to bash, with support for complex command pipelines, input/output redirections, and heredocs.

**🤝 This project was completed in pair programming (100% peer programming) with [@RVTEL-git](https://github.com/RVTEL-git) (Barnabé).**

## 🚀 Features

### Core Functionality
- **Interactive prompt** with command history (using GNU Readline)
- **Command execution** with PATH resolution
- **Pipelines** (`|`) for chaining commands
- **Redirections**:
  - Input redirection (`<`)
  - Output redirection (`>`)
  - Append mode (`>>`)
  - Heredoc (`<<`)
- **Environment variables** with `$` expansion
- **Exit status** tracking (`$?`)
- **Quote handling** (single `'` and double `"` quotes)
- **Signal handling** (Ctrl-C, Ctrl-D, Ctrl-\)

### Built-in Commands
- `echo` (with `-n` option)
- `cd` (change directory with relative/absolute path)
- `pwd` (print working directory)
- `export` (set environment variables)
- `unset` (remove environment variables)
- `env` (display environment variables)
- `exit` (exit the shell)

## 📁 Project Structure

```
.
├── Makefile                    # Build configuration
├── includes/
│   ├── minishell.h            # Main header file
│   └── structs.h              # Data structures definitions
├── libft/                     # Custom C library
└── sources/
    ├── main.c                 # Entry point
    ├── builtin/               # Built-in commands implementation
    │   ├── builtin_cd.c
    │   ├── builtin_echo.c
    │   ├── builtin_env.c
    │   ├── builtin_exit.c
    │   ├── builtin_export.c
    │   ├── builtin_pwd.c
    │   └── builtin_unset.c
    ├── exec/                  # Execution engine
    │   ├── exec_ast.c         # AST traversal and execution
    │   ├── exec_builtins.c    # Built-in execution
    │   ├── exec_cmd.c         # Command execution
    │   ├── exec_shell.c       # Main shell loop
    │   └── exec_redirs.c      # Redirections handling
    ├── lexer/                 # Tokenization
    │   ├── lexer.c
    │   └── token.c
    ├── parsing/               # AST construction
    │   ├── parsing_pipe.c
    │   ├── parsing_cmd.c
    │   └── parsing_redir.c
    ├── signals/               # Signal management
    └── structs/               # Memory management
        ├── structs_init.c
        └── free_structs.c
```

## 🔧 Installation

### Prerequisites
- GCC or Clang compiler
- GNU Readline library
- Make

On Ubuntu/Debian:
```bash
sudo apt-get install libreadline-dev
```

On macOS (using Homebrew):
```bash
brew install readline
```

### Compilation

Clone the repository and compile:
```bash
git clone https://github.com/clothildesc/42-minishell.git
cd 42-minishell
make
```

This will generate the `minishell` executable.

### Make Commands
- `make` or `make all` - Compile the project
- `make clean` - Remove object files
- `make fclean` - Remove object files and executable
- `make re` - Recompile everything from scratch

## 💻 Usage

### Starting the Shell

```bash
./minishell
```

You'll see the interactive prompt:
```
minishell>
```

### Basic Examples

**Simple commands:**
```bash
minishell> ls -la
minishell> pwd
minishell> echo "Hello, World!"
```

**Pipelines:**
```bash
minishell> ls -l | grep minishell | wc -l
minishell> cat file.txt | grep "pattern" | sort
```

**Redirections:**
```bash
minishell> echo "text" > output.txt
minishell> cat < input.txt
minishell> ls -l >> logfile.txt
```

**Heredoc:**
```bash
minishell> cat << EOF
> This is a heredoc
> Multiple lines supported
> EOF
```

**Environment variables:**
```bash
minishell> export MY_VAR="Hello"
minishell> echo $MY_VAR
minishell> echo $PATH
minishell> echo $?
```

**Quotes:**
```bash
minishell> echo "Variable: $USER"    # Expands $USER
minishell> echo 'Variable: $USER'    # Literal string
```

**Built-in commands:**
```bash
minishell> cd /tmp
minishell> pwd
minishell> env
minishell> export PATH="/usr/bin:$PATH"
minishell> unset MY_VAR
minishell> exit
```

## 🏗️ Architecture

### 1. Lexer (Tokenization)
Converts raw input string into tokens:
- Handles quotes (single and double)
- Identifies operators (`|`, `<`, `>`, `>>`, `<<`)
- Separates commands and arguments
- Marks tokens for expansion

### 2. Parser (AST Construction)
Builds an Abstract Syntax Tree from tokens:
- **NODE_PIPE**: Pipeline nodes (binary tree)
- **NODE_CMD**: Command nodes (leaf nodes)
- Handles redirections and arguments

### 3. Expander
Processes `$` variable expansion:
- Environment variables (`$PATH`, `$USER`)
- Exit status (`$?`)
- Handles quotes properly

### 4. Executor
Traverses the AST and executes:
- **Pipelines**: Creates pipes and forks processes
- **Built-ins**: Executes in parent or child process
- **External commands**: Resolves PATH and calls `execve`
- **Redirections**: Manipulates file descriptors with `dup2`

### 5. Signal Handler
Manages signals:
- **SIGINT (Ctrl-C)**: Interrupts current command
- **SIGQUIT (Ctrl-\)**: Ignored in interactive mode
- **EOF (Ctrl-D)**: Exits the shell

## 🧪 Testing

Test with various scenarios:

**Basic commands:**
```bash
echo test
ls
pwd
```

**Complex pipelines:**
```bash
cat file | grep pattern | sort | uniq | wc -l
```

**Multiple redirections:**
```bash
< input.txt cat | grep test > output.txt
```

**Edge cases:**
```bash
echo "unclosed quote
cat < nonexistent_file
ls | | grep    # Syntax error
exit 42
```

## 📝 Exit Status Codes

- `0` - Success
- `1` - General error
- `2` - Syntax error
- `126` - Permission denied (command not executable)
- `127` - Command not found
- `130` - Terminated by Ctrl-C (SIGINT)
- `131` - Terminated by Ctrl-D (EOF)

## 🔍 Key Concepts

### Process Management
- `fork()` - Creates child processes
- `execve()` - Replaces process with new program
- `wait()/waitpid()` - Waits for child process completion

### File Descriptors
- `dup2()` - Duplicates file descriptors for redirections
- `pipe()` - Creates inter-process communication channels

### Signal Handling
- Custom signal handlers for interactive behavior
- Different handling in parent vs child processes

## ⚠️ Known Limitations

This shell is a simplified version and does not support:
- Logical operators (`&&`, `||`)
- Wildcards (`*`, `?`)
- Subshells `()`
- Command substitution `` `cmd` `` or `$(cmd)`
- Job control (`bg`, `fg`, `jobs`)
- Aliases
- Shell scripting (must be used interactively)

## 🎓 Learning Outcomes

This project teaches:
- ✅ Deep understanding of Unix processes
- ✅ File descriptor manipulation
- ✅ Parsing and lexical analysis
- ✅ Abstract Syntax Tree construction
- ✅ Memory management in complex programs
- ✅ Signal handling in interactive applications
- ✅ **Collaboration and pair programming best practices**

## 👥 Authors

This project was developed in **100% pair programming** by:

**Clothilde Scache** (cscache)
- GitHub: [@clothildesc](https://github.com/clothildesc)

**Barnabé Martinez** (barmarti)
- GitHub: [@RVTEL-git](https://github.com/RVTEL-git)

## 📄 License

This project is part of the 42 School curriculum.

---

*Built with 🤝 at 42 School*