# Usage

**Clone repo:**

    git clone https://github.com/anton-tchekov/unholy-c.git

**Regular build:**

    make

**Clean build:**

    make clean all

**Run:**

    ./nano-c [file.uhc]

# Unholy C Language Documentation

![Logo](logo.png)

## Source File Extension

`.uhc` for obvious reasons.

## Example Code

See `examples/` directory for code examples.

- Conway's Game of Life: `life.uhc`

## Variables

Local variables can be only be declared at the beginning of a function with the `let`
keyword. Every local variable is 32-bit in size. Variables can not be initialized
during declaration.

Variable names can only contain with letters, numbers or underscores.
Additionally, the first character can not be a number.

**Example:**

    fn test()
    {
        let value, number;
        let count;
    }

Global variables and constants must be defined at the beginning of the file,
before all functions.

**Example:**

    let _counter;
    const ANSWER = 42;
    const PI = 3.1416;

## Types

There are **no** types (and no type safety):
The meaning of the bits in a variable is defined only by what
**you** (the programmer) do with it.

Doing `var1 = var2` simply copies the bits from var2 into var1.

However, there are typed literals:

- Integer:
	- Decimal: `-25`, `0`, `1`, `77`, etc.
	- Hexadecimal: `0xFF`, `0xAA`, etc. [note: small x, letters must be uppercase]
	- Binary: `0b1010`, etc. [note: small b]

- Floating Point: `-42.57`, `3.14`, etc.

- Boolean: `true`, `false`

- Character: `'A'`, `'7'`, `'\n'`, `'\\'`, `'!'`, etc.

- String (char array): `"Hello World!"`

**Note:** The string will be placed in the programs readonly data area. In an
expression, it will evaluate to a pointer to the first character of the string.
The string is terminated by a `'\0'` character.

## Comments

Both multi-line and single-line comments are supported:

    /* This is a comment */

    // This is a comment too

## Expressions

Expressions (used for example on the right hand side of an assignment)
consist only out of function calls.

**Example:**

	/* y = x^2 + x + 1 */
    y = add(add(mul(x, x), x), 1);

## Built-in Functions

### Basic Integer Arithmetic
| C Operator | Function            |
|------------|---------------------|
| `+`        | `int add(int, int)` |
| `-`        | `int sub(int, int)` |
| `*`        | `int mul(int, int)` |
| `/`        | `int div(int, int)` |
| `%`        | `int mod(int, int)` |
|            | `int abs(int, int)` |

### Equality
| C Operator | Function            | Meaning   |
|------------|---------------------|-----------|
| `==`       | `bool eq(any, any)` | Equal     |
| `!=`       | `bool ne(any, any)` | Not Equal |

### Integer Comparison
There are two sets of integer comparison functions for signed
and unsigned numbers.

### Unsigned
| C Operator | Function              | Meaning          |
|------------|-----------------------|------------------|
| `>`        | `bool hi(uint, uint)` | Higher           |
| `>=`       | `bool hs(uint, uint)` | Higher or Same   |
| `<=`       | `bool ls(uint, uint)` | Lower or Same    |
| `<`        | `bool lo(uint, uint)` | Lower            |

### Signed
| C Operator | Function            | Meaning          |
|------------|---------------------|------------------|
| `>`        | `bool gt(int, int)` | Greater Than     |
| `>=`       | `bool ge(int, int)` | Greater or Equal |
| `<=`       | `bool le(int, int)` | Less or Equal    |
| `<`        | `bool lt(int, int)` | Less Than        |

### Bitwise operators
| C Operator | Function               | Meaning              |
|------------|------------------------|----------------------|
| `\|`       | `uint or(uint, uint)`  | Bitwise OR           |
| `&`        | `uint and(uint, uint)` | Bitwise AND          |
| `~`        | `uint inv(uint, uint)` | Bitwise NOT          |
| `^`        | `uint xor(uint, uint)` | Bitwise Exclusive OR |
| `<<`       | `uint shl(uint, uint)` | Bitwise Shift Left   |
| `>>`       | `uint shr(uint, uint)` | Bitwise Shift Right  |

### Boolean operators
| C Operator | Function         | Meaning                                  |
|------------|------------------|------------------------------------------|
| `!`        | `bool not(bool)` | If argument 0, returns 1, else returns 0 |

### Floating Point
| C Operator | Function                   | Meaning          |
|------------|----------------------------|------------------|
|            | `float itf(int)`           | Int to float     |
|            | `int fti(float)`           | Float to int     |
| `+`        | `float addf(float, float)` | Add              |
| `-`        | `float subf(float float)`  | Subtract         |
| `*`        | `float mulf(float, float)` | Multiply         |
| `/`        | `float divf(float, float)` | Divide           |
|            | `float modf(float, float)` | Modulo           |
| `>`        | `bool gtf(float, float)`   | Greater Than     |
| `>=`       | `bool gef(float, float)`   | Greater or Equal |
| `<=`       | `bool lef(float, float)`   | Less or Equal    |
| `<`        | `bool ltf(float, float)`   | Less Than        |
|            | `float absf(float)`        | Absolute value   |
|            | `int ceil(float)`          | Ceiling function |
|            | `int floor(float)`         | Floor function   |
|            | `int round(float)`         | Round function   |

### Math
Exactly the same as in `<math.h>`

| Function                    |
|-----------------------------|
| `float sin(float)`          |
| `float cos(float)`          |
| `float tan(float)`          |
| `float asin(float)`         |
| `float acos(float)`         |
| `float atan(float)`         |
| `float atan2(float, float)` |
| `float sinh(float)`         |
| `float cosh(float)`         |
| `float tanh(float)`         |
| `float exp(float)`          |
| `float log(float)`          |
| `float log10(float)`        |
| `float pow(float)`          |
| `float sqrt(float)`         |

### Characters
| Function              | Meaning                            | true for                                      |
|-----------------------|------------------------------------|-----------------------------------------------|
| `bool isupper(char)`  | Is uppercase?                      | `A-Z`                                         |
| `bool islower(char)`  | Is lowercase?                      | `a-z`                                         |
| `bool isdigit(char)`  | Is digit?                          | `0-9`                                         |
| `bool isalpha(char)`  | Is in alphabet?                    | `a-z`, `A-Z`                                  |
| `bool isalnum(char)`  | Is alphanumeric?                   | `a-z`, `A-Z`, `0-9`                           |
| `bool iscntrl(char)`  | Is control character?              | ASCII `0-31` and `127`                        |
| `bool isgraph(char)`  | Is graphical?                      | ASCII `33-126`                                |
| `bool isprint(char)`  | Is printable?                      | ASCII `32-126` (Opposite of iscntrl)          |
| `bool ispunct(char)`  | Is punctuation?                    | ``!"#$%&'()*+,-./:;<=>?@[\]^_`{\|}~``         |
| `bool isspace(char)`  | Is whitespace?                     | `' '`, `'\n'`, `'\t'`, `'\v'`, `'\f'`, `'\r'` |
| `bool isxdigit(char)` | Is hexadecimal?                    | `0-9`, `a-f`, `A-F`                           |
| `char tolower(char)`  | If uppercase, convert to lowercase |                                               |
| `char toupper(char)`  | If lowercase, convert to uppercase |                                               |

### Random
| Function          | Meaning       |
|-------------------|---------------|
| `int rand(void)`  | Random Number |
| `void srand(int)` | Set RNG Seed  |

### Timer
| Function            | Meaning                            |
|---------------------|------------------------------------|
| `uint millis(void)` | Milliseconds since program started |

### Memory
| Function | Meaning                                 |
|----------|-----------------------------------------|
| `void w32(addr, value32)`    | Store 32-bit value at address           |
| `void w16(addr, value16)`    | Store 16-bit value at address           |
| `void w8(addr, value8)`     | Store 8-bit value at address            |
| `uint r32`    | Load 32-bit value from address          |
| `uint ru16`   | Load unsigned 16-bit value from address |
| `uint ru8`    | Load unsigned 8-bit value from address  |
| `int rs16`   | Load signed 16-bit value from address   |
| `int rs8`    | Load signed 8-bit value from address    |
| `memcpy()`   | memmove                                 |
| `int memcmp()`   | memcmp                                  |
| `memchr()`   | memchr                                  |
| `void memset(addr, value, len)`   | memset                                  |

### File/Stream
| Function | Meaning                                                |
|----------|--------------------------------------------------------|
| `char fgetc(void)`  |                                                        |
| `void fputc(char c)`  |                                                        |
| `void fputs`  | Print string (null-terminated)                         |
| `void fputse` | Print string (with length)                             |
| `void fputd(int d)`  | Print decimal                                          |
| `void fputde(int d, int width)` | Print decimal extended (left-pad with spaces)          |
| `void fputx(uint x)`  | Print hexadecimal                                      |
| `void fputxe(uint x, int width)` | Print hexadecimal extended (left-pad with zeros)       |
| `void fputf(float f)`  | Print float                                            |
| `void fputfe` | Print float extended (left-pad with spaces, precision) |


| `void fopen(string file, string mode)`  |                                                        |
| `void fread`  |                                                        |
| `void fclose` |                                                        |
| `fwrite` |                                                        |
| `fseek`  |                                                        |
| `ftell`  |                                                        |
| `fcopy`  | Copy file                                              |
| `fmove`  | Move/rename file                                       |
| `fdel`   | Delete file                                            |

## Functions

**Example:**

    fn function_name(param0, param1, param2)
    {
        return add(mul(param0, param1), param2);
    }

All functions have a 32-bit return value.
If there return statement is empty (`return;`) or does not exist,
the implicit default return value is 0.

The program starts at the `main` function.

## Control Flow

Curly braces are required for all control flow contstructs.

### Loops

There are four types of loops:

- **counting**

```
for c = 'A' to 'Z' {
    fputc(0, c);
}

for i = 0 to 100 inc 5 {
    fputd(0, i);
    fputc(0, '\n');
}
```

**Note:** for loops only work with signed integers.

- **top-controlled**

```
i = 1;
while le(i, 10) {
    fputd(0, i);
    fputc(0, '\n');
    i = add(i, 1);
}
```

- **bottom-controlled**

```
do {
    c = get_char();
} while ne(c, '\n');
```

- **infinite**

```
loop {
    /* Main Loop */
}
```

`break;` and `continue;` statements are supported. There is no `goto`.

### if-elif-else Branch

**Example:**

    if lt(number, 10) {
        fputs(0, "Your number is less than 10\n");
    }
    elif eq(number, 42) {
        fputs(0, "Good choice!\n");
    }
    else {
        fputs(0, "Your number ");
        fputd(0, number);
        fputc(0, '\n');
    }

# Development

## Design Decisions

There are some unusual design decisions that I am going to explain here:

**1.** `memory_XX` functions:

One of the goals for this project to be able to be run on an AVR with external
memory (23LC1024). Since the chips have very little internal RAM (eg. 2048
bytes for an ATMega328, the one on an Arduino UNO), I am using these functions
to access the external memory. When compiling for an OS target, the external
memory is simulated by an equally sized, flat u8-array.

**2.** In some places, the Tokenizer does things that are actually the job of
the parser, (best example: string literals)

It makes the code shorter, more efficient and easier to understand by eliminating
unneccessary passing around of data.

**3.** (Excessive?) Use of macros: `NANOC_PROPAGATE`, `NANOC_THROW`,
`NANOC_EXPECT`, etc.

I am using macros mainly to make error handling easier.
The parser uses recursion to handle expressions, nested blocks, etc.
When there is an error, it needs to be propagated up to
where it can be handled.

`NANOC_THROW` "throws" an error, by returning the error code. In debug mode,
also prints file, function and line number to make finding bugs easier.

`NANOC_PROPAGATE`
If the called function returns 0 (success), NANOC_PROPAGATE does nothing.
If the called function returns a negative status code (error),
NANOC_PROPAGATE returns the return value

Exceptions in C, yay!

`NANOC_EXPECT` is used to shorten the commonly needed:

    if(_token.Type != SOME_TOKEN_TYPE)
    {
        NANOC_THROW(ERROR_EXPECTED_SOME_TOKEN_TYPE);
    }

`error.c` uses macros to generate the error messages automatically.

**4.** Including `.c` files

By including all other C files into one single file, the whole program is in one
translation unit, which enables the C compiler to make much better optimizations.
This is further improved by making all functions `static`, which tells the compiler
that the function cannot be called externally.

This is essentially my adaptation of the "[SQLite Amalgamation](https://www.sqlite.org/amalgamation.html)"

Quote from the link:

> Combining all the code for SQLite into one big file makes SQLite easier to
> deploy - there is just one file to keep track of. And because all code is in a
> single translation unit, compilers can do better inter-procedure and inlining
> optimization resulting in machine code that is between 5% and 10% faster.

## Code Overview

**Directories:**

- `src/`: NanoC Compiler and Interpreter

- `platform/`: Platform specific code
    - `simulator/`
    - `avr/`

- `lib/`: Library code

- `apps/`: UnholyC Apps
    - `editor/`: Code and text editor
    - `test/`: For Testing purposes

- `examples/`: UnholyC Examples

**Utility:**

- `types.h`: Contains integer type definitions and platform dependant

- `error.c`: Error handling macros and error definitions

`NANOC_THROW` to throw a previously defined error\
`NANOC_PROPAGATE` to propagate the error\

- `util.c`: Misc. utility functions

`_string_find` is used to detect keywords and builtin functions
by finding the position of a string in another, delimiter separated string.

- `instr.c`: Bytecode instruction definitions (e.g. `INSTR_CALL`)

- `token.c`: Token type and keyword definitions like `TT_IF`, `TT_WHILE`, etc.

**Main:**

- `builtin.c`: Contains builtin functions like `mul`, `sin`, `isupper`, etc.

- `tokenizer.c`: Extracts the next token from the source file
(keyword, identifier, literal, etc.) and passes it to the parser.

- `parser.c`: Predictive recursive descent parser. Requests tokens from
the Tokenizer and outputs bytecode instructions.

An IdentifierMap stores the locations of identifiers
like function and variable names in the source code.

An AddressStack is used to handle break and continue statements.

It is needed because the parser reads the file from start to finish in
a single pass. So when a `break;` is encountered, at that point the
parser does not know where the end of the loop is going to be.
So the parser leaves a gap in the bytecode, and stores the location
of the gap in the AddressStack. When the end of the loop is reached,
the gap can be filled in with an unconditional jump to the current
output address.

The same technique is also utilized with function calls, when a function
is used before the parser has seen it's implementation.

At this point `optimizer.c` might be added to perform simple
optimizations like constant folding or inlining.

Generating actual machine code could also be an interesting project,
however the current aim is to have a sort of scripting language for
embedded systems with Harvard ISA that can only execute code from
their flash memory so the interpreter part is to work around that.

- `interpreter.c`: Runs the bytecode

The instruction set only needs 13 instructions.
It uses two stacks, the regular call stack and an operand stack.
I am still looking for a clean way to remove the need for
two stacks and the parameter copying when calling a function.

**Driver:**

- `main.c`: Used for Testing/driving the main code

## TODO

- compile time constants
- array constants data definitions
- re-implement strings
