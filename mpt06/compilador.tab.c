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
#line 6 "compilador.y"

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"

int num_vars = 0, nivel_lexico = 0, aux, numRotulo = -1, ref = 0, proc = 0, busy = 0, varlocal = 0, r;
char str[TAM], ident[TAM], inicio[TAM], fim[TAM];
tabela ts;
pilhaTipo pt;
pilhaRotulo pr;
pilhaAux pa;
elemenTS *element, *p;



/* Line 268 of yacc.c  */
#line 89 "compilador.tab.c"

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
     GOTO = 261,
     VIRGULA = 262,
     PONTO_E_VIRGULA = 263,
     DOIS_PONTOS = 264,
     PONTO = 265,
     T_BEGIN = 266,
     T_END = 267,
     VAR = 268,
     IDENT = 269,
     ATRIBUICAO = 270,
     FALSE = 271,
     TRUE = 272,
     MAIS = 273,
     MENOS = 274,
     MULT = 275,
     DIV = 276,
     NOT = 277,
     AND = 278,
     OR = 279,
     MAIOR = 280,
     MENOR = 281,
     IGUAL = 282,
     DIFERENTE = 283,
     NUMERO = 284,
     READ = 285,
     WRITE = 286,
     WRITELN = 287,
     INTEGER = 288,
     BOOLEAN = 289,
     STRING = 290,
     WHILE = 291,
     DO = 292,
     REPEAT = 293,
     UNTIL = 294,
     FOR = 295,
     TO = 296,
     DOWNTO = 297,
     IF = 298,
     THEN = 299,
     ELSE = 300,
     PROCEDURE = 301,
     FUNCTION = 302,
     LABEL = 303,
     LOWER_THAN_ELSE = 304
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
#line 180 "compilador.tab.c"

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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   203

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  95
/* YYNRULES -- Number of rules.  */
#define YYNRULES  159
/* YYNRULES -- Number of states.  */
#define YYNSTATES  257

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   304

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
      45,    46,    47,    48,    49
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     5,    16,    20,    22,    24,    25,
      33,    35,    36,    39,    41,    45,    49,    51,    54,    57,
      58,    60,    61,    64,    66,    69,    73,    76,    77,    82,
      86,    88,    90,    92,    94,    96,    97,   100,   101,   104,
     106,   107,   108,   116,   117,   118,   128,   129,   133,   137,
     138,   140,   141,   145,   147,   148,   152,   154,   155,   160,
     164,   166,   168,   170,   172,   174,   176,   178,   180,   184,
     186,   190,   192,   194,   196,   198,   200,   202,   204,   206,
     207,   210,   213,   216,   217,   218,   222,   223,   227,   230,
     232,   233,   240,   241,   245,   246,   250,   252,   253,   257,
     259,   261,   262,   263,   266,   270,   274,   276,   280,   284,
     288,   293,   297,   302,   307,   309,   313,   317,   319,   323,
     327,   329,   330,   334,   336,   338,   340,   344,   345,   352,
     353,   358,   363,   367,   369,   371,   376,   380,   382,   384,
     386,   388,   390,   391,   392,   399,   400,   406,   407,   408,
     418,   419,   423,   424,   428,   431,   432,   438,   439,   443
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      51,     0,    -1,    -1,    -1,     3,    14,    52,     4,    54,
      53,     5,     8,    56,    10,    -1,    54,     7,    55,    -1,
      55,    -1,    14,    -1,    -1,    58,    64,    57,    73,    11,
      95,    12,    -1,    59,    -1,    -1,    59,    60,    -1,    60,
      -1,    48,    61,     8,    -1,    61,     7,    62,    -1,    62,
      -1,    14,    63,    -1,    29,    63,    -1,    -1,    65,    -1,
      -1,    65,    66,    -1,    66,    -1,    13,    67,    -1,    67,
      68,     8,    -1,    68,     8,    -1,    -1,    69,    70,     9,
      72,    -1,    70,     7,    71,    -1,    71,    -1,    14,    -1,
      33,    -1,    34,    -1,    35,    -1,    -1,    74,    75,    -1,
      -1,    75,    76,    -1,    76,    -1,    -1,    -1,    46,    77,
      81,     8,    56,    78,     8,    -1,    -1,    -1,    47,    79,
      81,     9,    93,     8,    56,    80,     8,    -1,    -1,    14,
      82,    83,    -1,     4,    84,     5,    -1,    -1,    85,    -1,
      -1,    85,     8,    86,    -1,    86,    -1,    -1,    13,    87,
      88,    -1,    88,    -1,    -1,    89,    90,     9,    92,    -1,
      90,     7,    91,    -1,    91,    -1,    14,    -1,    33,    -1,
      34,    -1,    35,    -1,    33,    -1,    34,    -1,    35,    -1,
      11,    95,    12,    -1,    96,    -1,    95,     8,    96,    -1,
      96,    -1,    97,    -1,   100,    -1,   102,    -1,   122,    -1,
     125,    -1,   128,    -1,   140,    -1,    -1,     6,    98,    -1,
      14,    99,    -1,    29,    99,    -1,    -1,    -1,    29,   101,
     107,    -1,    -1,    14,   103,   104,    -1,    15,   114,    -1,
     107,    -1,    -1,     4,   105,   112,   109,   113,     5,    -1,
      -1,   106,   112,   113,    -1,    -1,     9,   108,    96,    -1,
     110,    -1,    -1,   110,     7,   111,    -1,   111,    -1,   114,
      -1,    -1,    -1,    22,   115,    -1,   114,    23,   115,    -1,
     114,    24,   115,    -1,   115,    -1,   115,    25,   116,    -1,
     115,    26,   116,    -1,   115,    27,   116,    -1,   115,    26,
      25,   116,    -1,   115,    28,   116,    -1,   115,    25,    27,
     116,    -1,   115,    26,    27,   116,    -1,   116,    -1,   116,
      18,   117,    -1,   116,    19,   117,    -1,   117,    -1,   117,
      20,   118,    -1,   117,    21,   118,    -1,   118,    -1,    -1,
      14,   119,   120,    -1,    29,    -1,    17,    -1,    16,    -1,
       4,   114,     5,    -1,    -1,     4,   121,   112,   109,   113,
       5,    -1,    -1,    31,     4,   123,     5,    -1,    32,     4,
     123,     5,    -1,   123,     7,   124,    -1,   124,    -1,   114,
      -1,    30,     4,   126,     5,    -1,   126,     7,   127,    -1,
     127,    -1,    14,    -1,   129,    -1,   132,    -1,   134,    -1,
      -1,    -1,    36,   130,   114,   131,    37,    94,    -1,    -1,
      38,   133,    95,    39,   114,    -1,    -1,    -1,    40,    14,
     135,    15,   114,   136,   137,    37,    94,    -1,    -1,   138,
      41,   114,    -1,    -1,   139,    42,   114,    -1,   141,   143,
      -1,    -1,    43,   114,   142,    44,    94,    -1,    -1,    45,
     144,    94,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    36,    36,    46,    35,    58,    59,    62,    67,    65,
      86,    87,    90,    91,    94,    97,    98,   101,   102,   105,
     114,   115,   118,   119,   122,   125,   126,   129,   129,   133,
     134,   137,   152,   153,   154,   157,   157,   176,   179,   180,
     184,   186,   183,   194,   196,   193,   207,   206,   232,   233,
     236,   238,   241,   242,   246,   245,   249,   252,   252,   256,
     257,   260,   274,   280,   286,   294,   296,   298,   302,   303,
     307,   308,   311,   312,   313,   314,   315,   316,   317,   318,
     321,   324,   325,   328,   340,   339,   345,   344,   357,   381,
     383,   382,   388,   388,   396,   395,   411,   412,   415,   416,
     419,   429,   439,   454,   461,   468,   475,   478,   485,   492,
     499,   506,   513,   520,   527,   530,   537,   544,   547,   554,
     561,   565,   564,   573,   581,   588,   595,   599,   598,   606,
     642,   643,   646,   647,   650,   657,   660,   661,   664,   679,
     680,   681,   685,   694,   684,   721,   720,   742,   744,   741,
     772,   772,   805,   805,   842,   853,   852,   868,   867,   883
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PROGRAM", "ABRE_PARENTESES",
  "FECHA_PARENTESES", "GOTO", "VIRGULA", "PONTO_E_VIRGULA", "DOIS_PONTOS",
  "PONTO", "T_BEGIN", "T_END", "VAR", "IDENT", "ATRIBUICAO", "FALSE",
  "TRUE", "MAIS", "MENOS", "MULT", "DIV", "NOT", "AND", "OR", "MAIOR",
  "MENOR", "IGUAL", "DIFERENTE", "NUMERO", "READ", "WRITE", "WRITELN",
  "INTEGER", "BOOLEAN", "STRING", "WHILE", "DO", "REPEAT", "UNTIL", "FOR",
  "TO", "DOWNTO", "IF", "THEN", "ELSE", "PROCEDURE", "FUNCTION", "LABEL",
  "LOWER_THAN_ELSE", "$accept", "programa", "$@1", "$@2", "lista_idents",
  "ident", "bloco", "$@3", "parte_declara_labels", "parte_declara_label",
  "label", "lista_id_labels", "id_label", "insere_label",
  "parte_declara_vars", "parte_declara_var", "var", "declara_vars",
  "declara_var", "$@4", "lista_id_var", "id_var", "tipo",
  "parte_declara_procs", "$@5", "declara_procs", "declara_proc", "$@6",
  "$@7", "$@8", "$@9", "assinatura", "$@10", "declara_params",
  "qtde_params", "lista_params", "passagem_params", "$@11",
  "declara_param", "$@12", "lista_id_param", "param", "tipo_param",
  "tipo_retorno", "comando_composto", "comandos", "comando", "goto",
  "rotulo", "simbora", "rot", "$@13", "atrib_proc_rot", "$@14",
  "proc_ou_atrib", "$@15", "$@16", "goto_dest", "$@17", "lista_parametros",
  "parametros", "parametro", "inicia_params", "finaliza_params",
  "expressao", "logica", "aritmetica", "termo", "fator", "$@18",
  "funcao_ou_nao", "$@19", "write", "escrita", "impr", "read", "leitura",
  "leit", "repetitivo", "while", "$@20", "$@21", "repeat", "$@22", "for",
  "$@23", "$@24", "to_ou_downto", "$@25", "$@26", "if", "if_then", "$@27",
  "could_else", "$@28", 0
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    50,    52,    53,    51,    54,    54,    55,    57,    56,
      58,    58,    59,    59,    60,    61,    61,    62,    62,    63,
      64,    64,    65,    65,    66,    67,    67,    69,    68,    70,
      70,    71,    72,    72,    72,    74,    73,    73,    75,    75,
      77,    78,    76,    79,    80,    76,    82,    81,    83,    83,
      84,    84,    85,    85,    87,    86,    86,    89,    88,    90,
      90,    91,    92,    92,    92,    93,    93,    93,    94,    94,
      95,    95,    96,    96,    96,    96,    96,    96,    96,    96,
      97,    98,    98,    99,   101,   100,   103,   102,   104,   104,
     105,   104,   106,   104,   108,   107,   109,   109,   110,   110,
     111,   112,   113,   114,   114,   114,   114,   115,   115,   115,
     115,   115,   115,   115,   115,   116,   116,   116,   117,   117,
     117,   119,   118,   118,   118,   118,   118,   121,   120,   120,
     122,   122,   123,   123,   124,   125,   126,   126,   127,   128,
     128,   128,   130,   131,   129,   133,   132,   135,   136,   134,
     138,   137,   139,   137,   140,   142,   141,   144,   143,   143
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,    10,     3,     1,     1,     0,     7,
       1,     0,     2,     1,     3,     3,     1,     2,     2,     0,
       1,     0,     2,     1,     2,     3,     2,     0,     4,     3,
       1,     1,     1,     1,     1,     0,     2,     0,     2,     1,
       0,     0,     7,     0,     0,     9,     0,     3,     3,     0,
       1,     0,     3,     1,     0,     3,     1,     0,     4,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       2,     2,     2,     0,     0,     3,     0,     3,     2,     1,
       0,     6,     0,     3,     0,     3,     1,     0,     3,     1,
       1,     0,     0,     2,     3,     3,     1,     3,     3,     3,
       4,     3,     4,     4,     1,     3,     3,     1,     3,     3,
       1,     0,     3,     1,     1,     1,     3,     0,     6,     0,
       4,     4,     3,     1,     1,     4,     3,     1,     1,     1,
       1,     1,     0,     0,     6,     0,     5,     0,     0,     9,
       0,     3,     0,     3,     2,     0,     5,     0,     3,     0
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     1,     0,     0,     7,     3,     6,
       0,     0,     5,     0,    11,     0,     0,    21,    10,    13,
      19,    19,     0,    16,     4,    27,     8,    20,    23,    12,
      17,    18,     0,    14,    24,     0,     0,    35,    22,    15,
       0,    26,    31,     0,    30,     0,     0,    25,     0,     0,
      79,    40,    43,    36,    39,    29,    32,    33,    34,    28,
       0,    86,    84,     0,     0,     0,   142,   145,     0,     0,
       0,    71,    72,    73,    74,    75,    76,    77,   139,   140,
     141,    78,   159,     0,     0,    38,    83,    83,    80,    92,
       0,     0,     0,     0,     0,    79,   147,     0,   121,   125,
     124,     0,   123,   155,   106,   114,   117,   120,    79,     9,
     157,   154,    46,     0,     0,    81,    82,    90,    94,     0,
      87,   101,    89,    85,   138,     0,   137,   134,     0,   133,
       0,   143,     0,     0,     0,   129,   103,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    70,    79,
      49,    11,     0,   101,    79,    88,   102,   135,     0,   130,
       0,   131,     0,     0,     0,   126,   127,   122,   104,   105,
      79,     0,   107,     0,     0,   108,   109,   111,   115,   116,
     118,   119,    79,   158,    69,    51,    47,    41,    65,    66,
      67,     0,    97,    95,    93,   136,   132,    79,   146,   148,
     101,   156,   112,   110,   113,     0,    54,     0,    50,    53,
      56,     0,     0,    11,   102,    96,    99,   100,   144,   150,
      97,    68,    57,    48,    57,    61,     0,    60,    42,    44,
       0,     0,     0,     0,     0,   102,    55,    52,     0,     0,
       0,    91,    98,    79,     0,     0,     0,    59,    62,    63,
      64,    58,    45,   149,   151,   153,   128
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     5,    11,     8,     9,    16,    37,    17,    18,
      19,    22,    23,    30,    26,    27,    28,    34,    35,    36,
      43,    44,    59,    45,    46,    53,    54,    83,   212,    84,
     240,   113,   150,   186,   207,   208,   209,   222,   210,   211,
     226,   227,   251,   191,   183,    70,   184,    72,    88,   115,
      73,    90,    74,    89,   120,   153,   121,   122,   154,   214,
     215,   216,   156,   194,   127,   104,   105,   106,   107,   135,
     167,   200,    75,   128,   129,    76,   125,   126,    77,    78,
      94,   162,    79,    95,    80,   133,   219,   232,   233,   234,
      81,    82,   139,   111,   149
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -200
static const yytype_int16 yypact[] =
{
      15,    21,    29,  -200,  -200,    56,    64,  -200,    79,  -200,
      64,    88,  -200,   102,    76,    27,   111,   123,    76,  -200,
    -200,  -200,    57,  -200,  -200,  -200,  -200,   123,  -200,  -200,
    -200,  -200,    27,  -200,   112,   135,   130,   136,  -200,  -200,
     140,  -200,  -200,    83,  -200,   138,    36,  -200,   130,    73,
      41,  -200,  -200,    36,  -200,  -200,  -200,  -200,  -200,  -200,
      34,  -200,  -200,   146,   148,   149,  -200,  -200,   141,    87,
      49,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,
    -200,  -200,   109,   142,   142,  -200,  -200,  -200,  -200,    65,
     150,   143,    87,    87,    87,    41,  -200,    87,  -200,  -200,
    -200,    10,  -200,   108,   -16,   116,   117,  -200,    41,  -200,
    -200,  -200,  -200,   152,   154,  -200,  -200,  -200,  -200,    87,
    -200,  -200,  -200,  -200,  -200,    12,  -200,   108,    92,  -200,
     106,   108,    -2,   151,    44,   157,   -16,    10,    10,   114,
      98,    71,    10,    10,    10,    10,    10,    10,  -200,     2,
     160,    76,    84,  -200,    41,   108,  -200,  -200,   143,  -200,
      87,  -200,   128,    87,    87,  -200,  -200,  -200,   -16,   -16,
       2,    10,   116,    10,    10,   116,   116,   116,   117,   117,
    -200,  -200,    41,  -200,  -200,   126,  -200,  -200,  -200,  -200,
    -200,   159,    87,  -200,  -200,  -200,  -200,     2,   108,   108,
    -200,  -200,   116,   116,   116,    50,  -200,   163,   161,  -200,
    -200,   156,   164,    76,  -200,   166,  -200,   108,  -200,   129,
      87,  -200,  -200,  -200,   165,  -200,   113,  -200,  -200,  -200,
     169,    87,   144,   139,   137,  -200,  -200,  -200,   156,    95,
     174,  -200,  -200,     2,    87,    87,   172,  -200,  -200,  -200,
    -200,  -200,  -200,  -200,   108,   108,  -200
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -200,  -200,  -200,  -200,  -200,   173,  -147,  -200,  -200,  -200,
     167,  -200,   155,   168,  -200,  -200,   170,  -200,   158,  -200,
    -200,   145,  -200,  -200,  -200,  -200,   131,  -200,  -200,  -200,
    -200,   104,  -200,  -200,  -200,  -200,   -38,  -200,   -32,  -200,
    -200,   -47,  -200,  -200,  -167,   -93,   -49,  -200,  -200,   107,
    -200,  -200,  -200,  -200,  -200,  -200,  -200,   105,  -200,   -24,
    -200,   -33,  -148,  -199,   -69,   -94,  -120,    -3,    -1,  -200,
    -200,  -200,  -200,   110,    39,  -200,  -200,    42,  -200,  -200,
    -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,  -200,
    -200,  -200,  -200,  -200,  -200
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -153
static const yytype_int16 yytable[] =
{
     103,    71,   132,   201,   187,   192,   108,   136,    60,   140,
     141,   142,   143,   182,    97,   230,    61,   157,     1,   158,
     172,   175,   176,   177,    98,   131,    99,   100,   134,     4,
     218,    62,    63,    64,    65,     3,   246,   163,    66,   102,
      67,    20,    68,   168,   169,    69,    71,    60,    86,   165,
     155,   202,   220,   203,   204,    61,    21,   108,   108,   148,
       6,   109,   221,    87,    32,    33,   229,   137,   138,   117,
      62,    63,    64,    65,   118,    97,   253,    66,     7,    67,
     119,    68,    51,    52,    69,    98,    10,    99,   100,   205,
      48,    97,    49,    13,   198,   199,   173,   159,   174,   160,
     102,    98,    97,    99,   100,   193,    56,    57,    58,   101,
      14,   161,    98,   160,    99,   100,   102,   188,   189,   190,
     238,    24,   239,   217,    15,   171,   -27,   102,   248,   249,
     250,   137,   138,    71,   144,   145,    25,   146,   147,   206,
     -57,   178,   179,    41,    42,   180,   181,   -37,    47,    50,
      91,   217,    92,    93,   110,    96,   112,   124,   170,   118,
     151,   166,   217,   152,   185,   197,   164,   213,   223,   224,
     225,  -152,   228,   231,   241,   254,   255,   256,   206,   245,
     244,   243,   252,    12,    85,    29,   237,    39,   114,    31,
     236,   247,    40,    55,   116,   123,   235,    38,   242,   196,
     195,     0,     0,   130
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-200))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      69,    50,    95,   170,   151,   153,     8,   101,     6,    25,
      26,    27,    28,    11,     4,   214,    14,     5,     3,     7,
     140,   141,   142,   143,    14,    94,    16,    17,    97,     0,
     197,    29,    30,    31,    32,    14,   235,    39,    36,    29,
      38,    14,    40,   137,   138,    43,    95,     6,    14,     5,
     119,   171,   200,   173,   174,    14,    29,     8,     8,   108,
       4,    12,    12,    29,     7,     8,   213,    23,    24,     4,
      29,    30,    31,    32,     9,     4,   243,    36,    14,    38,
      15,    40,    46,    47,    43,    14,     7,    16,    17,   182,
       7,     4,     9,     5,   163,   164,    25,     5,    27,     7,
      29,    14,     4,    16,    17,   154,    33,    34,    35,    22,
       8,     5,    14,     7,    16,    17,    29,    33,    34,    35,
       7,    10,     9,   192,    48,    27,    14,    29,    33,    34,
      35,    23,    24,   182,    18,    19,    13,    20,    21,    13,
      14,   144,   145,     8,    14,   146,   147,    11,     8,    11,
       4,   220,     4,     4,    45,    14,    14,    14,    44,     9,
       8,     4,   231,     9,     4,    37,    15,     8,     5,     8,
      14,    42,     8,     7,     5,   244,   245,     5,    13,    42,
      41,    37,     8,    10,    53,    18,   224,    32,    84,    21,
     222,   238,    34,    48,    87,    90,   220,    27,   231,   160,
     158,    -1,    -1,    93
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    51,    14,     0,    52,     4,    14,    54,    55,
       7,    53,    55,     5,     8,    48,    56,    58,    59,    60,
      14,    29,    61,    62,    10,    13,    64,    65,    66,    60,
      63,    63,     7,     8,    67,    68,    69,    57,    66,    62,
      68,     8,    14,    70,    71,    73,    74,     8,     7,     9,
      11,    46,    47,    75,    76,    71,    33,    34,    35,    72,
       6,    14,    29,    30,    31,    32,    36,    38,    40,    43,
      95,    96,    97,   100,   102,   122,   125,   128,   129,   132,
     134,   140,   141,    77,    79,    76,    14,    29,    98,   103,
     101,     4,     4,     4,   130,   133,    14,     4,    14,    16,
      17,    22,    29,   114,   115,   116,   117,   118,     8,    12,
      45,   143,    14,    81,    81,    99,    99,     4,     9,    15,
     104,   106,   107,   107,    14,   126,   127,   114,   123,   124,
     123,   114,    95,   135,   114,   119,   115,    23,    24,   142,
      25,    26,    27,    28,    18,    19,    20,    21,    96,   144,
      82,     8,     9,   105,   108,   114,   112,     5,     7,     5,
       7,     5,   131,    39,    15,     5,     4,   120,   115,   115,
      44,    27,   116,    25,    27,   116,   116,   116,   117,   117,
     118,   118,    11,    94,    96,     4,    83,    56,    33,    34,
      35,    93,   112,    96,   113,   127,   124,    37,   114,   114,
     121,    94,   116,   116,   116,    95,    13,    84,    85,    86,
      88,    89,    78,     8,   109,   110,   111,   114,    94,   136,
     112,    12,    87,     5,     8,    14,    90,    91,     8,    56,
     113,     7,   137,   138,   139,   109,    88,    86,     7,     9,
      80,     5,   111,    37,    41,    42,   113,    91,    33,    34,
      35,    92,     8,    94,   114,   114,     5
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
#line 36 "compilador.y"
    {
                geraCodigo (NULL, "INPP");
                iniciaTS(&ts, 8);
                iniciaTipo(&pt, 8);
                iniciaRotulo(&pr, 8);
                iniciaAux(&pa, 8);
                inserePROCTS(&ts, token, procedimento, nivel_lexico, numRotulo);
                numRotulo++;
              }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 46 "compilador.y"
    {
                tipoTS(&ts, unkwon, num_vars);
                deslocamentoPARAMTS(&ts);
                num_vars = 0;
              }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 52 "compilador.y"
    {
                geraCodigo (NULL, "PARA");
                protocoloZero(&ts, &pt, &pr, &pa);
              }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 62 "compilador.y"
    { inserePARAMTS(&ts, token, parametro_formal, nivel_lexico, referencia); num_vars++; }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 67 "compilador.y"
    {
                if (num_vars) {
                    sprintf(str, "AMEM %d", num_vars);
                    geraCodigo (NULL, str);
                }
              }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 77 "compilador.y"
    {
                aux = removeTS(&ts, nivel_lexico);
                if (aux) {
                    sprintf(str, "DMEM %d", aux);
                    geraCodigo (NULL, str);
                }
              }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 105 "compilador.y"
    {
                    if(insereROTTS(&ts, token, rotulo, nivel_lexico, numRotulo)) {
                        sprintf(str, "redeclaration of '%s'", token);
                        imprimeErro(str);
                    }
                    numRotulo++;
                  }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 129 "compilador.y"
    { aux = num_vars; }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 138 "compilador.y"
    {
          	if(varlocal) {
          		if(insereVLTS(&ts, token, variavel_simples, nivel_lexico, num_vars)) {
		            sprintf(str, "redeclaration of '%s'", token);
		            imprimeErro(str);
            	}
            } else if(insereVSTS(&ts, token, variavel_simples, nivel_lexico, num_vars)) {
                sprintf(str, "redeclaration of '%s'", token);
                imprimeErro(str);
            }
            num_vars++;
          }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 152 "compilador.y"
    { tipoTS(&ts, inteiro, num_vars-aux); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 153 "compilador.y"
    { tipoTS(&ts, boolean, num_vars-aux); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 154 "compilador.y"
    { tipoTS(&ts, string, num_vars-aux); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 157 "compilador.y"
    {
                        sprintf(str, "DSVS R%d", numRotulo);
                        if(numRotulo < 10)
                        	sprintf(str, "DSVS R0%d", numRotulo);
                        empilhaRotulo(&pr, numRotulo);
                        geraCodigo(NULL, str);
                        numRotulo++;
                        varlocal = 1;
                      }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 167 "compilador.y"
    {
                        varlocal = 0;
                        num_vars = 0;
                        r = desempilhaRotulo(&pr);
                        sprintf(fim, "R%d", r);
                        if(r < 10)
                        	sprintf(fim, "R0%d", r);
                        geraCodigo(fim, "NADA");
                      }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 184 "compilador.y"
    { aux = 0; }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 186 "compilador.y"
    {
                    element = buscaPROCTS(&ts, "");
                    sprintf(str, "RTPR %d, %d", nivel_lexico, element->parametros[0]);
                    geraCodigo(NULL, str);
                    nivel_lexico--;
                  }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 194 "compilador.y"
    { aux = 1; proc++; }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 196 "compilador.y"
    {
                    element = buscaPROCTS(&ts, "");
                    sprintf(str, "RTPR %d, %d", nivel_lexico, element->parametros[0]);
                    geraCodigo(NULL, str);
                    nivel_lexico--;
                    proc--;
                  }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 207 "compilador.y"
    {
                //strcpy(ident, token);
                nivel_lexico++;
                num_vars = 0;
                if (aux)
                    aux = inserePROCTS(&ts, token, funcao, nivel_lexico, numRotulo);
                else
                    aux = inserePROCTS(&ts, token, procedimento, nivel_lexico, numRotulo);
                if (aux) {
                    sprintf(str, "redeclaration of '%s'", token);
                    imprimeErro(str);
                }
                element = buscaPROCTS(&ts, "");
                sprintf(inicio, "R%d", numRotulo);
                if(numRotulo < 10)
                	sprintf(inicio, "R0%d", numRotulo);
                sprintf(str, "ENPR %d", nivel_lexico);
                geraCodigo(inicio, str);
                numRotulo++;
                
                //geraCodigo(NULL, str);
              }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 237 "compilador.y"
    { deslocamentoPARAMTS(&ts); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 246 "compilador.y"
    { ref = 1; }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 248 "compilador.y"
    { ref = 0; }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 252 "compilador.y"
    { aux = element->parametros[0]; }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 261 "compilador.y"
    {
            if (ref)
                element->parametros[PASSAGEM(element->parametros[0])] = referencia;
            else
                element->parametros[PASSAGEM(element->parametros[0])] = valor;
            if(inserePARAMTS(&ts, token, parametro_formal, nivel_lexico, element->parametros[PASSAGEM(element->parametros[0])])) {
                sprintf(str, "redeclaration of '%s'", token);
                imprimeErro(str);
            }
            element->parametros[0]++;
          }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 275 "compilador.y"
    {
                tipoTS(&ts, inteiro, element->parametros[0]-aux);
                for (;aux < element->parametros[0]; aux++)
                    element->parametros[TIPO(aux)] = inteiro;
              }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 281 "compilador.y"
    {
                tipoTS(&ts, boolean, element->parametros[0]-aux);
                for (;aux < element->parametros[0]; aux++)
                    element->parametros[TIPO(aux)] = boolean;
              }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 287 "compilador.y"
    {
                tipoTS(&ts, string, element->parametros[0]-aux);
                for (;aux < element->parametros[0]; aux++)
                    element->parametros[TIPO(aux)] = string;
              }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 295 "compilador.y"
    { element->tipo = inteiro; }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 297 "compilador.y"
    { element->tipo = boolean; }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 299 "compilador.y"
    { element->tipo = string; }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 328 "compilador.y"
    {
            element = buscaTS(&ts, token);
            if (element->categoria != rotulo)
                imprimeErro("Illegal expression");
            sprintf(str, "DSVR R%d, %d, %d", element->rotulo, element->nivel, nivel_lexico);
            if(element->rotulo < 10)
            	sprintf(str, "DSVR R0%d, %d, %d", element->rotulo, element->nivel, nivel_lexico);
            geraCodigo(NULL, str);
          }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 340 "compilador.y"
    { strcpy(ident, token); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 345 "compilador.y"
    {
                    strcpy(ident, token);
                    element = buscaTS(&ts, token);
                    if (element == NULL) {
                        sprintf(str, "'%s' undeclared (first use in this function)", token);
                        imprimeErro(str);
                    }
                    empilhaAux(&pa, element);
                  }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 358 "compilador.y"
    {
                    desempilhaAux(&pa);
                    element = buscaTS(&ts, ident);
                    if (element->busy) {
                        sprintf(str, "Illegal assignment to for-loop variable '%s'", ident);
                        imprimeErro(str);
                    }
                    if (desempilhaTipo(&pt) != element->tipo)
                        imprimeErro("Incompatible types");
                    sprintf(str, "ARMZ %d, %d", element->nivel, element->deslocamento);
                    if (element->categoria == parametro_formal) {
                        if (element->passagem)
                            sprintf(str, "ARMI %d, %d", element->nivel, element->deslocamento);
                    } else if (element->categoria == funcao) {
                        if (proc) {
                            if (!buscaFUNCTS(&ts, ident, nivel_lexico))
                                imprimeErro("Illegal expression");
                        } else
                            imprimeErro("Illegal expression");
                    } else if ((element->categoria == procedimento) || (element->categoria == rotulo))
                        imprimeErro("Illegal expression");
                    geraCodigo(NULL, str);
                  }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 383 "compilador.y"
    {
                    if ((element->categoria != procedimento) && (element->categoria != funcao))
                        imprimeErro("Illegal expression");
                  }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 388 "compilador.y"
    {
                    if ((element->categoria != procedimento) && (element->categoria != funcao))
                        imprimeErro("Illegal expression");
                  }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 396 "compilador.y"
    {
                element = buscaTS(&ts, ident);
                if (element->nivel != nivel_lexico)
                    imprimeErro("Illegal expression");
                sprintf(inicio, "R%d", element->rotulo);
                if(element->rotulo < 10)
            		sprintf(inicio, "R0%d", element->rotulo);
            	sprintf(str, "ENRT %d, %d", element->nivel, qtdeVL(&ts));
                geraCodigo(inicio, str);
                
                //geraCodigo(NULL, str);
              }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 420 "compilador.y"
    {
                 if (desempilhaTipo(&pt) != p->parametros[TIPO(num_vars)])  {
                     sprintf(str, "Incompatible types %d", p->parametros[TIPO(num_vars)]);
                     imprimeErro(str);
                 }
                 num_vars++;
              }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 429 "compilador.y"
    {
                    p = buscaAux(&pa);
                    pa.num[pa.topo] = num_vars;
                    num_vars = 0;
                    ref++;
                    if (p->categoria == funcao)
                        geraCodigo(NULL, "AMEM 1");
                  }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 439 "compilador.y"
    {
                    ref--;
                    if (num_vars != p->parametros[0]) {
                        sprintf(str, "Wrong number of parameters specified for call to '%s'", p->id);
                        imprimeErro(str);
                    }
                    sprintf(str, "CHPR R%d, %d", p->rotulo, nivel_lexico);
                    if(p->rotulo < 10)
                    	sprintf(str, "CHPR R0%d, %d", p->rotulo, nivel_lexico);
                    geraCodigo(NULL, str);
                    num_vars = pa.num[pa.topo];
                    p = desempilhaAux(&pa);
                  }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 455 "compilador.y"
    {
                if (desempilhaTipo(&pt) != boolean)
                    imprimeErro("Incompatible types");
                empilhaTipo(&pt, boolean);
                geraCodigo(NULL, "NEGA");
              }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 462 "compilador.y"
    {
                if ((desempilhaTipo(&pt) != boolean) || (desempilhaTipo(&pt) != boolean))
                    imprimeErro("Incompatible types");
                empilhaTipo(&pt, boolean);
                geraCodigo(NULL, "CONJ");
              }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 469 "compilador.y"
    {
                if ((desempilhaTipo(&pt) != boolean) || (desempilhaTipo(&pt) != boolean))
                    imprimeErro("Incompatible types");
                empilhaTipo(&pt, boolean);
                geraCodigo(NULL, "DISJ");
              }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 479 "compilador.y"
    {
            if ((desempilhaTipo(&pt) != inteiro) || (desempilhaTipo(&pt) != inteiro))
                imprimeErro("Incompatible types");
            empilhaTipo(&pt, boolean);
            geraCodigo(NULL, "CMMA");
          }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 486 "compilador.y"
    {
            if ((desempilhaTipo(&pt) != inteiro) || (desempilhaTipo(&pt) != inteiro))
                imprimeErro("Incompatible types");
            empilhaTipo(&pt, boolean);
            geraCodigo(NULL, "CMME");
          }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 493 "compilador.y"
    {
          if (desempilhaTipo(&pt) !=  desempilhaTipo(&pt))
            imprimeErro("Incompatible types");
          empilhaTipo(&pt, boolean);
          geraCodigo(NULL, "CMIG");
          }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 500 "compilador.y"
    {
            if (desempilhaTipo(&pt) !=  desempilhaTipo(&pt))
              imprimeErro("Incompatible types");
             empilhaTipo(&pt, boolean);
            geraCodigo(NULL, "CMDG");
          }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 507 "compilador.y"
    {
              if (desempilhaTipo(&pt) !=  desempilhaTipo(&pt))
                imprimeErro("Incompatible types");
              empilhaTipo(&pt, boolean);
              geraCodigo(NULL, "CMDG");
          }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 514 "compilador.y"
    {
              if ((desempilhaTipo(&pt) != inteiro) || (desempilhaTipo(&pt) != inteiro))
                imprimeErro("Incompatible types");
              empilhaTipo(&pt, boolean);
              geraCodigo(NULL, "CMAG");
          }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 521 "compilador.y"
    {
              if ((desempilhaTipo(&pt) != inteiro) || (desempilhaTipo(&pt) != inteiro))
                imprimeErro("Incompatible types");
              empilhaTipo(&pt, boolean);
              geraCodigo(NULL, "CMEG");
          }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 531 "compilador.y"
    {
                  if ((desempilhaTipo(&pt) != inteiro) || (desempilhaTipo(&pt) != inteiro))
                    imprimeErro("Incompatible types");
                  empilhaTipo(&pt, inteiro);
                  geraCodigo(NULL, "SOMA");
              }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 538 "compilador.y"
    {
                  if ((desempilhaTipo(&pt) != inteiro) || (desempilhaTipo(&pt) != inteiro))
                    imprimeErro("Incompatible types");
                  empilhaTipo(&pt, inteiro);
                  geraCodigo(NULL, "SUBT");
              }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 548 "compilador.y"
    {
              if ((desempilhaTipo(&pt) != inteiro) || (desempilhaTipo(&pt) != inteiro))
                imprimeErro("Incompatible types");
              empilhaTipo(&pt, inteiro);
              geraCodigo(NULL, "MULT");
          }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 555 "compilador.y"
    {
              if ((desempilhaTipo(&pt) != inteiro) || (desempilhaTipo(&pt) != inteiro))
                imprimeErro("Incompatible types");
              empilhaTipo(&pt, inteiro);
              geraCodigo(NULL, "DIVI");
          }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 565 "compilador.y"
    {
            element = buscaTS(&ts, token);
            if (element == NULL) {
                sprintf(str, "'%s' undeclared (first use in this function)", token);
                imprimeErro(str);
            }
          }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 574 "compilador.y"
    {
              if (ref && p->parametros[PASSAGEM(num_vars)])
                imprimeErro("Variable identifier expected");
              empilhaTipo(&pt, inteiro);
              sprintf(str, "CRCT %s", token);
              geraCodigo(NULL, str);
          }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 582 "compilador.y"
    {
              if (ref && p->parametros[PASSAGEM(num_vars)])
                imprimeErro("Variable identifier expected");
              empilhaTipo(&pt, boolean);
              geraCodigo(NULL, "CRCT 1");
          }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 589 "compilador.y"
    {
              if (ref && p->parametros[PASSAGEM(num_vars)])
                imprimeErro("Variable identifier expected");
              empilhaTipo(&pt, boolean);
              geraCodigo(NULL, "CRCT 0");
          }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 599 "compilador.y"
    {
                    empilhaAux(&pa, element);
                    empilhaTipo(&pt, element->tipo);
                    if ((element->categoria != procedimento) && (element->categoria != funcao))
                        imprimeErro("Illegal expression");
                  }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 606 "compilador.y"
    {
                    if ((element->categoria == procedimento) || (element->categoria == rotulo))
                        imprimeErro("Illegal expression");
                    else {
                        empilhaTipo(&pt, element->tipo);
                        sprintf(str, "CRVL %d, %d", element->nivel, element->deslocamento);
                        if (element->categoria == funcao) {
                            if (element->parametros[0] != 0) {
                                sprintf(str, "Wrong number of parameters specified for call to '%s'", element->id);
                                imprimeErro(str);
                            }
                            geraCodigo(NULL, "AMEM 1");
                            sprintf(str, "CHPR R%d, %d", element->rotulo, nivel_lexico);
                            if(element->rotulo < 10)
                            	sprintf(str, "CHPR R0%d, %d", element->rotulo, nivel_lexico);
                        } else if (element->categoria == variavel_simples) {
                            if (ref)
                                if (p->parametros[PASSAGEM(num_vars)])
                                    sprintf(str, "CREN %d, %d", element->nivel, element->deslocamento);
                        } else if (element->categoria == parametro_formal) {
                            if (ref) {
                                if (p->parametros[PASSAGEM(num_vars)]) {
                                    if (!element->passagem)
                                        sprintf(str, "CREN %d, %d", element->nivel, element->deslocamento);
                                } else
                                    if (element->passagem)
                                        sprintf(str, "CRVI %d, %d", element->nivel, element->deslocamento);
                            } else
                                if (element->passagem)
                                    sprintf(str, "CRVI %d, %d", element->nivel, element->deslocamento);
                        }
                    }
                    geraCodigo(NULL, str);
                  }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 651 "compilador.y"
    {
              desempilhaTipo(&pt);
              geraCodigo(NULL, "IMPR");
          }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 665 "compilador.y"
    {
              geraCodigo(NULL, "LEIT");
              element = buscaTS(&ts, token);
              if (element == NULL) {
               sprintf(str, "'%s' undeclared (first use in this function)", token);
               imprimeErro(str);
              }
              if (element->tipo == boolean)
                imprimeErro("Can't read variables of this type");
              sprintf(str, "ARMZ %d, %d", element->nivel, element->deslocamento);
              geraCodigo(NULL, str);
          }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 685 "compilador.y"
    {
              sprintf(inicio, "R%d", numRotulo);
              if(numRotulo < 10)
              	sprintf(inicio, "R0%d", numRotulo);
              empilhaRotulo(&pr, numRotulo);
              numRotulo++;
              geraCodigo(inicio, "NADA");
          }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 694 "compilador.y"
    {
              if (desempilhaTipo(&pt) != boolean)
                imprimeErro("Incompatible types");
              sprintf(fim, "R%d", numRotulo);
              if(numRotulo < 10)
              	sprintf(fim, "R0%d", numRotulo);
              empilhaRotulo(&pr, numRotulo);
              numRotulo++;
              sprintf(str, "DSVF %s", fim);
              geraCodigo(NULL, str);
          }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 706 "compilador.y"
    {
          	  r = desempilhaRotulo(&pr);
              sprintf(fim, "R%d", r);
              if(r < 10)
              	sprintf(fim, "R0%d", r);
              r = desempilhaRotulo(&pr);
              sprintf(str, "DSVS R%d", r);
              if(r < 10)
              	sprintf(str, "DSVS R0%d", r);
              geraCodigo(NULL, str);
              geraCodigo(fim, "NADA");
          }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 721 "compilador.y"
    {
               sprintf(inicio, "R%d", numRotulo);
               if(numRotulo < 10)
               	sprintf(inicio, "R0%d", numRotulo);
               empilhaRotulo(&pr, numRotulo);
               numRotulo++;
               geraCodigo(inicio, "NADA");
           }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 730 "compilador.y"
    {
               if (desempilhaTipo(&pt) != boolean)
                imprimeErro("Incompatible types");
               r = desempilhaRotulo(&pr);
               sprintf(str, "DSVF R%d", r);
               if(r < 10)
               	sprintf(str, "DSVF R0%d", r);
               geraCodigo(NULL, str);
           }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 742 "compilador.y"
    { strcpy(ident, token); busy++; }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 744 "compilador.y"
    {
            element = buscaTS(&ts, ident);
            if (element == NULL) {
             sprintf(str, "'%s' undeclared (first use in this function)", ident);
             imprimeErro(str);
            }
            if (element->tipo != inteiro)
              imprimeErro("Incompatible types");
            if (desempilhaTipo(&pt) != inteiro)
              imprimeErro("Incompatible types");
            element->busy = busy;
        }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 757 "compilador.y"
    {
        	r = desempilhaRotulo(&pr);
            sprintf(fim, "R%d", r);
            if(r < 10)
            	sprintf(fim, "R0%d", r);
            sprintf(str, "DSVS R%d", r);
            if(r < 10)
            	sprintf(str, "DSVS R0%d", r);
            geraCodigo(NULL, str);
            geraCodigo(fim, "NADA");
            liberaFOR(&ts, busy);
            busy--;
        }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 772 "compilador.y"
    {
                      geraCodigo(NULL, "CRCT 1");
                      geraCodigo(NULL, "SUBT");
                      sprintf(str, "ARMZ %d, %d", element->nivel, element->deslocamento);
                      geraCodigo(NULL, str);
                      sprintf(inicio, "R%d", numRotulo);
                      if(numRotulo < 10)
                      	sprintf(inicio, "R0%d", numRotulo);
                      empilhaRotulo(&pr, numRotulo);
                      numRotulo++;
                      geraCodigo(inicio, "NADA");
                      sprintf(str, "CRVL %d, %d", element->nivel, element->deslocamento);
                      geraCodigo(NULL, str);
                      geraCodigo(NULL, "CRCT 1");
                      geraCodigo(NULL, "SOMA");
                      sprintf(str, "ARMZ %d, %d", element->nivel, element->deslocamento);
                      geraCodigo(NULL, str);
                      sprintf(str, "CRVL %d, %d", element->nivel, element->deslocamento);
                      geraCodigo(NULL, str);
                  }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 793 "compilador.y"
    {
                      if (desempilhaTipo(&pt) != inteiro)
                        imprimeErro("Incompatible types");
                      geraCodigo(NULL, "CMEG");
                      sprintf(fim, "R%d", numRotulo);
                      if(numRotulo < 10)
                      	sprintf(fim, "R0%d", numRotulo);
                      empilhaRotulo(&pr, numRotulo);
                      numRotulo++;
                      sprintf(str, "DSVF %s", fim);
                      geraCodigo(NULL, str);
                  }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 805 "compilador.y"
    {
                      geraCodigo(NULL, "CRCT 1");
                      geraCodigo(NULL, "SOMA");
                      sprintf(str, "ARMZ %d, %d", element->nivel, element->deslocamento);
                      geraCodigo(NULL, str);
                      sprintf(inicio, "R%d", numRotulo);
                      if(numRotulo < 10)
                      	sprintf(inicio, "R0%d", numRotulo);
                      empilhaRotulo(&pr, numRotulo);
                      numRotulo++;
                      geraCodigo(inicio, "NADA");
                      sprintf(str, "CRVL %d, %d", element->nivel, element->deslocamento);
                      geraCodigo(NULL, str);
                      geraCodigo(NULL, "CRCT 1");
                      geraCodigo(NULL, "SUBT");
                      sprintf(str, "ARMZ %d, %d", element->nivel, element->deslocamento);
                      geraCodigo(NULL, str);
                      sprintf(str, "CRVL %d, %d", element->nivel, element->deslocamento);
                      geraCodigo(NULL, str);
                  }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 826 "compilador.y"
    {
                      if (desempilhaTipo(&pt) != inteiro)
                        imprimeErro("Incompatible types");
                      geraCodigo(NULL, "CMAG");
                      sprintf(fim, "R%d", numRotulo);
                      if(numRotulo < 10)
                      	sprintf(fim, "R0%d", numRotulo);
                      empilhaRotulo(&pr, numRotulo);
                      numRotulo++;
                      sprintf(str, "DSVF %s", fim);
                      geraCodigo(NULL, str);
                  }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 843 "compilador.y"
    {
      	  r = desempilhaRotulo(&pr);
          sprintf(fim, "R%d", r);
          if(r < 10)
          	sprintf(fim, "R0%d", r);
          geraCodigo(fim, "NADA");
      }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 853 "compilador.y"
    {
              if (desempilhaTipo(&pt) != boolean)
                imprimeErro("Incompatible types");
              sprintf(inicio, "R%d", numRotulo);
              if(numRotulo < 10)
              	sprintf(inicio, "R0%d", numRotulo);
              empilhaRotulo(&pr, numRotulo);
              numRotulo++;
              sprintf(str, "DSVF %s", inicio);
              geraCodigo(NULL, str);
          }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 868 "compilador.y"
    {
                  sprintf(fim, "R%d", numRotulo);
                  if(numRotulo < 10)
                  	sprintf(fim, "R0%d", numRotulo);
                  sprintf(str, "DSVS %s", fim);
                  geraCodigo(NULL, str);
                  r = desempilhaRotulo(&pr);
                  sprintf(inicio, "R%d", r);
                  if(r < 10)
                  	sprintf(inicio, "R0%d", r);
                  geraCodigo(inicio, "NADA");
                  empilhaRotulo(&pr, numRotulo);
                  numRotulo++;
              }
    break;



/* Line 1806 of yacc.c  */
#line 2689 "compilador.tab.c"
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
#line 887 "compilador.y"


main (int argc, char** argv) {
   FILE* fp;
   extern FILE* yyin;
   tabela ts;

   if (argc<2 || argc>2) {
         printf("usage compilador <arq>a %d\n", argc);
         return(-1);
      }

   fp=fopen (argv[1], "r");
   if (fp == NULL) {
      printf("usage compilador <arq>b\n");
      return(-1);
   }
    
   yyin=fp;
   yyparse();

   return 0;
}


