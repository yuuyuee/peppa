# Peppa Design

## Peppa compiler

* Lexer
* Parser
* Symbols table
* Constant table
* Bytecodes
* Object
* Intermedia representation
  * suffix-style
    * inverse polish representation
  * graph-style
    * abstrct syntax tree
    * directed acyclic graph
  * three address code
    * ternary equation
    * indirect ternary equation
    * quaternion equation
* Optimization design

### Progress

1. scan -> token
2. parse -> abstract syntax tree
3. optimize -> three address code, symbol table, constant table

## Peppa Virtual Machine (PVM)

* built-in function or standard library function
* State
* JIT

### Progress

1. create state
2. load builtin-function and standard library function
3. load three address code, symbol table, constant table
4. execute the entry function
