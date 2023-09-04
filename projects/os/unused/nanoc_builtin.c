#if 0

static const i8 _num_parameters[] PROGMEM =
{
	/* INT */
	2, /* add  */
	2, /* sub  */
	2, /* mul  */
	2, /* div  */
	2, /* mod  */
	2, /* abs  */
	2, /* and  */
	2, /* or   */
	2, /* xor  */
	2, /* inv  */
	2, /* shl  */
	2, /* shr  */
	1, /* not  */
	2, /* eq   */
	2, /* ne   */
	2, /* lt   */
	2, /* gt   */
	2, /* le   */
	2, /* ge   */
	2, /* hi   */
	2, /* lo   */
	2, /* hs   */
	2, /* ls   */

	/* FLOAT */
	1, /* itf  */
	1, /* fti  */
	2, /* addf */
	2, /* subf */
	2, /* mulf */
	2, /* divf */
	2, /* modf */
	1, /* absf */
	2, /* ltf  */
	2, /* gtf  */
	2, /* lef  */
	2, /* gef  */

	/* MATH */
	1, /* sin   */
	1, /* cos   */
	1, /* tan   */
	1, /* asin  */
	1, /* acos  */
	1, /* atan  */
	2, /* atan2 */
	1, /* sinh  */
	1, /* cosh  */
	1, /* tanh  */
	1, /* exp   */
	1, /* log   */
	1, /* log10 */
	2, /* pow   */
	1, /* sqrt  */
	1, /* ceil  */
	1, /* floor */
	1, /* round */

	/* CHAR */
	1, /* isupper  */
	1, /* islower  */
	1, /* isdigit  */
	1, /* isalpha  */
	1, /* isalnum  */
	1, /* iscntrl  */
	1, /* isgraph  */
	1, /* isprint  */
	1, /* ispunct  */
	1, /* isspace  */
	1, /* isxdigit */
	1, /* tolower  */
	1, /* toupper  */

	/* MEMORY */
	2, /* w32  */
	2, /* w16 */
	2, /* w8  */
	1, /* r32  */
	1, /* ru16 */
	1, /* ru8  */
	1, /* rs16 */
	1, /* rs8  */

	1, /* alloc */
	1, /* free */

	3, /* mcpy */
	3, /* mcmp */
	3, /* mchr */
	3, /* mset */

	/* RANDOM */
	1, /* srand */
	0, /* rand  */

	/* IO */
	2, /* fputs */
	3, /* fputse */
	2, /* fputd */
	3, /* fputde */
	2, /* fputx */
	3, /* fputxe */

	2, /* fputf */
	4, /* fputfe */

	2, /* fputc */

	/* FILE */
	2, /* fopen  */
	3, /* fread  */
	1, /* fclose */
	3, /* fwrite */
	2, /* fseek  */
	1, /* ftell  */

	0, /* millis */

	5, /* rect  */
	3, /* color */
	0, /* update */
	3, /* hsl_to_rgb */
};

typedef u32 (*const builtin_ptr)(u32 *);

static builtin_ptr _builtins[] PROGMEM =
{
	/* INT */
	_add,
	_sub,
	_mul,
	_div,
	_mod,
	_abs,
	_and,
	_or,
	_xor,
	_inv,
	_shl,
	_shr,
	_not,
	_eq,
	_ne,
	_lt,
	_gt,
	_le,
	_ge,
	_hi,
	_lo,
	_hs,
	_ls,

	/* FLOAT */
	_itf,
	_fti,
	_addf,
	_subf,
	_mulf,
	_divf,
	_modf,
	_absf,
	_ltf,
	_gtf,
	_lef,
	_gef,

	/* MATH */
	_sin,
	_cos,
	_tan,
	_asin,
	_acos,
	_atan,
	_atan2,
	_sinh,
	_cosh,
	_tanh,
	_exp,
	_log,
	_log10,
	_pow,
	_sqrt,
	_ceil,
	_floor,
	_round,

	/* CHAR */
	_isupper,
	_islower,
	_isdigit,
	_isalpha,
	_isalnum,
	_iscntrl,
	_isgraph,
	_isprint,
	_ispunct,
	_isspace,
	_isxdigit,
	_tolower,
	_toupper,

	/* MEMORY */
	_w32,
	_w16,
	_w8,
	_r32,
	_ru16,
	_ru8,
	_rs16,
	_rs8,

	_alloc,
	_free,

	_mcpy,
	_mcmp,
	_mchr,
	_mset,

	/* RANDOM */
	_srand,
	_rand,

	/* IO */
	_fputs,
	_fputse,
	_fputd,
	_fputde,
	_fputx,
	_fputxe,

	_fputf,
	_fputfe,

	_fputc,

	/* FILE */
	_file_open,
	_file_read,
	_file_close,
	_file_write,
	_file_seek,
	_file_tell,

	_millis,

	_rect,
	_color,
	_update,
	_hsl_to_rgb
};

static const char _identifiers[] PROGMEM =

	/* INT */
	"add\0"
	"sub\0"
	"mul\0"
	"div\0"
	"mod\0"
	"abs\0"
	"and\0"
	"or\0"
	"xor\0"
	"inv\0"
	"shl\0"
	"shr\0"
	"not\0"
	"eq\0"
	"ne\0"
	"lt\0"
	"gt\0"
	"le\0"
	"ge\0"
	"hi\0"
	"lo\0"
	"hs\0"
	"ls\0"

	/* FLOAT */
	"itf\0"
	"fti\0"
	"addf\0"
	"subf\0"
	"mulf\0"
	"divf\0"
	"modf\0"
	"absf\0"
	"ltf\0"
	"gtf\0"
	"lef\0"
	"gef\0"

	/* MATH */
	"sin\0"
	"cos\0"
	"tan\0"
	"asin\0"
	"acos\0"
	"atan\0"
	"atan2\0"
	"sinh\0"
	"cosh\0"
	"tanh\0"
	"exp\0"
	"log\0"
	"log10\0"
	"pow\0"
	"sqrt\0"
	"ceil\0"
	"floor\0"
	"round\0"

	/* CHAR */
	"isupper\0"
	"islower\0"
	"isdigit\0"
	"isalpha\0"
	"isalnum\0"
	"iscntrl\0"
	"isgraph\0"
	"isprint\0"
	"ispunct\0"
	"isspace\0"
	"isxdigit\0"
	"tolower\0"
	"toupper\0"

	/* MEMORY  */
	"w32\0"
	"w16\0"
	"w8\0"
	"r32\0"
	"ru16\0"
	"ru8\0"
	"rs16\0"
	"rs8\0"

	"alloc\0"
	"free\0"

	"mcpy\0"
	"mcmp\0"
	"mchr\0"
	"mset\0"

	/* RANDOM */
	"srand\0"
	"rand\0"

	/* IO */
	"fputs\0"
	"fputse\0"
	"fputd\0"
	"fputde\0"
	"fputx\0"
	"fputxe\0"

	"fputf\0"
	"fputfe\0"

	"fputc\0"

	/* FILE */
	"fopen\0"
	"fread\0"
	"fclose\0"
	"fwrite\0"
	"fseek\0"
	"ftell\0"

	"millis\0"

	"rect\0"
	"color\0"
	"update\0"
	"hsl_to_rgb\0"

	"|";

#ifdef DEBUG_INTERPRETER

static const char *_builtin_name(u32 id)
{
	const char *start;
	u32 count;

	if(id > ARRLEN(_num_parameters))
	{
		return "INVALID";
	}

	start = _identifiers;
	for(count = 0; count < id; ++count)
	{
		start += strlen(start) + 1;
	}

	return start;
}

#endif

static i8 _builtin_find(const char *name)
{
	return _string_find(_identifiers, name);
}

static i8 _builtin_num_parameters(u8 id)
{
	return pgm_read_byte(_num_parameters + id);
}

static u32 _builtin_call(u8 id, u32 *args)
{
	return ((builtin_ptr)pgm_read_ptr(_builtins + id))(args);
}
#endif