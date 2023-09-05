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
#include <assert.h>

static i32r output;

static i32r test(i32r a, i32 *p)
{
	output = p[0];
	return 0;
}

static int test_positive_run(const char *source, int expected)
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
	ret = nanoc_interpreter_run(parser.Output.Buffer, &builtins);
	if(ret)
	{
		printf("Interpreter error: %s\n",
			nanoc_interpreter_status_message(ret));
		return 0;
	}

	if(output != expected)
	{
		printf("expected %d got %d\n", expected, output);
		return 0;
	}

	return 1;
}

static int test_negative_run(const char *source)
{
	NanoC_Parser parser;
	u8 output_buf[1024];
	nanoc_parser_init(&parser, source, output_buf, sizeof(output_buf));
	return nanoc_statement(&parser);
}

void nanoc_test_run(void)
{
	typedef struct
	{
		const char *Source;
		int Expected;
	} TestCase;

	static const TestCase tests[] =
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

		{ "test('\\x55');", '\x55' },
		{ "test('\\x7a');", '\x7a' },
		{ "test('A');", 65 },
		{ "test('\\t');", '\t' },
		{ "test('\\0');", '\0' },
		{ "test('\\n');", '\n' },
		{ "test(0b1010);", 10 },
		{ "test(0b11111);", 31 },
		{ "test(0644);", 420 },
		{ "test(0755);", 493 },
		{ "test(0777);", 511 },
		{ "{ let/* comment */i =/*comment*/79; test(i); }", 79 },
		{ "{ let i = 0; // i = 10;\ni = 5; test(i); }", 5},

		{ "test(2+(5*8)-9*(3-1)/6);", 39},
		{ "test(7+1*9-0+43*89*3);", 11497},

		{ "test(5 >= 5);", 1 },
		{ "test(1 >= -1);", 1 },
		{ "test(3 >= 9);", 0 },
		{ "test(-5 >= -9);", 1 },

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

		/* 101 */
		{ "{let c=0; do {} while(0); test(c); }", 0},

		/* 102 */
		{ "{let x, y = 0; x = 1; if ((x << 1) != 2) {y = 1;} test(y);}", 0 },

		/* 126 */
		{ "{ let x; x = 3; x = !x; x = !x; x = ~x; x = -x; test(x); }", 2 },

		/* 127 */
		{ "{ let x=0, c = 0;"
			"if(0) { x = 1; }"
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

		{ "{let i = 5; i*=i;i*=2;i+=7;test(i);}", 57},

		/* Loops */
		{ "{let i=0,q=0; while(i < 100) { ++i; q += i; } test(q); }", 5050 },

		/* Break and continue! */
		{ "{let i=0;while(i<10){if(i==5){break;}++i;} test(i);}", 5 },
		{ "{let i,n=14512891;while(i<=n/2){if(n%i==0){test(i);break;}++i;}}", 2371 },
		{
			"{\n"
			"    let count = 0, i = 2;\n"
			"    while(i < 10000) {\n"
			"        let j = 2, isprime = 1;\n"
			"        while(j <= i / 2) {\n"
			"            if(!(i % j)) {\n"
			"                isprime = 0;\n"
			"                break;\n"
			"            }\n"
			"            ++j;\n"
			"        }\n"
			"        if(isprime) { ++count; }\n"
			"        ++i;\n"
			"    }\n"
			"    test(count);\n"
			"}\n",
			1229 },

		{ "{ let i = 42;do { ++i;break; ++i; } while(1); test(i); }", 43},
		{ "{ let i = 0; loop { ++i; if(i == 10) {break;} } test(i); }", 10 },
		{ "{let i = 0, n = 0; while(i < 50) { if(i%3==0){++i;continue;} ++n;++i;} test(n); }", 33},
		{ "{let counter = 100; while(counter > 5) { --counter; } test(counter);}", 5 },

	};

	static const char *tofail[] =
	{
		"test('\\xHG');",
		"test('\\k');",
		"let a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,\n"
			"aa,bb,cc,dd,ee,ff,gg,hh,ii,jj,kk,ll,mm,nn,oo;",
		"test(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20);",
		"{ le/* comment */t i; }",
		"{ let i, i = 0; }",
		"++bla;",
		"?",
		"{ let i = 0; if(i == 1) { let i = 6; } }",
		"test(bla); ",
		"test(5+);",
		"test(90;",
		"test(1)",
		"{if[1) {} }",
		"{if(1 {} }",
		"{if() {} }",
		"{if {} }",
		"{if(()) {} }",
		"{if(-()) {} }",
		"{if(1) { }",
		"{if(1) { break; }}",
		"{if(1) { continue; }}",
		"{while(1) { break }}",
		"{while(0) { continue }}",
		"blub; ",
		"x = 0; ",
		"let a = ; ",
		"let b, a,; ",
		"let b, a, c,",
		"{ let a, c; a = 0 c = 6; }",
		"{ let a, c; a = 0; c = 6 }",
		"{ let a += 1; }",
	};

	size_t i;
	int success = 0;
	for(i = 0; i < ARRLEN(tests); ++i)
	{
		int ret = test_positive_run(tests[i].Source, tests[i].Expected);
		if(!ret)
		{
			printf("Positive Test %ld failed!\n%s\n\n", i, tests[i].Source);
		}

		success += ret;
	}

	for(i = 0; i < ARRLEN(tofail); ++i)
	{
		int ret = test_negative_run(tofail[i]);
		if(!ret)
		{
			printf("Negative Test %ld should fail but didn't!\n%s\n\n",
				i, tofail[i]);
		}
		else
		{
			++success;
		}
	}


	printf("%d/%ld tests succeeded\n", success, ARRLEN(tests) + ARRLEN(tofail));
}

#endif /* NANOC_DESKTOP */
