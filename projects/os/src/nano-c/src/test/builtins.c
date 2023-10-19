#include "builtins.h"

static const NanoC_ParserBuiltin parser_builtins_data[] =
{
	{
		.Name = "test",
		.NumArgs = 1,
		.IsVariadic = 0,
	},
	{
		.Name = "ldchar",
		.NumArgs = 1,
		.IsVariadic = 0,
	},
};

const NanoC_ParserBuiltins parser_builtins =
{
	.Count = ARRLEN(parser_builtins_data),
	.Table = parser_builtins_data,
};
