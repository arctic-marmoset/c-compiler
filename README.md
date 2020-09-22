# C Compiler

A compiler for a stripped-down version of ANSI C.

Currently, the compiler is able to:
- Open a source file passed as a command-line argument
- Tokenise the source file by recursively matching character sequences
- Generate an intermediate representation (IR)—currently an abstract syntax tree—by recursively matching token sequences with rules defined by the C grammar (as in an LR(1) parser)
- Keep track of function and variable declaration scope using a linked list of symbol tables
  - Shadow variables previously defined in parent scope

Statements and expressions that have been implemented include:
- Binary expressions
  - '+' operator
  - '=' operator
- Variable declarations
- Function declarations
- Compound statements

## Usage

DEBUG MODE: interactive console (REPL).

RELEASE MODE: command-line executable that takes a filename as a parameter.

`> ccompiler.exe source.cpp`
