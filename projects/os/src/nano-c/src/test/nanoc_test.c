#ifdef NANOC_DESKTOP

#include "nanoc_test.h"

#include "nanoc_parser.h"
#include "nanoc_expression.h"
#include "nanoc_disasm.h"
#include "nanoc_debug.h"
#include "nanoc_instruction.h"
#include "nanoc_interpreter.h"
#include "nanoc_builtin.h"
#include "nanoc_util.h"

#include "types.h"
#include <stdio.h>

static i32r output;

static i32r test(i32r a, i32 *p)
{
	output = p[0];
	return 0;
}

static int test_run(const char *source, int expected)
{
	int ret;
	NanoC_Parser parser;
	u8 output_buf[1024];
	i32r (*functions[])(i32r, i32 *) = { test };
	NanoC_Builtins builtins =
	{
		1,
		functions
	};

	nanoc_parser_init(&parser, source, output_buf, sizeof(output_buf));
	ret = nanoc_statement(&parser);
	if(ret)
	{
		printf("parse error: %s\n", nanoc_status_message(ret));
		return 0;
	}

	nanoc_output_emit(&parser.Output, NANOC_INSTR_HALT);
	nanoc_disasm(parser.Output.Buffer, parser.Output.Pos);
	ret = nanoc_interpreter_run(parser.Output.Buffer, &builtins);
	if(ret)
	{
		printf("Interpreter error: %d\n", ret);
		return 0;
	}

	if(output != expected)
	{
		printf("expected %d got %d\n", expected, output);
		return 0;
	}

	return 1;
}

void nanoc_test_run(void)
{
	struct
	{
		const char *Source;
		int Expected;
	}
	tests[] =
	{
		/* Simple Arithmetic and Logic */
		{ "test(5 + 5);", 10 },
		{ "test(5 + 5 * 7);", 40 },
		{ "test(1 << 16);", 65536 },
		{ "test((1 + 1) * 4);", 8 },
		{ "test(-7 * -9);", 63 },
		{ "test(0x55 | 0xAA);", 0xFF },
		{ "test(17 - 18);", -1 },
		{ "test(99 - 98 + 1);", 2 },
		{ "test(9 >> 2);", 2 },
		{ "test(!5);", 0 },
		{ "test(!(1 - 1));", 1 },
		{ "test((1 | 0) && !1);", 0 },
		{ "test(-5 / -1);", 5 },
		{ "test(5 == 3 + 2);", 1 },
		{ "test(1 && 1 && 1 && 1 && 1);", 1 },

		{ "test(0 || 0);", 0 },
		{ "test(0 || 1);", 1 },
		{ "test(1 || 0);", 1 },
		{ "test(1 || 1);", 1 },

		{ "test(0 && 0);", 0 },
		{ "test(0 && 1);", 0 },
		{ "test(1 && 0);", 0 },
		{ "test(1 && 1);", 1 },

		{ "test(2+(5*8)-9*(3-1)/6);", 39},
		{ "test(7+1*9-0+43*89*3);", 11497},

		{ "test(0);", 0 },
		{ "test(3 - 3);", 0 },
		{ "{let x; x = 4; test(x - 4);}", 0 },
		{ "{let x; x = 50; while(x) { x = x - 1; } test(x);}", 0 },
		{ "{let x; x = 50; do { x = x - 1; } while(x); test(x); }", 0 },
		{ "let x; x = 1; x = x * 10; x = x / 2; x = x % 3; test(x - 2);", 0 },
		{ "test((2 + 2) * 2 - 8);", 0},
		{ "{let x; x = 1; x = x | 4; test(x - 5); }", 0 },
		{ "{let x; x = 1; x = x & 3; test(x - 1); }", 0 },
		{ "{let x; x = 1; x = x ^ 3; test(x - 2); }", 0 },

		{ "{ let n = 2, t, c = 0, p;"
		"while (n < 5000) {"
			"t = 2;"
			"p = 1;"
			"while (t*t <= n) {"
				"if (n % t == 0)"
					"{ p = 0; }"
				"++t;"
			"}"
			"++n;"
			"if (p)"
				"{ ++c; }"
		"}"
		"test(c); }", 669 },

		// 101
		{ "{let c=0; do {} while(0); test(c); }", 0},

		// 102
		{ "{let x, y = 0; x = 1; if ((x << 1) != 2) {y = 1;} test(y);}", 0 },

		// 126
		{ "{ let x; x = 3; x = !x; x = !x; x = ~x; x = -x; test(x); }", 2 },

		// 127
		{ "{ let x=0, c = 0;"
			"if(0) { x =1; }"
			"else if(0) { }"
			"else {"
			"    if(1) {"
			"        if(c)"
			"            { x =1; }"
			"        else"
			"            { x =0; }"
			"    } else {"
			"        x =1;"
			"    }"
			"}"
			"test(x); }", 0 },


		{ "test(100 < 1);", 0 },

		/* If */
		{ "{let i=5; if(i == 5) { ++i; } test(i); }", 6 },

		/* Loops */
		{ "{let i=0,q=0; while(i < 100) { ++i; q += i; } test(q); }", 5050 },

		/* Break and continue! */

		/* For Loops */

		{ "test(5+);", 0 },
		{ "test(90;", 0 },
		{ "test(1)", 0 },
		{ "{if[1) {} }", 0 },
		{ "{if(1 {} }", 0 },
		{ "{if() {} }", 0 },
		{ "{if {} }", 0 },
		{ "{if(()) {} }", 0 },
		{ "{if(-()) {} }", 0 },
		{ "{if(1) { }", 0 },
		{ "{if(1) { break; }}", 0 },
		{ "{if(1) { continue; }}", 0 },
		{ "{while(1) { break }}", 0 },
		{ "{while(0) { continue }}", 0 },
		{ "blub; ", 0},
		{ "x = 0; ", 0},
		{ "let a = ; ", 0},
		{ "let b, a,; ", 0},
		{ "let b, a, c,", 0},
		{ "{ let a, c; a = 0 c = 6; }", 0},
		{ "{ let a, c; a = 0; c = 6 }", 0},
		{ "{ let a += 1; }", 0},
	};

	size_t i;
	int success = 0;
	for(i = 0; i < ARRLEN(tests); ++i)
	{
		int ret = test_run(tests[i].Source, tests[i].Expected);
		if(!ret)
		{
			printf("Test %ld failed!\n%s\n\n", i, tests[i].Source);
		}

		success += ret;
	}

	printf("%d/%ld tests succeeded\n", success, ARRLEN(tests));
}

#endif /* NANOC_DESKTOP */
