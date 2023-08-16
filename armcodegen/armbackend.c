#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define ARRLEN(N) (sizeof(N) / sizeof(*N))

enum
{
	COND_EQ = 0x0, /* Equal (Z == 1) */
	COND_NE = 0x1, /* Not Equal (Z == 0) */
	COND_CS = 0x2, /* Carry Set = Unsigned Higher or Same (C == 1) */
	COND_HS = COND_CS,
	COND_CC = 0x3, /* Carry Clear = Unsigned Lower (C == 0) */
	COND_LO = COND_CC,
	COND_MI = 0x4, /* Negative (N == 1) */
	COND_PL = 0x5, /* Positive (N == 0) */
	COND_VS = 0x6, /* Overflow Set (V == 1) */
	COND_VC = 0x7, /* Overflow Clear (V == 0) */
	COND_HI = 0x8, /* Unsigned Higher (C == 1) && (Z == 0) */
	COND_LS = 0x9, /* Unsigned Lower or Same (C == 0) || (Z == 0) */
	COND_GE = 0xA, /* Signed Greater or Equal (N == V) */
	COND_LT = 0xB, /* Signed Less Than (N != V) */
	COND_GT = 0xC, /* Signed Greater Than (Z == 0) && (N == V) */
	COND_LE = 0xD, /* Signed Less or Equal (Z == 1) || (N != V) */
	COND_AL = 0xE, /* Always */
};

enum
{
	R0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7,
	R8,
	R9,
	R10,
	R11,
	FP = R11, /* Frame Pointer */
	R12,
	IP = R12, /* Interprocedural Scratch Register */
	R13,
	SP = R13, /* Stack Pointer */
	R14,
	LR = R14, /* Link Register */
	R15,
	PC = R15  /* Program Counter */
};

enum
{
	OP_AND, /* And */
	OP_EOR, /* Exclusive Or */
	OP_SUB, /* Subtract */
	OP_RSB, /* Reverse Subtract */
	OP_ADD, /* Add */
	OP_ADC, /* Add with carry */
	OP_SBC, /* Subtract with carry */
	OP_RSC, /* Reverse Subtract with carry */
	OP_TST, /* Test bit */
	OP_TEQ, /*  */
	OP_CMP, /* Compare */
	OP_CMN, /* Compare Negated */
	OP_ORR, /* Or */
	OP_MOV, /* Move */
	OP_BIC, /* Bit clear */
	OP_MVN, /* Move Negated */
};

//void arm_r(int cond, );

#define COND_OFFSET 28

uint32_t arm_imm(
	uint32_t cond, uint32_t rd, uint32_t s, uint32_t rn, uint32_t val)
{
	uint32_t op2 = 0;

	return (cond << COND_OFFSET) |
		(s << 20) |
		(rn << 16) |
		(rd << 12) |
		op2;
}

/**
 * @brief Checks if the argument is a valid immediate shift amount
 *
 * @param v Shift amount to check
 * @return 1 if the shift amount is valid
 */
static uint32_t valid_imm_shift_amount(uint32_t v)
{
	return v < 32;
}

/**
 * @brief Checks if the argument is a valid immediate operand
 *
 * @param v Value to check
 * @return 1 if the value can be used as an immediate operand
 */
static uint32_t valid_imm_operand(uint32_t v)
{
	return 1;
}

/**
 * @brief Generate the ARM Opcode for a data processing
 *        instruction with register operands and a shift
 *        amount stored in a register
 *
 * @param cond Condition code
 * @param op Arithmetic / Logic Function
 * @param s Set condition codes
 * @param rd Destination register
 * @param rn First operand register
 * @param rm Second operand register
 * @param rs Register that contains the shift amount
 * @param shift_type Shift type
 */
static uint32_t arm_reg_shift_reg(
	uint32_t cond, uint32_t op, uint32_t s,
	uint32_t rd, uint32_t rn, uint32_t rm,
	uint32_t rs, uint32_t shift_type)
{
	return (cond << COND_OFFSET) |
		(op << 21) |
		(s << 20) |
		(rn << 16) |
		(rd << 12) |
		(rs << 8) |
		(shift_type << 5) |
		(1 << 4) |
		rm;
}

/**
 * @brief Generate the ARM Opcode for a data processing
 *        instruction with register operands and an
 *        immediate shift amount
 *
 * @param cond Condition code
 * @param op Arithmetic / Logic Function
 * @param s Set condition codes
 * @param rd Destination register
 * @param rn First operand register
 * @param rm Second operand register
 * @param shift Immediate shift amount
 * @param shift_type Shift type
 */
static uint32_t arm_reg_shift_imm(
	uint32_t cond, uint32_t op, uint32_t s,
	uint32_t rd, uint32_t rn, uint32_t rm,
	uint32_t shift, uint32_t shift_type)
{
	return (cond << COND_OFFSET) |
		(op << 21) |
		(s << 20) |
		(rn << 16) |
		(rd << 12) |
		(shift << 8) |
		(shift_type << 5) |
		rm;
}

/**
 * @brief Generate the ARM Opcode for a branch instruction
 *
 * @param cond Condition code
 * @param link If set, branch and link
 * @param cur Current address
 * @param target Target address
 */
static uint32_t arm_branch(
	uint32_t cond, uint32_t link, uint32_t cur, uint32_t target)
{
	uint32_t offset = (target - cur - 8) & ((1 << 24) - 1);

	return (cond << COND_OFFSET) |
		((uint32_t)0x05 << 25) |
		(link << 24) |
		offset;
}

/* Branch */
static uint32_t arm_b(uint32_t cond, uint32_t cur, uint32_t target)
{
	return arm_branch(cond, 0, cur, target);
}

/* Branch and Link */
static uint32_t arm_bl(uint32_t cond, uint32_t cur, uint32_t target)
{
	return arm_branch(cond, 1, cur, target);
}

/* Branch and Exchange */
static uint32_t arm_bx(uint32_t cond, uint32_t rn)
{
	return (cond << COND_OFFSET) |
		((uint32_t)0x12FFF1 << 4) | rn;
}

typedef enum
{
	TT_UNKNOWN = 128,

	TT_IDENTIFIER,
	TT_NUMBER,
	TT_STRING,

	TT_L_SHIFT,
	TT_R_SHIFT,
	TT_L_AND,
	TT_L_OR,

	TT_GT,
	TT_LT,
	TT_GE,
	TT_LE,
	TT_EQ,
	TT_NE,

	TT_ASSIGN,
	TT_ADD_ASSIGN,
	TT_SUB_ASSIGN,
	TT_MUL_ASSIGN,
	TT_DIV_ASSIGN,
	TT_MOD_ASSIGN,
	TT_L_SHIFT_ASSIGN,
	TT_R_SHIFT_ASSIGN,
	TT_B_OR_ASSIGN,
	TT_B_AND_ASSIGN,
	TT_B_NOT_ASSIGN,
	TT_XOR_ASSIGN,
	TT_INCREMENT,
	TT_DECREMENT,
	TT_ARROW,
} TokenType;

typedef struct
{
	const char *Start;
	size_t Length;
	TokenType Type;
} Token;

typedef struct
{
	const char *Pos;
	size_t Capacity;
	size_t Length;
	Token *Buffer;
} TokenList;

static int is_identifer_start(int c)
{
	return isalpha(c) || c == '_';
}

static int is_identifier_char(int c)
{
	return isalnum(c) || c == '_';
}

static void save_token(TokenList *list, const char *start, size_t len,
	TokenType type)
{
	Token *cur;

	if(list->Length == list->Capacity)
	{
		return;
	}

	cur = list->Buffer + list->Length;

	cur->Start = start;
	cur->Length = len;
	cur->Type = type;

	printf("save token: %d\n", type);

	++list->Length;
}

static int is_instruction(char *ident)
{
	enum
	{
		D_UNKNOWN = 128,
		D_ALIGN,
		D_FILL,
		D_EQU,
		D_DCB,
		D_DCW,
		D_DCD
	};

	const char cmds[] =
	{
		'A', 'L', 'I', 'G', 'N', D_ALIGN,
		'F', 'I', 'L', 'L', D_FILL,
		'E', 'Q', 'U', D_EQU,
		'D', 'C', 'B', D_DCB,
		'D', 'C', 'W', D_DCW,
		'D', 'C', 'D', D_DCD,
		D_UNKNOWN,
	};

	int cmd = 0;

	if(cmd == D_FILL)
	{
		/* expr (number of bytes),
		value to fill with (default 0),
		valuesize (default 1) */
	}

	return 0;
}




#if 0

static int _parser_expression(NanoC *n)
{
	TokenType prev, op_stack[NANOC_MAX_OP];
	u32 idx_old, paren_cnt, cnt;
	size_t si;

	prev = TT_NULL;
	paren_cnt = 1;
	si = 0;
	idx_old = n->Parser.Index;
	cnt = 0;

	for(;;)
	{
		switch(n->Token.Type)
		{
			case TT_NUMBER:
			{
				u32 value;
				if(prev == TT_NUMBER || prev == TT_IDENTIFIER ||
						prev == TT_R_PAREN)
				{
					TRACE(ERROR_UNEXPECTED_TOKEN);
				}

				++cnt;

				value = n->Token.Value.Number;
				n->Parser.Output[n->Parser.Index++] = INSTR_PUSHI32;
				_write_32(n->Parser.Output + n->Parser.Index, value);
				n->Parser.Index += 4;
				break;
			}

			case TT_IDENTIFIER:
			{
				int i;
				if(prev == TT_NUMBER || prev == TT_IDENTIFIER ||
						prev == TT_R_PAREN)
				{
					TRACE(ERROR_UNEXPECTED_TOKEN);
				}


				if((i = _map_find(&n->Parser.Variables, tok.Value.Identifier.Name,
					tok.Value.Identifier.Length)) < 0)
				{
					TRACE(ERROR_UNDEFINED_VAR);
				}

				++cnt;
				n->Parser.Output[n->Parser.Index++] = INSTR_PUSHL;
				n->Parser.Output[n->Parser.Index++] = (u8)(i & 0xFF);
				break;
			}

			case TT_L_PAREN:
			{
				if(si >= sizeof(op_stack) / sizeof(*op_stack) - 1)
				{
					TRACE(ERROR_STACK_OVERFLOW);
				}

				cnt = 0;
				++paren_cnt;
				op_stack[si++] = n->Token.Type;
				break;
			}

			case TT_R_PAREN:
			{
				if(--paren_cnt == 0)
				{
					goto exit;
				}

				for(; si; --si)
				{
					if(op_stack[si - 1] == TT_L_PAREN)
					{
						--si;
						goto found;
					}

					n->Parser.Output[n->Parser.Index++] = token_to_instr(op_stack[si - 1]);
				}

				TRACE(ERROR_MISMATCHED_PAREN);
found:
				if(!cnt)
				{
					TRACE(ERROR_UNEXPECTED_TOKEN);
				}
				break;
			}

			case TT_COMMA:
			case TT_SEMICOLON:
			case TT_R_BRACKET:
			{
				goto exit;
			}

			case TT_SUB:
			{
				if(prev == TT_NULL || prev == TT_L_PAREN)
				{
					n->Token.Type = TT_U_MINUS;
				}
			}
			/* fall through */

			default:
			{
				if(IS_OPERATOR(n->Token.Type))
				{
					int prec;
					if(IS_OPERATOR(prev) && n->Token.Type != TT_U_MINUS)
					{
						TRACE(ERROR_UNEXPECTED_TOKEN);
					}

					prec = _get_precedence(n->Token.Type);
					for(; si; --si)
					{
						if((_get_precedence(op_stack[si - 1]) > prec) ||
							(op_stack[si - 1] == TT_L_PAREN))
						{
							break;
						}

						n->Parser.Output[n->Parser.Index++] = token_to_instr(op_stack[si - 1]);
					}

					if(si >= sizeof(op_stack) / sizeof(*op_stack) - 1)
					{
						TRACE(ERROR_STACK_OVERFLOW);
					}

					op_stack[si++] = n->Token.Type;
				}
				else
				{
					TRACE(ERROR_UNEXPECTED_TOKEN);
				}
				break;
			}
		}

		prev = n->Token.Type;
		return_if(_lexer_next(&n->Lexer, &n->Token));
		if(IS_OPERATOR(prev) &&
				n->Token.Type != TT_NUMBER &&
				n->Token.Type != TT_L_PAREN &&
				n->Token.Type != TT_IDENTIFIER)
		{
			if(prev != TT_U_MINUS)
			{
				TRACE(ERROR_UNEXPECTED_TOKEN);
			}
		}
	}

exit:
	for(; si; --si)
	{
		if(op_stack[si - 1] == TT_L_PAREN)
		{
			TRACE(ERROR_MISMATCHED_PAREN);
		}

		n->Parser.Output[n->Parser.Index++] = token_to_instr(op_stack[si - 1]);
	}

	if(n->Parser.Index == idx_old)
	{
		TRACE(ERROR_INV_EXPR);
	}

	return 0;
}

#endif


static int hexdigit_value(int c)
{
	return (c & 15) + (c >= 'A' ? 9 : 0);
}

static int isbinary(int c)
{
	return c == '0' || c == '1';
}

static int isoctal(int c)
{
	return c >= '0' && c <= '7';
}

static TokenType _get_operator(const char *line, size_t *out_len)
{
	/* Must be sorted from longer to shorter operator
		strings because of common prefixes */
	static const uint8_t op_list[] =
	{
		/* Three character */
		'<', '<', '=', TT_L_SHIFT_ASSIGN,
		'>', '>', '=', TT_R_SHIFT_ASSIGN,

		/* Two character */
		'!', '=', TT_NE,
		'=', '=', TT_EQ,
		'>', '=', TT_GE,
		'<', '=', TT_LE,
		'<', '<', TT_L_SHIFT,
		'>', '>', TT_R_SHIFT,
		'&', '&', TT_L_AND,
		'|', '|', TT_L_OR,
		'+', '=', TT_ADD_ASSIGN,
		'-', '=', TT_SUB_ASSIGN,
		'*', '=', TT_MUL_ASSIGN,
		'/', '=', TT_DIV_ASSIGN,
		'%', '=', TT_MOD_ASSIGN,
		'~', '=', TT_B_NOT_ASSIGN,
		'&', '=', TT_B_AND_ASSIGN,
		'|', '=', TT_B_OR_ASSIGN,
		'^', '=', TT_XOR_ASSIGN,
		'+', '+', TT_INCREMENT,
		'-', '-', TT_DECREMENT,
		'-', '>', TT_ARROW,

		/* Not found */
		TT_UNKNOWN
	};

	const uint8_t *r = op_list;
	for(;;)
	{
		const char *s = line;
		const uint8_t *q = r;

		/* Compare strings */
		while(*s == *r)
		{
			++s;
			++r;
		}

		if(*r & 0x80)
		{
			*out_len = r - q;
			return *r;
		}

		while(!(*r & 0x80)) { ++r; }
		++r;
	}
}

static int parse_identifier(TokenList *tokens)
{
	const char *p, *start;

	p = tokens->Pos;
	if(!is_identifer_start(*p)) { return 0; }
	start = p;
	do { ++p; } while(is_identifier_char(*p));
	save_token(tokens, start, p - start, TT_IDENTIFIER);
	tokens->Pos = p;
	return 1;
}

static int parse_number(TokenList *tokens)
{
	const char *p, *start;
	int n, c;

	p = tokens->Pos;
	c = *p;
	if(!isdigit(c))
	{
		return 0;
	}

	n = c - '0';
	start = p;
	++p;
	c = *p;
	if(n > 0)
	{
		/* decimal */
		while(isdigit(c))
		{
			n = n * 10 + (c - '0');
			++p;
			c = *p;
		}
	}
	else
	{
		if(c == 'x' || c == 'X')
		{
			/* hexadecimal */
			++p;
			while(isxdigit((c = *p)))
			{
				n = n * 16 + hexdigit_value(c);
				++p;
			}
		}
		else if(c == 'b' || c == 'B')
		{
			/* binary */
			++p;
			while(isbinary((c = *p)))
			{
				n = n * 2 + (c - '0');
				++p;
			}
		}
		else
		{
			/* octal */
			while(isoctal(c))
			{
				n = n * 8 + (c - '0');
				++p;
				c = *p;
			}
		}
	}

	save_token(tokens, start, p - start, TT_NUMBER);
	tokens->Pos = p;
	return 1;
}

static int escseq(const char *p, size_t *out_len)
{
	static const char tab[] =
	{
		'\\', '\\',
		'\'', '\'',
		'\"', '\"',
		't', '\t',
		'v', '\v',
		'r', '\r',
		'n', '\n',
		'b', '\b',
		'0', '\0',
		'\0'
	};

	const char *t;
	int c;

	c = *p;
	if(c == 'x' && isxdigit(p[1]) && isxdigit(p[2]))
	{
		*out_len = 3;
		return 16 * hexdigit_value(p[1]) + hexdigit_value(p[2]);
	}

	for(t = tab; *t; t += 2)
	{
		if(c == *t)
		{
			*out_len = 1;
			return t[1];
		}
	}

	return -1;
}

static int parse_char(TokenList *tokens)
{
	const char *p, *start;
	int c, v;

	p = tokens->Pos;
	if(*p != '\'')
	{
		return 0;
	}

	start = p;
	++p;
	c = *p;
	if(c == '\\')
	{
		size_t len;
		++p;
		v = escseq(p, &len);
		if(v < 0)
		{
			return 0;
		}

		p += len;
	}
	else if(c >= 32 && c <= 126 && c != '\'')
	{
		v = c;
		++p;
	}
	else
	{
		return 0;
	}

	if(*p != '\'')
	{
		return 0;
	}

	++p;
	tokens->Pos = p;
	save_token(tokens, start, p - start, TT_NUMBER);
	return 1;
}

static int parse_string(TokenList *tokens)
{
	const char *p, *start;
	int c, v;

	p = tokens->Pos;
	if(*p != '\"') { return 0; }
	start = p;
	++p;
	while((c = *p) != '\"')
	{
		if(c == '\\')
		{
			size_t len;
			++p;
			v = escseq(p, &len);
			if(v < 0)
			{
				return 0;
			}

			p += len;
		}
		else if(c >= 32 && c <= 126 && c != '\'')
		{
			v = c;
			++p;
		}
		else
		{
			return 0;
		}
	}

	++p;
	save_token(tokens, start, p - start, TT_STRING);
	tokens->Pos = p;
	return 1;
}

static int parse_operator(TokenList *tokens)
{
	const char *p;
	size_t len;
	TokenType op_type;

	p = tokens->Pos;
	op_type = _get_operator(p, &len);
	if(op_type == TT_UNKNOWN)
	{
		save_token(tokens, p, 1, *p);
		++p;
	}
	else
	{
		save_token(tokens, p, len, op_type);
		p += len;
	}

	tokens->Pos = p;
	return 0;
}

static int parse_whitespace(TokenList *tokens)
{
	const char *p;

	p = tokens->Pos;
	if(!isspace(*p))
	{
		return 0;
	}

	do { ++p; } while(isspace(*p));
	tokens->Pos = p;
	return 1;
}

static void parse_piece(TokenList *tokens)
{
	if(parse_whitespace(tokens)) { return; }
	if(parse_identifier(tokens)) { return; }
	if(parse_number(tokens)) { return; }
	if(parse_char(tokens)) { return; }
	if(parse_string(tokens)) { return; }
	if(parse_operator(tokens)) { return; }
}

#define COLOR_RESET "\033[0m"
#define COLOR_BOLD_RED "\033[1;31m"

static void print_token_marked(Token *token, const char *line)
{
	int i, token_offset, token_len;

	token_offset = (int)(token->Start - line);
	token_len = token->Length;

	printf("\n\n");
	printf("%.*s", token_offset, line);
	printf(COLOR_BOLD_RED "%.*s" COLOR_RESET, token_len, token->Start);
	printf("%s\n", token->Start + token->Length);

	for(i = 0; i < token_offset; ++i)
	{
		printf(" ");
	}

	printf(COLOR_BOLD_RED "^");
	for(i = 1; i < token_len; ++i)
	{
		printf("~");
	}

	printf(COLOR_RESET "\n\n");
}

static void debug_tokens(TokenList *tokens, const char *line)
{
	size_t i;
	for(i = 0; i < tokens->Length; ++i)
	{
		print_token_marked(&tokens->Buffer[i], line);
	}
}

static int parse_line(size_t line_nr, const char *line, size_t len)
{
	Token token_buffer[256];
	TokenList tokens =
	{
		.Pos = line,
		.Length = 0,
		.Capacity = ARRLEN(token_buffer),
		.Buffer = token_buffer
	};

	const char *p;
	int c;

	printf("Parsing line %ld (len = %ld): %s\n", line_nr, len, line);

	p = line;
	while((c = *p) && c != ';')
	{
		parse_piece(&tokens);
		p = tokens.Pos;
	}

	debug_tokens(&tokens, line);
	return 0;
}

static int parse_file(const char *input_filename)
{
	char line_buf[MAX_LINE_LEN];
	size_t line_nr;
	FILE *input_file;

	if(!(input_file = fopen(input_filename, "r")))
	{
		fprintf(stderr, "Failed to open input file `%s`\n", input_filename);
		return 1;
	}

	line_nr = 1;
	while(fgets(line_buf, MAX_LINE_LEN, input_file))
	{
		size_t line_len = strlen(line_buf);
		if(line_len > 0)
		{
			if(line_buf[line_len - 1] == '\n')
			{
				line_buf[--line_len] = '\0';
			}
			else if(!feof(input_file))
			{
				fprintf(stderr, "Line %ld exceeds maximum supported line "
					"length of %d characters\n", line_nr, MAX_LINE_LEN);

				fclose(input_file);
				return 1;
			}

			if(parse_line(line_nr, line_buf, line_len))
			{
				return 1;
			}
		}

		++line_nr;
	}

	fclose(input_file);
	return 0;
}

int main(int argc, char **argv)
{
	const char *program_name, *input_filename, *output_filename;

	program_name = argv[0];
	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s `input-file` `output-file`\n",
			program_name);
		return 1;
	}

	input_filename = argv[1];
	output_filename = argv[2];

	parse_file(input_filename);

	return 0;
}
