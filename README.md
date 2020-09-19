# Custom Compiler

Includes both front and back-end code generation. This lexigraphic parser compiles down to various object representations, which creates Intermediate Representation code (IR). Optimizations passes are run to generate improved IR. Ex. include Single Static Assignment (SSA), Abstract Syntax Tree (AST), and more. After the IR is fed into the [DLX architecture](https://github.com/HelmetBro/compilerVM) to generate Opcode.

*	Features advanced arithmetic with arrays, properly parses functions, all loops, assignments, nested if-statements, recursion, and more!
* Built in Graphviz API to print a visual representation of IR Basic Block, before and after every pass!

Take a look at a visual representation of the symbol-by-symbol recursive parser (front end, code -> AST):

![Front-end parse](https://github.com/HelmetBro/custom_compiler/blob/master/ParserDiagram.png)

While there are no ~beautiful~ graphs like the one above, I urge you to take a look at the Graphviz integration and see it work it's magic. SSA is my favorite part!
