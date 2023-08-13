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
} TokenType;

typedef struct
{
	const char *Start;
	size_t Length;
	TokenType Type;
} Token;

typedef struct
{
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

	"LDR"
	"STR"
	"MOV"
	"";

}

static TokenType _get_operator(char *line, size_t *out_len)
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

static int parse_line(size_t line_nr, char *line, size_t len)
{
	Token token_buffer[10];
	TokenList tokens =
	{
		.Length = 0,
		.Capacity = ARRLEN(token_buffer),
		.Buffer = token_buffer
	};

	char *p;
	int c;

	printf("parsing line %ld (len = %ld): %s\n", line_nr, len, line);

	p = line;
	while((c = *p))
	{
		if(is_identifer_start(c))
		{
			size_t identifier_len;
			const char *identifier_start = p;
			++p;
			while(is_identifier_char((c = *p)))
			{
				++p;
			}

			identifier_len = p - identifier_start;
			save_token(&tokens, p, identifier_len, TT_IDENTIFIER);
		}
		else if(isdigit(c))
		{
			const char *start = p;

			/* number */
			int n = c - '0';
			++p;
			if(n > 0)
			{
				/* decimal */
				while(isdigit((c = *p)))
				{
					n = n * 10 + (c - '0');
					++p;
				}
			}
			else
			{
				if((c = *p) == 'x' || c == 'X')
				{
					/* hexadecimal */
					++p;
					while(isxdigit((c = *p)))
					{
						n = n * 16 + (c & 15) + (c >= 'A' ? 9 : 0);
						++p;
					}
				}
				else if((c = *p) == 'b' || c == 'B')
				{
					/* binary */
					++p;
					while((c = *p) == '0' || c == '1')
					{
						n = n * 2 + (c - '0');
						++p;
					}
				}
				else
				{
					/* octal */
					while((c = *p) >= '0' && c <= '7')
					{
						n = n * 8 + (c - '0');
						++p;
					}
				}
			}

			save_token(&tokens, start, p - start, TT_NUMBER);
		}
		else if(c == '\'')
		{
			/* character */
		}
		else if(c == '\"')
		{
			/* string */
		}
		else if(isspace(c))
		{
			/* skip whitespace */
			do { ++p; } while(isspace((c = *p)));
		}
		else if(c == ';')
		{
			/* comment */
			break;
		}
		else
		{
			size_t len;
			TokenType op_type = _get_operator(p, &len);
			if(op_type == TT_UNKNOWN)
			{
				save_token(&tokens, p, 1, c);
				++p;
			}
			else
			{
				save_token(&tokens, p, len, op_type);
				p += len;
			}
		}
	}



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
