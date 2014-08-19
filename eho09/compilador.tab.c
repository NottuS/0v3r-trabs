/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "compilador.y"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"

char str[str_max];
char id[str_max];
int num_vars = 0;
int proc_num_vars = 0;
int num_param = 0;
int tipo;
int nivel = 0;
int deslocamento = 0;
int categoria = 0;
int num_tipo = 0;
int rot = 0;
int aux;
int auxDes;
int auxNiv;
int eh_ref = 0;
int proc_func = 0;
int params_cont = 0;
int tipo_func;
ETdS *elemento = NULL;
ETdS *elemento2 = NULL;
TdS *ts;
PdT *pt;
PdR *pr; 
PdF *pfunc;


/* Line 268 of yacc.c  */
#line 105 "compilador.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PROGRAM = 258,
     ABRE_PARENTESES = 259,
     FECHA_PARENTESES = 260,
     VIRGULA = 261,
     PONTO_E_VIRGULA = 262,
     DOIS_PONTOS = 263,
     PONTO = 264,
     T_BEGIN = 265,
     T_END = 266,
     VAR = 267,
     IDENT = 268,
     NUMERO = 269,
     ATRIBUICAO = 270,
     LABEL = 271,
     GOTO = 272,
     MAIOR = 273,
     MENOR = 274,
     IGUAL = 275,
     DIFERENTE = 276,
     MAIOR_OU_IGUAL = 277,
     MENOR_OU_IGUAL = 278,
     NOT = 279,
     OR = 280,
     AND = 281,
     FALSE = 282,
     TRUE = 283,
     MAIS = 284,
     MENOS = 285,
     DIV = 286,
     MUL = 287,
     DIV2 = 288,
     T_INTEGER = 289,
     T_REAL = 290,
     T_STR = 291,
     T_CHR = 292,
     T_BOOL = 293,
     WRITE = 294,
     READ = 295,
     FUNC = 296,
     PROC = 297,
     WHILE = 298,
     DO = 299,
     FOR = 300,
     TO = 301,
     DOWNTO = 302,
     REPEAT = 303,
     UNTIL = 304,
     IF = 305,
     THEN = 306,
     ELSE = 307,
     LOWER_THAN_ELSE = 308
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 200 "compilador.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   193

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  73
/* YYNRULES -- Number of rules.  */
#define YYNRULES  139
/* YYNRULES -- Number of states.  */
#define YYNSTATES  227

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   308

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     5,    14,    15,    21,    25,    26,
      30,    34,    36,    38,    40,    41,    45,    46,    49,    51,
      52,    53,    60,    62,    64,    66,    68,    70,    74,    76,
      80,    81,    85,    87,    91,    92,    94,    95,   102,   107,
     111,   113,   115,   117,   119,   121,   123,   124,   129,   130,
     133,   137,   138,   140,   141,   145,   148,   149,   156,   157,
     162,   164,   166,   168,   170,   172,   173,   177,   180,   184,
     188,   190,   194,   196,   198,   199,   203,   205,   206,   207,
     214,   215,   216,   217,   227,   228,   234,   236,   238,   239,
     240,   244,   245,   249,   252,   253,   259,   262,   263,   264,
     268,   270,   272,   274,   277,   280,   281,   285,   287,   288,
     292,   296,   300,   303,   305,   309,   313,   317,   321,   325,
     329,   331,   335,   339,   341,   345,   349,   353,   355,   357,
     359,   361,   362,   366,   370,   375,   380,   384,   386,   390
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      55,     0,    -1,    -1,    -1,    56,     3,    13,    71,    57,
       7,    58,     9,    -1,    -1,    60,    62,    82,    59,    91,
      -1,    16,    61,     7,    -1,    -1,    61,     6,    13,    -1,
      61,     6,    14,    -1,    13,    -1,    14,    -1,    63,    -1,
      -1,    64,    12,    65,    -1,    -1,    65,    66,    -1,    66,
      -1,    -1,    -1,    67,    70,     8,    69,    68,     7,    -1,
      34,    -1,    35,    -1,    36,    -1,    37,    -1,    38,    -1,
      70,     6,    13,    -1,    13,    -1,     4,    72,     5,    -1,
      -1,    72,     6,    13,    -1,    13,    -1,     4,    75,     5,
      -1,    -1,    12,    -1,    -1,    75,     7,    74,    76,     8,
      77,    -1,    74,    76,     8,    77,    -1,    76,     6,    13,
      -1,    13,    -1,    34,    -1,    35,    -1,    36,    -1,    37,
      -1,    38,    -1,    -1,    78,     6,    79,   118,    -1,    -1,
      80,   118,    -1,     4,    78,     5,    -1,    -1,    83,    -1,
      -1,    83,    84,     7,    -1,    84,     7,    -1,    -1,    41,
      85,    88,     8,    87,    90,    -1,    -1,    42,    86,    88,
      90,    -1,    34,    -1,    35,    -1,    36,    -1,    37,    -1,
      38,    -1,    -1,    13,    89,    73,    -1,     7,    58,    -1,
      10,    92,    11,    -1,    92,     7,    94,    -1,    94,    -1,
      10,    92,    11,    -1,    94,    -1,   109,    -1,    -1,    14,
      95,   113,    -1,   124,    -1,    -1,    -1,    43,    96,   118,
      97,    44,    93,    -1,    -1,    -1,    -1,    45,    13,    98,
     116,    99,   102,   100,    44,    93,    -1,    -1,    48,   101,
      92,    49,   118,    -1,   105,    -1,   112,    -1,    -1,    -1,
      46,   103,   120,    -1,    -1,    47,   104,   120,    -1,   106,
     108,    -1,    -1,    50,   118,   107,    51,    93,    -1,    52,
      93,    -1,    -1,    -1,    13,   110,   111,    -1,   116,    -1,
     113,    -1,   115,    -1,    17,    13,    -1,    17,    14,    -1,
      -1,     8,   114,    94,    -1,    81,    -1,    -1,    15,   117,
     118,    -1,   118,    26,   119,    -1,   118,    25,   119,    -1,
      24,   119,    -1,   119,    -1,   119,    20,   121,    -1,   119,
      21,   121,    -1,   119,    18,   121,    -1,   119,    19,   121,
      -1,   119,    22,   121,    -1,   119,    23,   121,    -1,   120,
      -1,   120,    29,   121,    -1,   120,    30,   121,    -1,   121,
      -1,   121,    32,   122,    -1,   121,    31,   122,    -1,   121,
      33,   122,    -1,   122,    -1,    28,    -1,    27,    -1,    14,
      -1,    -1,    13,   123,   115,    -1,     4,   118,     5,    -1,
      39,     4,   125,     5,    -1,    40,     4,   126,     5,    -1,
     125,     6,   118,    -1,   118,    -1,   126,     6,    13,    -1,
      13,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    49,    49,    55,    49,    77,    74,    82,    83,    86,
      92,    98,   104,   112,   119,   119,   120,   123,   124,   127,
     129,   127,   133,   134,   135,   136,   137,   140,   149,   161,
     162,   165,   166,   171,   172,   175,   176,   179,   180,   183,
     191,   201,   207,   213,   219,   225,   233,   233,   241,   241,
     251,   252,   255,   256,   259,   260,   263,   263,   264,   264,
     267,   268,   269,   270,   271,   274,   274,   301,   335,   338,
     339,   342,   343,   350,   351,   351,   358,   359,   364,   359,
     380,   389,   392,   380,   404,   404,   417,   418,   419,   422,
     422,   442,   442,   465,   471,   471,   491,   492,   495,   495,
     508,   509,   510,   514,   524,   535,   535,   550,   568,   568,
     600,   601,   602,   610,   613,   614,   615,   616,   617,   618,
     619,   622,   623,   624,   627,   628,   629,   630,   633,   638,
     643,   648,   648,   684,   688,   689,   692,   693,   696,   708
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PROGRAM", "ABRE_PARENTESES",
  "FECHA_PARENTESES", "VIRGULA", "PONTO_E_VIRGULA", "DOIS_PONTOS", "PONTO",
  "T_BEGIN", "T_END", "VAR", "IDENT", "NUMERO", "ATRIBUICAO", "LABEL",
  "GOTO", "MAIOR", "MENOR", "IGUAL", "DIFERENTE", "MAIOR_OU_IGUAL",
  "MENOR_OU_IGUAL", "NOT", "OR", "AND", "FALSE", "TRUE", "MAIS", "MENOS",
  "DIV", "MUL", "DIV2", "T_INTEGER", "T_REAL", "T_STR", "T_CHR", "T_BOOL",
  "WRITE", "READ", "FUNC", "PROC", "WHILE", "DO", "FOR", "TO", "DOWNTO",
  "REPEAT", "UNTIL", "IF", "THEN", "ELSE", "LOWER_THAN_ELSE", "$accept",
  "programa", "$@1", "$@2", "bloco", "$@3", "declara_rotulos", "rotulos",
  "parte_declara_vars", "var", "$@4", "declara_vars", "declara_var", "$@5",
  "$@6", "tipo", "lista_id_var", "prog_params", "lista_idents",
  "params_dec", "referencia", "lista_params", "lista_id_params",
  "tipo_param", "params_passagem", "$@7", "$@8", "params", "procs",
  "procs2", "proc_func", "$@9", "$@10", "tipo_func", "assinatura", "$@11",
  "corpo", "comando_composto", "comandos", "subcomandos", "comando",
  "$@12", "$@13", "$@14", "$@15", "$@16", "$@17", "$@18", "to_downto",
  "$@19", "$@20", "cond_if", "if_then", "$@21", "cond_else",
  "atribuicao_ou_CP", "$@22", "atribuicao_ou_CP2", "goto", "goto_dest",
  "$@23", "chama_proc", "atribuicao", "$@24", "expr", "comp", "arit",
  "termo", "fator", "$@25", "io", "write_vars", "read_var", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    54,    56,    57,    55,    59,    58,    60,    60,    61,
      61,    61,    61,    62,    64,    63,    63,    65,    65,    67,
      68,    66,    69,    69,    69,    69,    69,    70,    70,    71,
      71,    72,    72,    73,    73,    74,    74,    75,    75,    76,
      76,    77,    77,    77,    77,    77,    79,    78,    80,    78,
      81,    81,    82,    82,    83,    83,    85,    84,    86,    84,
      87,    87,    87,    87,    87,    89,    88,    90,    91,    92,
      92,    93,    93,    94,    95,    94,    94,    96,    97,    94,
      98,    99,   100,    94,   101,    94,    94,    94,    94,   103,
     102,   104,   102,   105,   107,   106,   108,   108,   110,   109,
     111,   111,   111,   112,   112,   114,   113,   115,   117,   116,
     118,   118,   118,   118,   119,   119,   119,   119,   119,   119,
     119,   120,   120,   120,   121,   121,   121,   121,   122,   122,
     122,   123,   122,   122,   124,   124,   125,   125,   126,   126
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,     8,     0,     5,     3,     0,     3,
       3,     1,     1,     1,     0,     3,     0,     2,     1,     0,
       0,     6,     1,     1,     1,     1,     1,     3,     1,     3,
       0,     3,     1,     3,     0,     1,     0,     6,     4,     3,
       1,     1,     1,     1,     1,     1,     0,     4,     0,     2,
       3,     0,     1,     0,     3,     2,     0,     6,     0,     4,
       1,     1,     1,     1,     1,     0,     3,     2,     3,     3,
       1,     3,     1,     1,     0,     3,     1,     0,     0,     6,
       0,     0,     0,     9,     0,     5,     1,     1,     0,     0,
       3,     0,     3,     2,     0,     5,     2,     0,     0,     3,
       1,     1,     1,     2,     2,     0,     3,     1,     0,     3,
       3,     3,     2,     1,     3,     3,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     1,     1,     1,
       1,     0,     3,     3,     4,     4,     3,     1,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     1,     0,    30,     0,     3,    32,     0,
       0,    29,     0,     8,    31,     0,     0,    16,    11,    12,
       0,     4,    53,    13,     0,     0,     7,    56,    58,     5,
      52,     0,    19,     9,    10,     0,     0,     0,     0,    55,
      15,    18,     0,    65,     0,     0,    88,     6,    54,    17,
      28,     0,    34,     0,     8,    59,    98,    74,     0,     0,
       0,    77,     0,    84,     0,     0,    70,    86,    97,    73,
      87,    76,     0,     0,    36,    66,    60,    61,    62,    63,
      64,     0,    67,    51,     0,   103,   104,     0,     0,     0,
      80,    88,     0,   131,   130,     0,   129,   128,    94,   113,
     120,   123,   127,    88,    68,    88,    93,    27,    22,    23,
      24,    25,    26,    20,    35,     0,     0,    57,    48,   105,
     108,   107,    99,   101,   102,   100,    75,   137,     0,   139,
       0,    78,     0,     0,     0,    51,   112,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    88,    96,    72,     0,    40,     0,    33,    36,
       0,     0,    88,     0,   134,     0,   135,     0,     0,    81,
       0,   133,   132,   111,   110,    88,   116,   117,   114,   115,
     118,   119,   121,   122,   125,   124,   126,     0,    21,     0,
       0,     0,    50,    46,    49,   106,   109,   136,   138,    88,
       0,    85,    95,    71,    39,    41,    42,    43,    44,    45,
      38,     0,     0,    79,    89,    91,    82,     0,    47,     0,
       0,     0,    37,    90,    92,    88,    83
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    10,    16,    37,    17,    20,    22,    23,
      24,    40,    41,    42,   155,   113,    51,     7,     9,    75,
     115,   116,   157,   210,   160,   212,   161,   121,    29,    30,
      31,    35,    36,    81,    44,    52,    55,    47,    65,   153,
     154,    84,    89,   168,   132,   200,   221,    91,   216,   219,
     220,    67,    68,   139,   106,    69,    83,   122,    70,   123,
     162,   124,   125,   163,    98,    99,   100,   101,   102,   135,
      71,   128,   130
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -172
static const yytype_int16 yypact[] =
{
    -172,    35,    50,  -172,    -1,    34,    43,  -172,  -172,    10,
      52,  -172,    59,    47,  -172,    92,    73,   131,  -172,  -172,
     113,  -172,    80,  -172,   133,   114,  -172,  -172,  -172,  -172,
      80,   119,  -172,  -172,  -172,    67,    67,   134,   139,  -172,
     135,  -172,   136,  -172,   142,   140,    12,  -172,  -172,  -172,
    -172,    33,   147,    31,    47,  -172,  -172,  -172,   116,   148,
     149,  -172,   141,  -172,    57,    20,  -172,  -172,   103,  -172,
    -172,  -172,   143,    66,   145,  -172,  -172,  -172,  -172,  -172,
    -172,   140,  -172,     9,   150,  -172,  -172,    57,   146,    57,
    -172,    12,    57,  -172,  -172,     5,  -172,  -172,    98,    76,
     102,   -11,  -172,    12,  -172,    -3,  -172,  -172,  -172,  -172,
    -172,  -172,  -172,  -172,  -172,   151,    68,  -172,  -172,  -172,
    -172,  -172,  -172,  -172,  -172,  -172,  -172,    98,   128,  -172,
     130,    98,   152,     1,    18,   156,    76,     5,     5,   111,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,  -172,    12,  -172,  -172,   154,  -172,    71,  -172,   145,
     132,    57,    12,    57,  -172,    57,  -172,   153,   121,  -172,
      57,  -172,  -172,    76,    76,    -3,   -11,   -11,   -11,   -11,
     -11,   -11,   -11,   -11,  -172,  -172,  -172,    23,  -172,   155,
      74,   151,  -172,  -172,    98,  -172,    98,    98,  -172,    -3,
      93,    98,  -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,
    -172,   110,    57,  -172,  -172,  -172,  -172,    74,    98,     5,
       5,   125,  -172,   102,   102,    -3,  -172
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -172,  -172,  -172,  -172,   109,  -172,  -172,  -172,  -172,  -172,
    -172,  -172,   137,  -172,  -172,  -172,  -172,  -172,  -172,  -172,
      11,  -172,   -20,   -44,  -172,  -172,  -172,  -172,  -172,  -172,
     144,  -172,  -172,  -172,   157,  -172,    91,  -172,   -88,  -171,
     -45,  -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,
    -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,    94,
    -172,    40,    44,  -172,   -87,   -89,   -78,   -54,   -35,  -172,
    -172,  -172,  -172
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -20
static const yytype_int16 yytable[] =
{
     127,    66,   131,   133,   202,   134,   136,   152,   103,    92,
      56,    57,     5,   118,    58,    11,    12,   119,    93,    94,
     148,   149,   150,   171,   120,    56,    57,   103,   213,    58,
     103,   104,    96,    97,   203,     3,    59,    60,     6,    72,
      61,    73,    62,   137,   138,    63,    66,    64,   173,   174,
     170,    59,    60,     4,   226,    61,     8,    62,   151,    13,
      63,    92,    64,    15,   187,    76,    77,    78,    79,    80,
      93,    94,    14,   158,   194,   159,   196,   189,   197,   190,
      43,    95,    21,   201,    96,    97,   176,   177,   178,   179,
     180,   181,   182,   183,   140,   141,   142,   143,   144,   145,
     108,   109,   110,   111,   112,    18,    19,    66,   205,   206,
     207,   208,   209,   184,   185,   186,   189,   195,   217,    25,
      26,    27,    28,   137,   138,   218,    39,    33,    34,    85,
      86,   146,   147,   164,   165,   166,   167,   192,   193,   214,
     215,   223,   224,   -14,    46,    32,    48,    54,   -19,    50,
      53,    74,    87,    88,    90,   105,   107,   114,   119,   129,
     118,   188,   175,    82,   156,   199,   198,   120,   204,   225,
     191,   211,   117,   222,    38,   172,   169,    49,   126,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    45
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-172))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      87,    46,    89,    91,   175,    92,    95,    10,     7,     4,
      13,    14,    13,     4,    17,     5,     6,     8,    13,    14,
      31,    32,    33,     5,    15,    13,    14,     7,   199,    17,
       7,    11,    27,    28,    11,     0,    39,    40,     4,     6,
      43,     8,    45,    25,    26,    48,    91,    50,   137,   138,
      49,    39,    40,     3,   225,    43,    13,    45,   103,     7,
      48,     4,    50,    16,   152,    34,    35,    36,    37,    38,
      13,    14,    13,     5,   161,     7,   163,     6,   165,     8,
      13,    24,     9,   170,    27,    28,   140,   141,   142,   143,
     144,   145,   146,   147,    18,    19,    20,    21,    22,    23,
      34,    35,    36,    37,    38,    13,    14,   152,    34,    35,
      36,    37,    38,   148,   149,   150,     6,   162,     8,     6,
       7,    41,    42,    25,    26,   212,     7,    13,    14,    13,
      14,    29,    30,     5,     6,     5,     6,     5,     6,    46,
      47,   219,   220,    12,    10,    12,     7,     7,    13,    13,
       8,     4,     4,     4,    13,    52,    13,    12,     8,    13,
       4,     7,    51,    54,    13,    44,    13,    15,    13,    44,
     159,   191,    81,   217,    30,   135,   132,    40,    84,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    55,    56,     0,     3,    13,     4,    71,    13,    72,
      57,     5,     6,     7,    13,    16,    58,    60,    13,    14,
      61,     9,    62,    63,    64,     6,     7,    41,    42,    82,
      83,    84,    12,    13,    14,    85,    86,    59,    84,     7,
      65,    66,    67,    13,    88,    88,    10,    91,     7,    66,
      13,    70,    89,     8,     7,    90,    13,    14,    17,    39,
      40,    43,    45,    48,    50,    92,    94,   105,   106,   109,
     112,   124,     6,     8,     4,    73,    34,    35,    36,    37,
      38,    87,    58,   110,    95,    13,    14,     4,     4,    96,
      13,   101,     4,    13,    14,    24,    27,    28,   118,   119,
     120,   121,   122,     7,    11,    52,   108,    13,    34,    35,
      36,    37,    38,    69,    12,    74,    75,    90,     4,     8,
      15,    81,   111,   113,   115,   116,   113,   118,   125,    13,
     126,   118,    98,    92,   118,   123,   119,    25,    26,   107,
      18,    19,    20,    21,    22,    23,    29,    30,    31,    32,
      33,    94,    10,    93,    94,    68,    13,    76,     5,     7,
      78,    80,   114,   117,     5,     6,     5,     6,    97,   116,
      49,     5,   115,   119,   119,    51,   121,   121,   121,   121,
     121,   121,   121,   121,   122,   122,   122,    92,     7,     6,
       8,    74,     5,     6,   118,    94,   118,   118,    13,    44,
      99,   118,    93,    11,    13,    34,    35,    36,    37,    38,
      77,    76,    79,    93,    46,    47,   102,     8,   118,   103,
     104,   100,    77,   120,   120,    44,    93
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 49 "compilador.y"
    { 
		ts = inicializa();
		pt = inicializaPdT();
		pr = inicializaPdR();
		pfunc = inicializaPdF();
		geraCodigo (NULL, "INPP"); 
	}
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 55 "compilador.y"
    {
		num_param = 0;
	}
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 57 "compilador.y"
    {
		int i = 0;
		do {
        	elemento = desempilha(ts);
        } while( elemento != NULL && (elemento->categoria == Proc || elemento->categoria == Func)  && elemento->nivel > nivel);
        ts->topo++;
		while( elemento != NULL){
        	elemento = desempilha(ts);
        	num_vars++;
        }
        sprintf(str,"DMEM %d", num_vars);
		geraCodigo (NULL, str);
		geraCodigo (NULL, "PARA");
    }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 77 "compilador.y"
    { 
	}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 86 "compilador.y"
    { 
			if( !empilha(ts, RT, nivel, -1, token) ){
				sprintf(str,"%s ja existe", token);
            	imprimeErro(str);
            }
		}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 92 "compilador.y"
    {
			if( !empilha(ts, RT, nivel, -1, token) ){
				sprintf(str,"%s ja existe", token);
            	imprimeErro(str);
            }
		}
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 98 "compilador.y"
    {
			if( !empilha(ts, RT, nivel, -1, token) ){
				sprintf(str,"%s ja existe", token);
            	imprimeErro(str);
            }
		}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 104 "compilador.y"
    {
			if( !empilha(ts, RT, nivel, -1, token) ){
				sprintf(str,"%s ja existe", token);
            	imprimeErro(str);
            }
		}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 112 "compilador.y"
    { 
				sprintf(str, "AMEM %d", num_vars);
				geraCodigo(NULL, str);
                num_vars = 0;
              }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 119 "compilador.y"
    { }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 127 "compilador.y"
    { }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 129 "compilador.y"
    {num_tipo = 0;}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 133 "compilador.y"
    {insereTipo(ts, Int, num_tipo);}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 134 "compilador.y"
    {insereTipo(ts, Real, num_tipo);}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 135 "compilador.y"
    {insereTipo(ts, Str, num_tipo);}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 136 "compilador.y"
    {insereTipo(ts, Chr, num_tipo);}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 137 "compilador.y"
    {insereTipo(ts, Bool, num_tipo);}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 140 "compilador.y"
    {
              	/* insere última vars na tabela de símbolos */
              	if( !empilha(ts, VS, nivel, num_vars, token) ){
					sprintf(str,"%s ja existe", token);
              		imprimeErro(str);
              	}
              	num_tipo++;
              	num_vars++;
            }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 149 "compilador.y"
    { 
    			/* insere última vars na tabela de símbolos */
    			/* insere vars na tabela de símbolos */
		      	if( !empilha(ts, VS, nivel, num_vars, token) ){
					sprintf(str,"%s ja existe", token);
		      		imprimeErro(str);
		      	}
		      	num_tipo++;
    			num_vars++; 
    		}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 171 "compilador.y"
    {params_cont = 0;num_tipo = 0;}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 175 "compilador.y"
    {eh_ref = 1;}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 179 "compilador.y"
    {eh_ref = 0; num_tipo = 0;}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 180 "compilador.y"
    {eh_ref = 0; num_tipo = 0;}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 183 "compilador.y"
    {
				if( !empilha(ts, PF, nivel, - 4 - num_param, token) ){
					sprintf(str,"%s ja existe", token);
		      		imprimeErro(str);
		      	}				
				num_param++;
				num_tipo++;
			}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 191 "compilador.y"
    {
				if( !empilha(ts, PF, nivel, - 4 - num_param, token) ){
					sprintf(str,"%s ja existe", token);
		      		imprimeErro(str);
		      	}				
				num_param++;
				num_tipo++;
			}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 201 "compilador.y"
    {
				if (eh_ref)
					insereTipoParam(ts, RefInt, num_tipo);
				else
					insereTipoParam(ts, Int, num_tipo); 
			}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 207 "compilador.y"
    {
				if (eh_ref)
					insereTipoParam(ts, RefReal, num_tipo);
				else
					insereTipoParam(ts, Real, num_tipo); 
			}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 213 "compilador.y"
    {
				if (eh_ref)
					insereTipoParam(ts, RefStr, num_tipo);
				else
					insereTipoParam(ts, Str, num_tipo); 
			}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 219 "compilador.y"
    {
				if (eh_ref)
					insereTipoParam(ts, RefChr, num_tipo);
				else
					insereTipoParam(ts, Chr, num_tipo); 
			}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 225 "compilador.y"
    {
				if (eh_ref)
					insereTipoParam(ts, RefBool, num_tipo);
				else
					insereTipoParam(ts, Bool, num_tipo); 
			}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 233 "compilador.y"
    { pfunc->topo->ind_param++; }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 233 "compilador.y"
    {
				int aux = desempilhaPdT(pt);
				if( aux != pfunc->topo->func->params[pfunc->topo->ind_param]) {
					sprintf(str,"Tipo de parametro-%d nao compativel com a proc/func: %s()", pfunc->topo->ind_param, pfunc->topo->func->id);
					imprimeErro(str);
				}
				
			}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 241 "compilador.y"
    { pfunc->topo->ind_param++; }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 241 "compilador.y"
    {
				int aux = desempilhaPdT(pt);
				if( aux != pfunc->topo->func->params[pfunc->topo->ind_param]) {
					sprintf(str,"Tipo de parametro-%d nao compativel com a proc/func: %s()", pfunc->topo->ind_param, pfunc->topo->func->id);
					imprimeErro(str);
				}
				
			}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 251 "compilador.y"
    { }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 263 "compilador.y"
    {proc_func = Func;}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 264 "compilador.y"
    {proc_func = Proc;}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 267 "compilador.y"
    {tipo_func = Int;}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 268 "compilador.y"
    {tipo_func = Real;}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 269 "compilador.y"
    {tipo_func = Str;}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 270 "compilador.y"
    {tipo_func = Chr;}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 271 "compilador.y"
    {tipo_func = Bool;}
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 274 "compilador.y"
    {
        nivel++;
		sprintf(str, "DSVS ROT%d", rot);
		empilhaPdR(pr, rot); 
		geraCodigo(NULL ,str);
        rot++;
		sprintf(str, "ROT%d", rot);
		geraCodigo(str, "NADA");
		if( !empilha(ts, proc_func, nivel, rot, token) ){
			sprintf(str,"%s ja existe", token);
      		imprimeErro(str);
      	}
		rot++;
		sprintf(str, "ENPR %d", nivel);
		geraCodigo(NULL, str);
		elemento = ts->topo;
	}
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 290 "compilador.y"
    {
		elemento->num_param = num_param;
		if( elemento->num_param > 10)
			imprimeErro("Numero de parametros superior a 10");
		
		preencheParams(ts, elemento, proc_func);
		elemento = NULL;
		num_param = 0;
	}
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 301 "compilador.y"
    {
		int num_vars_aux = 0;
		do {
        	elemento = desempilha(ts);
        } while( (elemento->categoria == Proc || elemento->categoria == Func) && elemento->nivel > nivel);
        ts->topo++;
		do{
			elemento = desempilha(ts);
        	num_vars_aux++;
        }while( elemento->categoria == VS);
		ts->topo++;
		do {
			elemento = desempilha(ts);
        } while( elemento->categoria == RT);
        ts->topo++;
		do {
			elemento = desempilha(ts);
        } while( elemento->categoria == PF);
       
        if( elemento->categoria != Proc || elemento->categoria != Func )
     		ts->topo++;
        sprintf(str,"DMEM %d", --num_vars_aux);
        geraCodigo (NULL, str);
        elemento->tipo = tipo_func;
        sprintf(str,"RTPR %d, %d", nivel, elemento->num_param);
        geraCodigo (NULL, str);
		sprintf(str, "ROT%d", desempilhaPdR(pr));
		geraCodigo(str, "NADA");
		nivel--;
		elemento = NULL;
    }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 351 "compilador.y"
    {
			elemento = buscaTs(ts, nivel, token);
			if(elemento == NULL) {
				sprintf(str,"Simbolo não encontrado: %s", token);
				imprimeErro(str);
			}
		}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 359 "compilador.y"
    { 
			empilhaPdR(pr, rot); 
			sprintf(str, "ROT%d", rot);
			geraCodigo(str, "NADA");
			rot++;
		}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 364 "compilador.y"
    {
			if( desempilhaPdT(pt) != Bool) {
				sprintf(str, "Operacao nao permitida: tipo incompativel");
				imprimeErro(str);
			}
			sprintf(str, "DSVF ROT%d", rot);
			empilhaPdR(pr, rot); 
			geraCodigo(NULL ,str);
			rot++;			
		}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 373 "compilador.y"
    {
			aux = desempilhaPdR(pr);
			sprintf(str, "DSVS ROT%d", desempilhaPdR(pr));
			geraCodigo(NULL, str);
			sprintf(str, "ROT%d", aux);
			geraCodigo(str, "NADA"); 
		}
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 380 "compilador.y"
    {
			strcpy(id, token);
			elemento = buscaTs(ts, nivel, id);
			if(elemento == NULL) {
				sprintf(str,"Simbolo não encontrado: %s", id);
				imprimeErro(str);
			}
			empilhaPdF(pfunc, -1, elemento);
			empilhaPdT(pt, elemento->tipo); 
		}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 389 "compilador.y"
    { 
			sprintf(str, "CRVL %d,%d", auxNiv, auxDes); 
			geraCodigo(NULL, str);
		}
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 392 "compilador.y"
    {
			sprintf(str, "DSVF ROT%d", rot);
			empilhaPdR(pr, rot); 
			geraCodigo(NULL ,str);
			rot++;
		}
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 397 "compilador.y"
    {
			aux = desempilhaPdR(pr);
			sprintf(str, "DSVS ROT%d", desempilhaPdR(pr));
			geraCodigo(NULL, str);
			sprintf(str, "ROT%d", aux);
			geraCodigo(str, "NADA"); 
		}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 404 "compilador.y"
    {
			empilhaPdR(pr, rot); 
			sprintf(str, "ROT%d", rot);
			geraCodigo(str, "NADA");
			rot++;
		}
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 409 "compilador.y"
    {
			if( desempilhaPdT(pt) != Bool) {
				sprintf(str, "Operacao nao permitida: tipo incompativel");
				imprimeErro(str);
			}
			sprintf(str, "DSVF ROT%d", desempilhaPdR(pr));
			geraCodigo(NULL ,str);
		}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 422 "compilador.y"
    {
			geraCodigo(NULL, "CRCT 1");
			geraCodigo(NULL, "SUBT");
			sprintf(str, "ARMZ %d,%d", auxNiv, auxDes); 
			geraCodigo(NULL, str);
			empilhaPdR(pr, rot); 
			sprintf(str, "ROT%d", rot);
			geraCodigo(str, "NADA");
			rot++;
			sprintf(str, "CRVL %d,%d", auxNiv, auxDes); 
			geraCodigo(NULL, str);
			geraCodigo(NULL, "CRCT 1");
			geraCodigo(NULL, "SOMA");
			sprintf(str, "ARMZ %d,%d", auxNiv, auxDes);
			geraCodigo(NULL, str);
			sprintf(str, "CRVL %d,%d", auxNiv, auxDes); 
			geraCodigo(NULL, str);
		}
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 439 "compilador.y"
    {
			geraCodigo(NULL, "CMME");	
		}
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 442 "compilador.y"
    {
			geraCodigo(NULL, "CRCT 1");
			geraCodigo(NULL, "SOMA");
			sprintf(str, "ARMZ %d,%d", auxNiv, auxDes); 
			geraCodigo(NULL, str);
			empilhaPdR(pr, rot); 
			sprintf(str, "ROT%d", rot);
			geraCodigo(str, "NADA");
			rot++;
			sprintf(str, "CRVL %d,%d", auxNiv, auxDes); 
			geraCodigo(NULL, str);
			geraCodigo(NULL, "CRCT 1");
			geraCodigo(NULL, "SUBT");
			sprintf(str, "ARMZ %d,%d", auxNiv, auxDes);
			geraCodigo(NULL, str);
			sprintf(str, "CRVL %d,%d", auxNiv, auxDes); 
			geraCodigo(NULL, str);
		}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 459 "compilador.y"
    {
			geraCodigo(NULL, "CMMA");	
		}
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 465 "compilador.y"
    {
			sprintf(str, "ROT%d", desempilhaPdR(pr));
			geraCodigo(str, "NADA");
		}
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 471 "compilador.y"
    {
			if( desempilhaPdT(pt) != Bool) {
				sprintf(str, "Operacao nao permitida: tipo incompativel");
				imprimeErro(str);
			}
			empilhaPdR(pr, rot);
			sprintf(str, "DSVF ROT%d", rot);
			geraCodigo(NULL ,str);
			rot++;
		}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 480 "compilador.y"
    {
			int aux = desempilhaPdR(pr);
			empilhaPdR(pr, rot);
			sprintf(str, "DSVS ROT%d", rot);
			geraCodigo(NULL ,str);
			rot++;
			sprintf(str, "ROT%d", aux);
			geraCodigo(str, "NADA");
		}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 495 "compilador.y"
    {
			strcpy(id, token);
			elemento = buscaTs(ts, nivel, id);
			if(elemento == NULL) {
				sprintf(str,"Simbolo não encontrado: %s", id);
				imprimeErro(str);
			}
			if(elemento->categoria != Proc && elemento->categoria != RT)
				empilhaPdT(pt, elemento->tipo);
			empilhaPdF(pfunc, -1, elemento);
		 }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 514 "compilador.y"
    {
		elemento = buscaTs(ts, nivel, token);
		if(elemento == NULL) {
			sprintf(str,"Simbolo não encontrado: %s", token);
			imprimeErro(str);
		}
		sprintf(str,"DSVR %s, %d, %d", elemento->id, elemento->nivel, nivel);
		geraCodigo(NULL, str);
		
	}
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 524 "compilador.y"
    {
		elemento = buscaTs(ts, nivel, token);
		if(elemento == NULL) {
			sprintf(str,"Simbolo não encontrado: %s", token);
			imprimeErro(str);
		}
		sprintf(str,"DSVR %s, %d, %d", elemento->id, elemento->nivel, nivel);
		geraCodigo(NULL, str);
	}
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 535 "compilador.y"
    {
		if(pfunc->topo != pfunc->elementos - 1 ){
			desempilhaPdF(pfunc);
		}
		if(elemento->deslocamento == 0){
			sprintf(str,"rotulo ja declarado: %s", elemento->id);
			imprimeErro(str);
		}
		elemento->deslocamento++;
		sprintf(str, "ENRT %d, %d", nivel, numVar(ts, nivel));	
		geraCodigo(elemento->id, str);
		elemento = NULL;
	}
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 550 "compilador.y"
    {
			EPdF *func = desempilhaPdF(pfunc);
				
			if( func->func->categoria == Func || func->func->categoria == Proc ){
				if(func->func->categoria == Func) {
					geraCodigo(NULL, "AMEM 1");
				}
				sprintf(str, "CHPR ROT%d, %d", func->func->deslocamento, nivel);
				geraCodigo(NULL, str);
				
				if(func->ind_param + 1 != func->func->num_param){
				sprintf(str, "Numero de parametros invalidos para função %s", func->func->id);
				imprimeErro(str);
			}
		}
	}
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 568 "compilador.y"
    {
				if(pfunc->topo != pfunc->elementos - 1 )
					desempilhaPdF(pfunc);
	}
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 571 "compilador.y"
    {
			int deslocamento = elemento->deslocamento;
			if( elemento->categoria == Proc ){
				sprintf(str,"Procedimento nao possui retorno");
				imprimeErro(str);
			}	
			if(elemento->categoria == Func){
				if(!verificaRetorno(ts, elemento, nivel)) {
					sprintf(str,"Não eh possivel fazer o retorno da funcao %s nesse local", elemento->id);
					imprimeErro(str);
				}
			}
			if( elemento->tipo > Bool )
				sprintf(str, "ARMI %d,%d", elemento->nivel, elemento->deslocamento);
			else if( elemento->categoria == Func ){
				sprintf(str, "ARMZ %d,%d", elemento->nivel, -4 - elemento->num_param );
				deslocamento = -4 - elemento->num_param;
			} else
		 		sprintf(str, "ARMZ %d,%d", elemento->nivel, elemento->deslocamento); 
	 		verificaTipo(pt, str);
	 		auxNiv = elemento->nivel;
			auxDes = deslocamento;
	 		elemento = NULL;		
	}
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 600 "compilador.y"
    {verificaTipo(pt, "CONJ"); empilhaPdT(pt, Bool);}
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 601 "compilador.y"
    {verificaTipo(pt, "DISJ"); empilhaPdT(pt, Bool);}
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 602 "compilador.y"
    {
		if( Bool != desempilhaPdT(pt)){
			sprintf(str,"Operação não permitida: tipos incompatíveis");
			imprimeErro(str);
		}
		geraCodigo(NULL, "NEGA");
		empilhaPdT(pt, Bool);
	}
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 613 "compilador.y"
    {verificaTipo(pt, "CMIG"); empilhaPdT(pt, Bool);}
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 614 "compilador.y"
    {verificaTipo(pt, "CMDG"); empilhaPdT(pt, Bool);}
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 615 "compilador.y"
    {verificaTipo(pt, "CMMA"); empilhaPdT(pt, Bool);}
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 616 "compilador.y"
    {verificaTipo(pt, "CMME"); empilhaPdT(pt, Bool);}
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 617 "compilador.y"
    {verificaTipo(pt, "CMAG"); empilhaPdT(pt, Bool);}
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 618 "compilador.y"
    {verificaTipo(pt, "CMEG"); empilhaPdT(pt, Bool);}
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 622 "compilador.y"
    {verificaTipo(pt, "SOMA"); empilhaPdT(pt, Int);}
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 623 "compilador.y"
    {verificaTipo(pt, "SUBT"); empilhaPdT(pt, Int);}
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 627 "compilador.y"
    {verificaTipo(pt, "MULT"); empilhaPdT(pt, Int);}
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 628 "compilador.y"
    {verificaTipo(pt, "DIVI"); empilhaPdT(pt, Int);}
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 629 "compilador.y"
    {verificaTipo(pt, "DIVI"); empilhaPdT(pt, Int);}
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 633 "compilador.y"
    {
		empilhaPdT (pt, Bool);
		sprintf(str, "CRCT 1");
		geraCodigo(NULL, str);
	 }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 638 "compilador.y"
    {
		empilhaPdT (pt, Bool);
		sprintf(str, "CRCT 0");
		geraCodigo(NULL, str);
	 }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 643 "compilador.y"
    {
		empilhaPdT (pt, Int);
		sprintf(str, "CRCT %s", token);
		geraCodigo(NULL, str);
	 }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 648 "compilador.y"
    {
	 	int tipo;
  		elemento2 = buscaTs(ts, nivel, token);
  		if(elemento2 == NULL) {
			sprintf(str,"Simbolo nao encontrado: %s", token);
			imprimeErro(str);
		}
		
		if(elemento2->categoria == Proc){
			sprintf(str,"Procedimento nao pode fazer parte de uma expressao");
			imprimeErro(str);
		}
		if(elemento2->categoria != Func ){
			if(elemento2->tipo > Bool) {
				 if((pfunc->topo != pfunc->elementos - 1) && pfunc->topo->func->params[pfunc->topo->ind_param] > Bool){
					sprintf(str, "CRVL %d,%d", elemento2->nivel, elemento2->deslocamento);
					tipo = elemento2->tipo;
				} else {
					sprintf(str, "CRVI %d,%d", elemento2->nivel, elemento2->deslocamento);
					tipo = elemento2->tipo - 5;
				}
			} else if(pfunc->topo != pfunc->elementos - 1 && pfunc->topo->func->params[pfunc->topo->ind_param] > Bool){
					sprintf(str, "CREN %d,%d", elemento2->nivel, elemento2->deslocamento);
					tipo = elemento2->tipo + 5;
				} else {
					sprintf(str, "CRVL %d,%d", elemento2->nivel, elemento2->deslocamento);
					tipo = elemento2->tipo;
				}
	
			geraCodigo(NULL, str);
		}else {
			tipo = elemento2->tipo;
		}
		empilhaPdF(pfunc, -1, elemento2);
		empilhaPdT (pt, tipo);
	 }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 692 "compilador.y"
    {geraCodigo(NULL, "IMPR");}
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 693 "compilador.y"
    {geraCodigo(NULL, "IMPR");}
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 696 "compilador.y"
    {
		strcpy(id,token);
		elemento = buscaTs(ts, nivel, id);
		if(elemento == NULL) {
			sprintf(str,"Simbolo não encontrado: %s", id);
			imprimeErro(str);
		}
		geraCodigo(NULL, "LEIT");
		sprintf(str, "ARMZ %d,%d", elemento->nivel, elemento->deslocamento);
		geraCodigo(NULL, str);
		elemento = NULL;
	}
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 708 "compilador.y"
    {
		strcpy(id,token);
		elemento = buscaTs(ts, nivel, id);
		if(elemento == NULL) {
			sprintf(str,"Simbolo não encontrado: %s", id);
			imprimeErro(str);
		}
		geraCodigo(NULL, "LEIT");
		sprintf(str, "ARMZ %d,%d", elemento->nivel, elemento->deslocamento);
		geraCodigo(NULL, str);
		elemento = NULL;
	}
    break;



/* Line 1806 of yacc.c  */
#line 2728 "compilador.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 722 "compilador.y"


main (int argc, char** argv) {
   FILE* fp;
   extern FILE* yyin;

   if (argc<2 || argc>2) {
         printf("usage compilador <arq>a %d\n", argc);
         return(-1);
      }

   fp=fopen (argv[1], "r");
   if (fp == NULL) {
      printf("usage compilador <arq>b\n");
      return(-1);
   }

/* -------------------------------------------------------------------
 *  Inicia a Tabela de Símbolos
 * ------------------------------------------------------------------- */

   yyin=fp;
   yyparse();

   return 0;
}

