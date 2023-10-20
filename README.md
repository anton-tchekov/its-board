# its-board
ITS Board Makefile and Libraries

## Creating a new project

Enter the `projects/` directory.
Use the shell script `new.sh [PROJECT-NAME]` to initialize a new project as a copy of the `sample` project.

## Upload

`make upload` to upload the `.hex` file to the connected board using ST-Link.

## Utilities

`make dump` to generate an assembly listing of the compiler
output.

`make sizes` to print the size of each symbol in .text, .rodata,
etc.

## Important

- The C standard library is not fully available
- Floating point is not supported yet
- Don't use the `.data` segment (no initialized mutable data)
