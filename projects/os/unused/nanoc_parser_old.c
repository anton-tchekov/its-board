
#if 0

void nanoc_init(NanoC *n)
{
	_map_init(&n->Parser.Variables, n->Parser.VariableBuffer, sizeof(n->Parser.VariableBuffer) / sizeof(*n->Parser.VariableBuffer));
	_map_init(&n->Parser.Functions, n->Parser.FunctionBuffer, sizeof(n->Parser.FunctionBuffer) / sizeof(*n->Parser.FunctionBuffer));
}

i32 nanoc_compile(NanoC *n, const char *src, u8 *out, u8 *strings, i32 string_offset)
{
	i32 i;
	u32 idx_main, len;
	const char *name;

	n->Parser.Output = out;
	_lexer_init(&n->Lexer, src, (char *)strings, string_offset);
	n->Parser.Output[n->Parser.Index++] = INSTR_CALL;
	n->Parser.Output[n->Parser.Index++] = 0;
	idx_main = n->Parser.Index;
	n->Parser.Index += 2;
	n->Parser.Output[n->Parser.Index++] = INSTR_HALT;
	for(;;)
	{
		PROPAGATE(_lexer_next(&n->Lexer, &n->Token));
		if(n->Token.Type == TT_NULL)
		{
			break;
		}
		else if(n->Token.Type != TT_INT)
		{
			THROW(ERROR_EXPECTED_INT);
		}

		PROPAGATE(_lexer_next(&n->Lexer, &n->Token));
		if(n->Token.Type != TT_IDENTIFIER)
		{
			THROW(ERROR_EXPECTED_IDENTIFIER);
		}

		name = n->Token.Value.Identifier.Name;
		len = n->Token.Value.Identifier.Length;
		PROPAGATE(_lexer_next(&n->Lexer, &n->Token));
		if(n->Token.Type != TT_L_PAREN)
		{
			THROW(ERROR_EXPECTED_L_PAREN);
		}

		PROPAGATE(_parser_function(n, name, len));
	}

	/* check that all functions are implemented */
	for(i = 0; i < (i32)n->Parser.Functions.Count; ++i)
	{
		if(n->Parser.FN_Addrs[i] == 0)
		{
			THROW(ERROR_UNDEFINED_FN);
		}
	}

	if((i = _map_find(&n->Parser.Functions, "main", 4)) < 0)
	{
		THROW(ERROR_UNDEFINED_MAIN);
	}

	_write_16(n->Parser.Output + idx_main, n->Parser.FN_Addrs[i]);
	return 0;
}

static i32 _parser_function(NanoC *n, const char *name, u32 len)
{
	i32 i, impl;
	u8 args;

	impl = 0;
	args = 0;
	if((i = _map_insert(&n->Parser.Functions, name, len)) < 0)
	{
		if(i == -ERROR_DUP_MAP_ELEM)
		{
			if((i = _map_find(&n->Parser.Functions, name, len)) < 0)
			{
				THROW(ERROR_DUP_MAP_ELEM);
			}

			if(n->Parser.FN_Addrs[i] == 0)
			{
				impl = 1;
			}
			else
			{
				THROW(ERROR_DUP_MAP_ELEM);
			}
		}
	}

	n->Parser.FN_Addrs[i] = n->Parser.Index;
	PROPAGATE(_lexer_next(&n->Lexer, &n->Token));
	for(;;)
	{
		if(n->Token.Type == TT_INT)
		{
			PROPAGATE(_lexer_next(&n->Lexer, &n->Token));
			if(n->Token.Type != TT_IDENTIFIER)
			{
				THROW(ERROR_EXPECTED_IDENTIFIER);
			}

			PROPAGATE(_map_insert(&n->Parser.Variables, n->Token.Value.Identifier.Name,
				n->Token.Value.Identifier.Length));

			++args;
			PROPAGATE(_lexer_next(&n->Lexer, &n->Token));
			if(n->Token.Type == TT_COMMA)
			{
				PROPAGATE(_lexer_next(&n->Lexer, &n->Token));
			}
		}
		else if(n->Token.Type == TT_R_PAREN)
		{
			break;
		}
		else
		{
			THROW(ERROR_UNEXPECTED_TOKEN);
		}
	}

	PROPAGATE(_lexer_next(&n->Lexer, &n->Token));
	if(n->Token.Type != TT_L_BRACE)
	{
		THROW(ERROR_EXPECTED_L_BRACE);
	}

	if(impl)
	{
		if(args != n->Parser.FN_Args[i])
		{
			THROW(ERROR_FN_NUM_ARGS);
		}

		{
			u32 k;
			for(k = 0; k < n->Parser.UsagesCount; ++k)
			{
				if(_read_16(&n->Parser.Output[n->Parser.FunctionUsages[k] + 2]) == i)
				{
					_write_16(&n->Parser.Output[n->Parser.FunctionUsages[k] + 2], n->Parser.FN_Addrs[i]);
				}
			}
		}
	}

	n->Parser.FN_Args[i] = args;

	if(args)
	{
		n->Parser.Output[n->Parser.Index++] = INSTR_DSP;
		n->Parser.Output[n->Parser.Index++] = args;
	}

	PROPAGATE(_parser_block(n));

	if(n->Parser.Output[n->Parser.Index - 1] != INSTR_RET)
	{
		n->Parser.Output[n->Parser.Index++] = INSTR_PUSHI8;
		n->Parser.Output[n->Parser.Index++] = 0;
		n->Parser.Output[n->Parser.Index++] = INSTR_RET;
	}

	n->Parser.Variables.Count -= args;
	return 0;
}

static i32 _parser_block(NanoC *n)
{
	i32 lvc;
	lvc = 0;
	for(;;)
	{
		PROPAGATE(_lexer_next(&n->Lexer, &n->Token));
		if(n->Token.Type == TT_INT)
		{
			do
			{
				i32 i;
				PROPAGATE(_lexer_next(&n->Lexer, &n->Token));
				if(n->Token.Type != TT_IDENTIFIER)
				{
					THROW(ERROR_INV_VAR_DECL);
				}

				PROPAGATE(i = _map_insert(&n->Parser.Variables, n->Token.Value.Identifier.Name,
					n->Token.Value.Identifier.Length));

				++lvc;
				PROPAGATE(_lexer_next(&n->Lexer, &n->Token));
			}
			while(n->Token.Type == TT_COMMA);
			if(n->Token.Type != TT_SEMICOLON)
			{
				THROW(ERROR_EXPECTED_SEMICOLON);
			}
		}
		else
		{
			break;
		}
	}

	if(lvc)
	{
		if(n->Parser.Output[n->Parser.Index - 2] == INSTR_DSP)
		{
			n->Parser.Output[n->Parser.Index - 1] += lvc;
		}
		else
		{
			n->Parser.Output[n->Parser.Index++] = INSTR_DSP;
			n->Parser.Output[n->Parser.Index++] = lvc;
		}
	}

	while(n->Token.Type != TT_R_BRACE)
	{
		PROPAGATE(_parser_statement(n));
		PROPAGATE(_lexer_next(&n->Lexer, &n->Token));
	}

	n->Parser.Variables.Count -= lvc;
	return 0;
}

static void _handle_break(NanoC *n, u32 prev, u32 addr_break)
{
	while(n->Parser.BT > prev)
	{
		--n->Parser.BT;
		_write_16(n->Parser.Output + n->Parser.BAS[n->Parser.BT], addr_break);
	}
}

static void _handle_continue(NanoC *n, u32 prev, u32 addr_continue)
{
	while(n->Parser.CT > prev)
	{
		--n->Parser.CT;
		_write_16(n->Parser.Output + n->Parser.CAS[n->Parser.CT], addr_continue);
	}
}

i32 nanoc_add_function(NanoC *n, u16 id, const char *name, u8 num_args, i32 (*function)(i32 *parameters, u8 *heap))
{
#if defined(NANOC_ENABLE_COMPILER) && NANOC_ENABLE_COMPILER != 0
	i32 i;
	PROPAGATE(i = _map_insert(&n->Parser.Functions, name, strlen(name)));
	n->Parser.FN_Addrs[i] = -id - 1;
	n->Parser.FN_Args[i] = num_args;
#endif

#if defined(NANOC_ENABLE_INTERPRETER) && NANOC_ENABLE_INTERPRETER != 0
	n->Interpreter.Functions[id] = function;
#endif

	return 0;
}



/* --- PARSER --- */
static StatusCode parser_compile(void)
{
	while(_token.Type != TT_NULL)
	{
		EXPECT(TT_FN, ERROR_EXPECTED_FN);
		PROPAGATE(_parser_fn());
		PROPAGATE(tokenizer_next());
	}

	PROPAGATE(_parser_check_impl());
	return SUCCESS;
}

static StatusCode _parser_fn(void)
{
	i16 i;
	u8 used, parameters;

	PROPAGATE(tokenizer_next());
	EXPECT(TT_FN_IDENTIFIER, ERROR_EXPECTED_IDENTIFIER);

	if((i = identifier_map_find(&_parser.Functions, _token.Identifier)) < 0)
	{
		used = 0;
		i = _identifier_map_insert(&_parser.Functions, _token.Number);
	}
	else
	{
		used = 1;
		if(_fn_addr_get(i))
		{
			THROW(ERROR_FN_REDEFINITION);
		}
	}

	_fn_addr_set(i, _parser.Offset);

	parameters = 0;
	PROPAGATE(tokenizer_next());
	EXPECT('(', ERROR_EXPECTED_L_PAREN);
	PROPAGATE(tokenizer_next());
	while(_token.Type != ')')
	{
		EXPECT(TT_LET_IDENTIFIER, ERROR_EXPECTED_IDENTIFIER);
		if(identifier_map_insert(&_parser.Variables, _token.Number, _token.Identifier) < 0)
		{
			THROW(ERROR_VAR_REDEFINITION);
		}

		++parameters;
		PROPAGATE(tokenizer_next());
		if(_token.Type == ',')
		{
			PROPAGATE(tokenizer_next());
			EXPECT(TT_LET_IDENTIFIER, ERROR_EXPECTED_IDENTIFIER);
		}
		else if(_token.Type != ')')
		{
			THROW(ERROR_UNEXPECTED_TOKEN);
		}
	}

	PROPAGATE(tokenizer_next());
	EXPECT('{', ERROR_EXPECTED_L_BRACE);
	if(used)
	{
		u16 j;
		if(parameters != _fn_params_get(i))
		{
			THROW(ERROR_FN_NUM_ARGS);
		}

		for(j = 0; j < _parser.UsagesCount; ++j)
		{
			u16 usage = _fn_usage_get(j);
			if(memory_r16(BANK_INTERPRETER, usage) == i)
			{
				memory_w16(BANK_INTERPRETER, usage, _fn_addr_get(i));
			}
		}
	}
	else
	{
		_fn_params_set(i, parameters);
	}

	PROPAGATE(_parser_fn_block(parameters));

	/* No return statement */
	if(memory_r8(BANK_INTERPRETER, OFFSET_CODE + _parser.Offset - 1) != INSTR_RET)
	{
		_emit8(INSTR_PUSHI8);
		_emit8(0);
		_emit8(INSTR_RET);
	}

	return SUCCESS;
}


#endif
