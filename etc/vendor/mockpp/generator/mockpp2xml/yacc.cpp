/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_UNIMPORTANT = 258,
     T_CHARACTER_LITERAL = 259,
     T_DOUBLE_LITERAL = 260,
     T_IDENTIFIER = 261,
     T_INTEGER_LITERAL = 262,
     T_STRING_LITERAL = 263,
     T_INCLUDE = 264,
     T_CLASS = 265,
     T_STRUCT = 266,
     T_LEFT_CURLY_BRACKET = 267,
     T_LEFT_PARANTHESIS = 268,
     T_RIGHT_CURLY_BRACKET = 269,
     T_RIGHT_PARANTHESIS = 270,
     T_COLON = 271,
     T_SEMICOLON = 272,
     T_PUBLIC = 273,
     T_PROTECTED = 274,
     T_TRIPE_DOT = 275,
     T_PRIVATE = 276,
     T_VIRTUAL = 277,
     T_CONST = 278,
     T_THROW = 279,
     T_INLINE = 280,
     T_FRIEND = 281,
     T_RETURN = 282,
     T_SIGNAL = 283,
     T_SLOT = 284,
     T_TYPEDEF = 285,
     T_PLUS = 286,
     T_MINUS = 287,
     T_COMMA = 288,
     T_ASTERISK = 289,
     T_TILDE = 290,
     T_LESS = 291,
     T_GREATER = 292,
     T_AMPERSAND = 293,
     T_EXTERN = 294,
     T_EXTERN_C = 295,
     T_ACCESS = 296,
     T_ENUM = 297,
     T_NAMESPACE = 298,
     T_USING = 299,
     T_UNKNOWN = 300,
     T_TRIPLE_DOT = 301,
     T_TRUE = 302,
     T_FALSE = 303,
     T_STATIC = 304,
     T_MUTABLE = 305,
     T_EQUAL = 306,
     T_SCOPE = 307,
     T_NULL = 308,
     T_INT = 309,
     T_ARRAY_OPEN = 310,
     T_ARRAY_CLOSE = 311,
     T_CHAR = 312,
     T_ULXRMETHODS_AREA = 313,
     T_ULXRFUNCTION = 314,
     T_ULXRCTOR_AREA = 315,
     T_MOCKMETHODS_AREA = 316,
     T_MOCKCTOR_AREA = 317,
     T_SIGNED = 318,
     T_UNSIGNED = 319,
     T_LONG = 320,
     T_SHORT = 321,
     T_FUNOPERATOR = 322,
     T_MISCOPERATOR = 323,
     T_SHIFT = 324,
     T_DCOP = 325,
     T_DCOP_AREA = 326,
     T_DCOP_SIGNAL_AREA = 327,
     T_QOBJECT = 328
   };
#endif
/* Tokens.  */
#define T_UNIMPORTANT 258
#define T_CHARACTER_LITERAL 259
#define T_DOUBLE_LITERAL 260
#define T_IDENTIFIER 261
#define T_INTEGER_LITERAL 262
#define T_STRING_LITERAL 263
#define T_INCLUDE 264
#define T_CLASS 265
#define T_STRUCT 266
#define T_LEFT_CURLY_BRACKET 267
#define T_LEFT_PARANTHESIS 268
#define T_RIGHT_CURLY_BRACKET 269
#define T_RIGHT_PARANTHESIS 270
#define T_COLON 271
#define T_SEMICOLON 272
#define T_PUBLIC 273
#define T_PROTECTED 274
#define T_TRIPE_DOT 275
#define T_PRIVATE 276
#define T_VIRTUAL 277
#define T_CONST 278
#define T_THROW 279
#define T_INLINE 280
#define T_FRIEND 281
#define T_RETURN 282
#define T_SIGNAL 283
#define T_SLOT 284
#define T_TYPEDEF 285
#define T_PLUS 286
#define T_MINUS 287
#define T_COMMA 288
#define T_ASTERISK 289
#define T_TILDE 290
#define T_LESS 291
#define T_GREATER 292
#define T_AMPERSAND 293
#define T_EXTERN 294
#define T_EXTERN_C 295
#define T_ACCESS 296
#define T_ENUM 297
#define T_NAMESPACE 298
#define T_USING 299
#define T_UNKNOWN 300
#define T_TRIPLE_DOT 301
#define T_TRUE 302
#define T_FALSE 303
#define T_STATIC 304
#define T_MUTABLE 305
#define T_EQUAL 306
#define T_SCOPE 307
#define T_NULL 308
#define T_INT 309
#define T_ARRAY_OPEN 310
#define T_ARRAY_CLOSE 311
#define T_CHAR 312
#define T_ULXRMETHODS_AREA 313
#define T_ULXRFUNCTION 314
#define T_ULXRCTOR_AREA 315
#define T_MOCKMETHODS_AREA 316
#define T_MOCKCTOR_AREA 317
#define T_SIGNED 318
#define T_UNSIGNED 319
#define T_LONG 320
#define T_SHORT 321
#define T_FUNOPERATOR 322
#define T_MISCOPERATOR 323
#define T_SHIFT 324
#define T_DCOP 325
#define T_DCOP_AREA 326
#define T_DCOP_SIGNAL_AREA 327
#define T_QOBJECT 328




/* Copy the first part of user declarations.  */
#line 2 "yacc.yy"

/*****************************************************************
Copyright (c) 1999 Torben Weis <weis@kde.org>
Copyright (c) 2000 Matthias Ettrich <ettrich@kde.org>
Copyright (c) 2007 Ewald Arnold <mockpp@ewald-arnold.de>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************/

//#include <config.h>

// Workaround for a bison issue:
// bison.simple concludes from _GNU_SOURCE that stpcpy is available,
// while GNU string.h only exposes it if __USE_GNU is set.
#ifdef _GNU_SOURCE
#define __USE_GNU 1
#endif

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "mstring.h"

#define AMP_ENTITY "&amp;"
#define YYERROR_VERBOSE

extern int yylex();

// extern MString idl_lexFile;
extern int idl_line_no;
extern int function_mode;

static int mocking_area = 0;
static int mockpp_methods_area = 0;
static int mockpp_constructor_area = 0;

static MString in_namespace( "" );

void dcopidlInitFlex( const char *_code );

void yyerror( const char *s )
{
  fprintf(stderr, "In line %i : %s\n", idl_line_no, s );
        exit(1);
  //   theParser->parse_error( idl_lexFile, s, idl_line_no );
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
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

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 70 "yacc.yy"
{
  long                 _int;
  MString             *_str;
  unsigned short       _char;
  double               _float;
}
/* Line 187 of yacc.c.  */
#line 315 "yacc.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 328 "yacc.cpp"

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
# if YYENABLE_NLS
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
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   645

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  74
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  58
/* YYNRULES -- Number of rules.  */
#define YYNRULES  199
/* YYNRULES -- Number of states.  */
#define YYNSTATES  406

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   328

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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     7,     8,    11,    16,    17,    20,    23,
      25,    27,    28,    35,    43,    47,    51,    57,    58,    66,
      71,    77,    80,    85,    93,   102,   105,   107,   109,   111,
     114,   115,   117,   119,   121,   123,   125,   127,   129,   133,
     136,   139,   142,   146,   149,   153,   155,   159,   161,   166,
     170,   172,   175,   179,   182,   184,   185,   187,   189,   192,
     196,   199,   202,   205,   208,   211,   214,   217,   220,   223,
     229,   234,   239,   244,   251,   256,   263,   270,   278,   285,
     292,   298,   302,   304,   308,   310,   312,   314,   317,   319,
     321,   323,   327,   331,   339,   349,   350,   355,   356,   358,
     360,   363,   365,   368,   371,   375,   378,   382,   385,   389,
     392,   396,   398,   400,   403,   405,   408,   410,   413,   416,
     419,   421,   423,   427,   428,   430,   434,   436,   438,   441,
     444,   449,   456,   460,   462,   465,   467,   471,   475,   478,
     481,   485,   488,   490,   494,   497,   501,   503,   507,   510,
     512,   513,   516,   522,   524,   526,   528,   530,   535,   536,
     538,   540,   542,   544,   546,   548,   556,   565,   576,   585,
     587,   591,   592,   597,   599,   603,   606,   612,   620,   627,
     631,   633,   635,   639,   644,   647,   649,   650,   652,   655,
     656,   658,   662,   665,   668,   672,   678,   684,   690,   697
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      75,     0,    -1,    76,    80,    75,    -1,    -1,    76,     9,
      -1,    40,    12,    75,    14,    -1,    -1,    71,    16,    -1,
      72,    16,    -1,    70,    -1,    73,    -1,    -1,    10,    90,
      94,    79,    96,    17,    -1,    10,     6,    90,    94,    79,
      96,    17,    -1,    10,    90,    17,    -1,    11,    90,    17,
      -1,    11,    90,    94,    96,    17,    -1,    -1,    43,     6,
      12,    81,    75,    14,    95,    -1,    44,    43,     6,    17,
      -1,    44,     6,    52,     6,    17,    -1,    39,    17,    -1,
      30,   112,    90,    17,    -1,    30,    11,    12,    82,    14,
      90,    17,    -1,    30,    11,    90,    12,    82,    14,    90,
      17,    -1,    25,   123,    -1,   123,    -1,   131,    -1,    97,
      -1,   131,    82,    -1,    -1,    47,    -1,    48,    -1,    21,
      -1,    19,    -1,    18,    -1,    28,    -1,    29,    -1,    84,
      85,    16,    -1,    85,    16,    -1,    84,    16,    -1,    61,
      16,    -1,    84,    61,    16,    -1,    62,    16,    -1,    84,
      62,    16,    -1,     6,    -1,     6,    52,    90,    -1,    90,
      -1,    90,    36,   113,    37,    -1,   117,    18,    91,    -1,
      91,    -1,    92,    12,    -1,    92,    33,    93,    -1,    16,
      93,    -1,    12,    -1,    -1,    17,    -1,    14,    -1,   102,
      96,    -1,    25,   123,    96,    -1,   123,    96,    -1,    88,
      96,    -1,    89,    96,    -1,    97,    96,    -1,    87,    96,
      -1,    77,    96,    -1,    86,    96,    -1,    78,    96,    -1,
     131,    96,    -1,    26,    10,    90,    17,    96,    -1,    26,
      90,    17,    96,    -1,    26,   119,    17,    96,    -1,    10,
      90,    17,    96,    -1,    10,    90,    94,    96,    17,    96,
      -1,    11,    90,    17,    96,    -1,    11,    90,    94,    96,
      17,    96,    -1,    44,     6,    52,     6,    17,    96,    -1,
      42,     6,    12,    98,    14,     6,    17,    -1,    42,     6,
      12,    98,    14,    17,    -1,    42,    12,    98,    14,     6,
      17,    -1,    42,    12,    98,    14,    17,    -1,    99,    33,
      98,    -1,    99,    -1,     6,    51,   101,    -1,     6,    -1,
       4,    -1,     7,    -1,    32,     7,    -1,    53,    -1,    90,
      -1,   100,    -1,   100,    31,   100,    -1,   100,    69,   100,
      -1,    30,    90,    36,   113,    37,    90,    17,    -1,    30,
      90,    36,   113,    37,    52,     6,    90,    17,    -1,    -1,
      24,    13,   108,    15,    -1,    -1,    23,    -1,    63,    -1,
      63,    54,    -1,    64,    -1,    64,    54,    -1,    63,    66,
      -1,    63,    66,    54,    -1,    63,    65,    -1,    63,    65,
      54,    -1,    64,    66,    -1,    64,    66,    54,    -1,    64,
      65,    -1,    64,    65,    54,    -1,    54,    -1,    65,    -1,
      65,    54,    -1,    66,    -1,    66,    54,    -1,    57,    -1,
      63,    57,    -1,    64,    57,    -1,    34,   106,    -1,    34,
      -1,   116,    -1,   107,    33,   116,    -1,    -1,   114,    -1,
     108,    33,   114,    -1,   105,    -1,    90,    -1,    11,    90,
      -1,    10,    90,    -1,    90,    36,   110,    37,    -1,    90,
      36,   110,    37,    52,    90,    -1,   111,    33,   110,    -1,
     111,    -1,   109,   106,    -1,   109,    -1,    23,   109,   106,
      -1,    23,   109,    38,    -1,    23,   109,    -1,   109,    23,
      -1,   109,    23,    38,    -1,   109,    38,    -1,   109,    -1,
     109,   106,    23,    -1,   109,   106,    -1,   112,    33,   113,
      -1,   112,    -1,   112,    90,   115,    -1,   112,   115,    -1,
      46,    -1,    -1,    51,   116,    -1,    51,    13,   112,    15,
     116,    -1,     8,    -1,   101,    -1,     5,    -1,    83,    -1,
      90,    13,   107,    15,    -1,    -1,    22,    -1,    68,    -1,
      69,    -1,    37,    -1,    36,    -1,    51,    -1,   112,    90,
      13,   108,    15,   104,   103,    -1,    22,   112,    90,    13,
     108,    15,   104,   103,    -1,    22,   112,    90,    13,   108,
      15,   104,   103,    51,    53,    -1,   112,    67,   118,    13,
     108,    15,   104,   103,    -1,   116,    -1,   116,    33,   120,
      -1,    -1,     6,    13,   120,    15,    -1,   121,    -1,   121,
      33,   122,    -1,   119,   125,    -1,    90,    13,   108,    15,
     125,    -1,    90,    13,   108,    15,    16,   122,   125,    -1,
     117,    35,    90,    13,    15,   125,    -1,    49,   119,   125,
      -1,    12,    -1,    17,    -1,   124,   126,    14,    -1,   124,
     126,    14,    17,    -1,   127,   126,    -1,    34,    -1,    -1,
      17,    -1,    33,   130,    -1,    -1,     6,    -1,     6,    51,
     116,    -1,   106,     6,    -1,   129,   128,    -1,   112,   130,
      17,    -1,   112,    90,    16,     7,    17,    -1,    49,   112,
       6,   115,    17,    -1,    50,   112,     6,   115,    17,    -1,
     112,     6,    55,   101,    56,    17,    -1,    49,   112,     6,
      55,   101,    56,    17,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   177,   177,   180,   184,   188,   192,   197,   204,   211,
     212,   213,   217,   222,   228,   231,   234,   238,   237,   249,
     252,   255,   258,   261,   264,   267,   270,   273,   276,   282,
     283,   286,   286,   288,   288,   288,   294,   294,   297,   301,
     307,   314,   319,   327,   332,   340,   344,   353,   359,   368,
     372,   379,   383,   390,   394,   402,   404,   408,   412,   416,
     420,   424,   428,   432,   436,   440,   444,   448,   452,   456,
     460,   464,   468,   472,   476,   480,   484,   491,   492,   493,
     494,   498,   499,   503,   504,   508,   509,   510,   511,   512,
     516,   517,   518,   522,   534,   543,   546,   554,   557,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   587,
     591,   598,   601,   609,   612,   613,   621,   622,   623,   624,
     625,   632,   644,   648,   656,   661,   670,   680,   689,   695,
     701,   710,   719,   725,   735,   748,   752,   759,   771,   782,
     792,   794,   797,   804,   807,   810,   813,   816,   822,   823,
     827,   827,   827,   827,   827,   831,   859,   887,   916,   927,
     928,   929,   934,   939,   940,   944,   958,   986,  1013,  1020,
    1032,  1039,  1040,  1041,  1045,  1046,  1047,  1051,  1055,  1056,
    1059,  1060,  1061,  1064,  1068,  1069,  1070,  1071,  1072,  1073
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_UNIMPORTANT", "T_CHARACTER_LITERAL",
  "T_DOUBLE_LITERAL", "T_IDENTIFIER", "T_INTEGER_LITERAL",
  "T_STRING_LITERAL", "T_INCLUDE", "T_CLASS", "T_STRUCT",
  "T_LEFT_CURLY_BRACKET", "T_LEFT_PARANTHESIS", "T_RIGHT_CURLY_BRACKET",
  "T_RIGHT_PARANTHESIS", "T_COLON", "T_SEMICOLON", "T_PUBLIC",
  "T_PROTECTED", "T_TRIPE_DOT", "T_PRIVATE", "T_VIRTUAL", "T_CONST",
  "T_THROW", "T_INLINE", "T_FRIEND", "T_RETURN", "T_SIGNAL", "T_SLOT",
  "T_TYPEDEF", "T_PLUS", "T_MINUS", "T_COMMA", "T_ASTERISK", "T_TILDE",
  "T_LESS", "T_GREATER", "T_AMPERSAND", "T_EXTERN", "T_EXTERN_C",
  "T_ACCESS", "T_ENUM", "T_NAMESPACE", "T_USING", "T_UNKNOWN",
  "T_TRIPLE_DOT", "T_TRUE", "T_FALSE", "T_STATIC", "T_MUTABLE", "T_EQUAL",
  "T_SCOPE", "T_NULL", "T_INT", "T_ARRAY_OPEN", "T_ARRAY_CLOSE", "T_CHAR",
  "T_ULXRMETHODS_AREA", "T_ULXRFUNCTION", "T_ULXRCTOR_AREA",
  "T_MOCKMETHODS_AREA", "T_MOCKCTOR_AREA", "T_SIGNED", "T_UNSIGNED",
  "T_LONG", "T_SHORT", "T_FUNOPERATOR", "T_MISCOPERATOR", "T_SHIFT",
  "T_DCOP", "T_DCOP_AREA", "T_DCOP_SIGNAL_AREA", "T_QOBJECT", "$accept",
  "main", "includes", "dcop_area_begin", "dcop_signal_area_begin",
  "platformtag", "declaration", "@1", "member_list", "bool_value",
  "access_kw", "sigslot", "signal_area_begin", "access_kw_begin",
  "mockpp_methods_area_begin", "mockpp_constructor_area_begin",
  "Identifier", "super_class_name", "super_class", "super_classes",
  "class_header", "opt_semicolon", "body", "enum", "enum_list",
  "enum_item", "number", "int_expression", "typedef", "throw_qualifier",
  "const_qualifier", "int_type", "asterisks", "vparams", "params",
  "type_name", "templ_type_list", "templ_type", "type", "type_list",
  "param", "default", "value", "virtual_qualifier", "operator",
  "function_header", "values", "init_item", "init_list", "function",
  "function_begin", "function_body", "function_lines", "function_line",
  "Identifier_list_rest", "Identifier_list_entry", "Identifier_list",
  "member", 0
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
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    74,    75,    75,    76,    76,    76,    77,    78,    79,
      79,    79,    80,    80,    80,    80,    80,    81,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    82,
      82,    83,    83,    84,    84,    84,    85,    85,    86,    86,
      87,    88,    88,    89,    89,    90,    90,    91,    91,    92,
      92,    93,    93,    94,    94,    95,    95,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    97,    97,    97,
      97,    98,    98,    99,    99,   100,   100,   100,   100,   100,
     101,   101,   101,   102,   102,   103,   103,   104,   104,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   106,
     106,   107,   107,   108,   108,   108,   109,   109,   109,   109,
     109,   109,   110,   110,   111,   111,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   113,   113,   114,   114,   114,
     115,   115,   115,   116,   116,   116,   116,   116,   117,   117,
     118,   118,   118,   118,   118,   119,   119,   119,   119,   120,
     120,   120,   121,   122,   122,   123,   123,   123,   123,   123,
     124,   125,   125,   125,   126,   126,   126,   127,   128,   128,
     129,   129,   129,   130,   131,   131,   131,   131,   131,   131
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     0,     2,     4,     0,     2,     2,     1,
       1,     0,     6,     7,     3,     3,     5,     0,     7,     4,
       5,     2,     4,     7,     8,     2,     1,     1,     1,     2,
       0,     1,     1,     1,     1,     1,     1,     1,     3,     2,
       2,     2,     3,     2,     3,     1,     3,     1,     4,     3,
       1,     2,     3,     2,     1,     0,     1,     1,     2,     3,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     5,
       4,     4,     4,     6,     4,     6,     6,     7,     6,     6,
       5,     3,     1,     3,     1,     1,     1,     2,     1,     1,
       1,     3,     3,     7,     9,     0,     4,     0,     1,     1,
       2,     1,     2,     2,     3,     2,     3,     2,     3,     2,
       3,     1,     1,     2,     1,     2,     1,     2,     2,     2,
       1,     1,     3,     0,     1,     3,     1,     1,     2,     2,
       4,     6,     3,     1,     2,     1,     3,     3,     2,     2,
       3,     2,     1,     3,     2,     3,     1,     3,     2,     1,
       0,     2,     5,     1,     1,     1,     1,     4,     0,     1,
       1,     1,     1,     1,     1,     7,     8,    10,     8,     1,
       3,     0,     4,     1,     3,     2,     5,     7,     6,     3,
       1,     1,     3,     4,     2,     1,     0,     1,     2,     0,
       1,     3,     2,     2,     3,     5,     5,     5,     6,     7
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       6,     0,     0,   158,     6,     1,    45,     4,     0,     0,
     159,     0,   158,     0,     0,     0,     0,     0,     0,     0,
     111,   116,    99,   101,   112,   114,     6,   127,    28,   126,
     142,     0,     0,     0,    26,    27,     0,     0,    45,   129,
     128,     0,     0,   127,     0,   138,     0,     0,    25,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,   117,   105,   103,   102,   118,   109,   107,   113,
     115,     2,   123,     0,   139,   120,   141,   144,    45,     0,
       0,     0,   189,     0,     0,   180,   181,   186,   175,     5,
      46,     0,    54,   158,    14,    11,    15,   158,   129,   128,
       0,   137,   136,     0,    30,   128,     0,     0,    84,     0,
      82,    17,     0,     0,    45,   179,   150,   106,   104,   110,
     108,   149,     0,   150,   124,   135,     0,   133,   140,   119,
     143,     0,     0,   163,   162,   164,   160,   161,     0,   123,
       0,   192,     0,   193,   194,     0,   187,   185,     0,   186,
      11,   159,    47,    50,     0,    53,     0,     9,    10,   158,
       0,     0,    57,    35,    34,    33,   158,     0,    36,    37,
       0,     0,     0,     0,     0,     0,   158,   158,     0,     0,
     158,   158,   158,   158,     0,   158,   158,   158,   158,   123,
       0,     0,     0,    30,    30,    22,     0,     0,     0,     0,
       6,     0,    19,     0,     0,     0,     0,     0,     0,   150,
     148,   134,   130,     0,    85,   155,    86,   153,     0,    31,
      32,    88,   156,    89,    90,   154,   191,    89,     0,   123,
       0,     0,   190,   188,     0,   182,   184,   158,     0,    51,
     158,     0,     0,   129,   128,   158,     0,   127,     0,     0,
       0,    41,    43,     7,     8,    65,    67,    40,     0,     0,
       0,    39,    66,    64,    61,    62,    16,    63,    58,    60,
      68,     0,     0,     0,     0,    29,     0,     0,    83,     0,
      80,    81,     0,    20,     0,   151,     0,   196,   197,     0,
     176,   125,   147,     0,   132,    87,     0,     0,     0,     0,
       0,    97,   195,     0,   183,     0,   146,     0,    52,    49,
      12,   158,   158,   158,   158,    59,   129,   158,   158,     0,
       0,    42,    44,    38,    97,   150,     0,     0,     0,    78,
      79,    55,     0,     0,     0,   173,     0,   131,     0,   121,
      91,    92,   198,    97,    98,    95,   178,    13,     0,    48,
      72,     0,    74,     0,   158,    70,    71,     0,     0,    95,
      23,     0,    77,    56,    18,     0,   199,   171,     0,   177,
     157,     0,    95,     0,   165,   145,   158,   158,    69,     0,
     158,   166,    24,   152,   169,     0,   174,   122,   168,   123,
      73,    75,     0,     0,    76,     0,   171,   172,     0,     0,
      93,   167,   170,    96,     0,    94
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,   176,   177,   159,    26,   200,   191,   222,
     178,   179,   180,   181,   182,   183,    27,   153,   154,   155,
      95,   364,   184,   185,   109,   110,   224,   225,   186,   374,
     345,    29,    81,   338,   122,    30,   126,   127,    31,   307,
     124,   205,   384,    32,   138,    33,   385,   335,   336,   187,
      87,    88,   148,   149,   143,    82,    83,   188
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -293
static const yytype_int16 yypact[] =
{
      18,    74,    53,   516,    31,  -293,    82,  -293,   149,   157,
     561,    56,    81,   579,   153,   165,   193,    64,   304,   561,
    -293,  -293,   177,   191,   152,   185,   129,   113,  -293,  -293,
     206,    14,   220,   241,  -293,  -293,   254,   157,    16,   263,
     300,   157,   157,   234,   157,   235,   304,    26,  -293,   210,
     157,  -293,   259,   280,   272,   252,   281,   561,    34,   241,
     287,  -293,  -293,   253,   270,  -293,  -293,   276,   277,  -293,
    -293,  -293,   333,    56,   268,   279,  -293,   309,    85,   151,
     202,   332,   314,   325,   157,  -293,  -293,    91,  -293,  -293,
    -293,   293,  -293,    22,  -293,   179,  -293,   452,  -293,  -293,
     335,  -293,  -293,   336,   271,   338,   334,   280,   302,   341,
     331,  -293,   356,   348,   139,  -293,   315,  -293,  -293,  -293,
    -293,  -293,    36,    49,  -293,   279,   337,   340,  -293,  -293,
    -293,   219,   144,  -293,  -293,  -293,  -293,  -293,   362,   333,
     371,  -293,    29,  -293,  -293,   367,  -293,  -293,   368,    91,
     179,  -293,   347,  -293,    45,  -293,   366,  -293,  -293,   452,
     157,   157,  -293,  -293,  -293,  -293,    81,   534,  -293,  -293,
     157,   379,   370,   376,   377,   378,   452,   452,    44,   384,
     452,   452,   452,   452,   385,   452,   452,   452,   452,   333,
     561,   381,    41,   271,   271,  -293,   387,   144,   122,   280,
      31,   386,  -293,   160,   144,   388,   389,   306,   333,   315,
    -293,  -293,   352,    56,  -293,  -293,  -293,  -293,   400,  -293,
    -293,  -293,  -293,   395,    15,  -293,  -293,  -293,   353,   333,
      62,   393,   361,  -293,   396,   397,  -293,   452,   561,  -293,
      22,   157,   398,   329,   360,   452,   157,   124,   399,   383,
     365,  -293,  -293,  -293,  -293,  -293,  -293,  -293,   404,   405,
     406,  -293,  -293,  -293,  -293,  -293,  -293,  -293,  -293,  -293,
    -293,    84,   407,   157,   408,  -293,   409,   229,  -293,   413,
    -293,  -293,   411,  -293,   561,  -293,   375,  -293,  -293,   428,
    -293,  -293,  -293,   157,  -293,  -293,   219,   144,   144,   418,
     142,   414,  -293,   241,  -293,   419,   410,   401,  -293,  -293,
    -293,   452,   452,   452,   452,  -293,   422,   452,   452,   561,
     434,  -293,  -293,  -293,   414,   278,   424,   157,   425,  -293,
    -293,   427,   430,   429,   435,   416,   241,  -293,   168,  -293,
    -293,  -293,  -293,   414,  -293,   423,  -293,  -293,   561,  -293,
    -293,   433,  -293,   436,   452,  -293,  -293,   415,   437,   423,
    -293,   438,  -293,  -293,  -293,   219,  -293,   219,   428,  -293,
    -293,   219,   423,   443,  -293,  -293,   452,   452,  -293,    46,
     452,   417,  -293,  -293,   431,   442,  -293,  -293,  -293,   333,
    -293,  -293,   445,   444,  -293,   412,   219,  -293,   195,   157,
    -293,  -293,  -293,  -293,   450,  -293
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -293,    17,  -293,  -293,  -293,   310,  -293,  -293,   -61,  -293,
    -293,   294,  -293,  -293,  -293,  -293,    -8,   238,  -293,   236,
     -32,  -293,   115,   480,   -90,  -293,   -36,  -107,  -293,  -179,
    -145,  -293,   -14,  -293,  -135,     1,   273,  -293,    70,  -292,
     282,   -97,  -124,   -79,  -293,    -9,    88,  -293,   117,    12,
    -293,   -53,   339,  -293,  -293,  -293,   345,    10
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -191
static const yytype_int16 yytable[] =
{
      39,    40,    43,    43,   230,    43,   115,   226,    97,    59,
      43,    43,    45,    35,   156,    34,    77,   196,    -3,   206,
      78,    36,     6,    80,    48,   228,   210,   357,     6,    90,
      91,   102,     6,    98,    99,   232,   100,    59,    43,   103,
     114,   105,   106,    71,   151,    -3,   297,    78,    75,    43,
     103,   207,     6,     5,   271,     6,   375,   239,     1,   150,
     257,   129,     6,    75,    43,    43,    41,    42,    37,   208,
      55,     1,   168,   169,   125,    75,   145,   301,   240,   285,
      44,    79,    47,    50,   298,   152,     4,     6,    58,    60,
     278,    41,    42,    79,   300,   208,    43,   286,   392,   324,
     203,    79,  -190,    10,    11,   258,   259,    56,   146,   281,
      20,   211,   292,    21,   193,   209,    47,   208,  -190,    22,
      23,    24,    25,   223,   227,   147,    72,    44,   279,    -3,
      46,    43,   275,   276,    37,    20,   131,    37,    21,   280,
     132,   317,   123,    -3,    22,    23,    24,    25,   214,    73,
       6,   216,   243,   244,   290,    38,  -150,   343,   248,   247,
      73,   156,   249,     6,   214,   215,     6,   216,   217,     1,
      51,    52,   339,   284,   192,   208,   218,    53,   245,   359,
     381,    43,    43,   370,   274,    43,    43,   133,   134,   227,
     203,    37,   218,   388,   204,   223,   227,   221,   372,    54,
      43,   371,   135,   193,   193,    43,    69,   219,   220,   123,
     403,   312,   314,   221,   125,   139,     6,   282,   140,   136,
     137,    43,   104,   214,   215,     6,   216,   217,   208,    74,
      43,    61,   152,   152,    62,   328,    47,    47,   316,    70,
      75,   383,    63,    64,    76,    65,   329,   387,    66,   157,
     346,   218,   158,    85,   398,    84,    67,    68,    86,   123,
     272,   340,   341,   192,   192,   326,   219,   220,    89,    75,
      73,   107,   221,   101,   242,    92,    43,     6,   123,    93,
      94,    41,    42,   369,   111,   337,   108,   113,   223,   227,
     227,   255,   256,   116,    11,   262,   263,   264,   265,   123,
     267,   268,   269,   270,   112,    92,   128,   117,   306,    93,
       6,    43,    92,    75,    41,    42,    93,    96,    85,   361,
     190,    19,   289,    86,   118,    20,    57,    11,    21,   203,
     119,   120,   130,   204,    22,    23,    24,    25,   141,     6,
      43,    92,   144,    41,    42,    93,   311,   142,   189,   139,
     194,   195,   305,   197,   332,   198,    11,   223,    20,   223,
     315,    21,   201,   223,   199,   202,   203,    22,    23,    24,
      25,   393,    92,   213,   212,   229,    93,   313,   231,   121,
     234,    43,   235,   238,   241,   250,   251,    20,   223,   306,
      21,   404,   252,   253,   254,   273,    22,    23,    24,    25,
     261,   277,   266,   283,   293,   287,   288,   295,   296,   299,
     302,   303,   131,   325,   304,   310,   318,   320,   306,   319,
     321,   322,   323,   327,   140,   331,   350,   351,   352,   353,
     330,   333,   355,   356,   334,   342,   347,   344,   349,   354,
     358,   360,   362,   348,   363,   365,   366,   373,   367,   368,
     376,   399,   379,   377,   380,   382,   389,   397,     6,   123,
     237,   400,   160,   161,   396,   401,   162,   405,   395,   378,
     163,   164,   260,   165,    10,    11,   308,   166,   167,   309,
     168,   169,   170,    28,   402,   386,   294,   233,   236,     0,
     291,   390,   391,     0,    15,   394,   171,     0,     0,     0,
       0,    18,    19,     0,     0,     0,    20,     0,     0,    21,
       0,     0,     0,   172,   173,    22,    23,    24,    25,     0,
       0,     0,     6,   174,   175,     7,     8,     9,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    10,    11,
       6,    12,     0,     0,   246,    42,    13,     0,     0,     0,
       0,     0,     0,     0,     0,    14,    57,    11,    15,    16,
      17,     0,     0,     0,     0,    18,    19,     6,     0,     0,
      20,    41,    42,    21,     0,     0,     0,     0,     0,    22,
      23,    24,    25,     0,    11,     6,     0,     0,    20,    41,
      49,    21,     0,     0,     0,     0,     0,    22,    23,    24,
      25,     0,    11,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    20,     0,     0,    21,     0,
       0,     0,     0,     0,    22,    23,    24,    25,     0,     0,
       0,     0,     0,    20,     0,     0,    21,     0,     0,     0,
       0,     0,    22,    23,    24,    25
};

static const yytype_int16 yycheck[] =
{
       8,     9,    10,    11,   139,    13,    59,   131,    40,    18,
      18,    19,    11,     3,    93,     3,    30,   107,     0,   116,
       6,     4,     6,    31,    12,   132,   123,   319,     6,    37,
      38,    45,     6,    41,    42,     6,    44,    46,    46,    47,
       6,    49,    50,    26,    22,    14,    31,     6,    34,    57,
      58,    15,     6,     0,   189,     6,   348,    12,    40,    91,
      16,    75,     6,    34,    72,    73,    10,    11,    52,    33,
       6,    40,    28,    29,    73,    34,    84,    15,    33,   203,
      10,    67,    12,    13,    69,    93,    12,     6,    18,    19,
     197,    10,    11,    67,   229,    33,   104,   204,    52,    15,
      51,    67,    17,    22,    23,    61,    62,    43,    17,   199,
      54,   125,   209,    57,   104,   123,    46,    33,    33,    63,
      64,    65,    66,   131,   132,    34,    13,    57,     6,     0,
      49,   139,   193,   194,    52,    54,    51,    52,    57,    17,
      55,    17,    72,    14,    63,    64,    65,    66,     4,    36,
       6,     7,   160,   161,   207,     6,    17,    15,   167,   167,
      36,   240,   170,     6,     4,     5,     6,     7,     8,    40,
      17,     6,   296,    13,   104,    33,    32,    12,   166,   324,
     359,   189,   190,    15,   192,   193,   194,    36,    37,   197,
      51,    52,    32,   372,    55,   203,   204,    53,   343,     6,
     208,    33,    51,   193,   194,   213,    54,    47,    48,   139,
      15,   243,   244,    53,   213,    13,     6,   200,    16,    68,
      69,   229,    12,     4,     5,     6,     7,     8,    33,    23,
     238,    54,   240,   241,    57,     6,   166,   167,   246,    54,
      34,   365,    65,    66,    38,    54,    17,   371,    57,    70,
     303,    32,    73,    12,   389,    35,    65,    66,    17,   189,
     190,   297,   298,   193,   194,   273,    47,    48,    14,    34,
      36,    12,    53,    38,   159,    12,   284,     6,   208,    16,
      17,    10,    11,   336,    12,   293,     6,     6,   296,   297,
     298,   176,   177,     6,    23,   180,   181,   182,   183,   229,
     185,   186,   187,   188,    52,    12,    38,    54,   238,    16,
       6,   319,    12,    34,    10,    11,    16,    17,    12,   327,
      49,    50,    16,    17,    54,    54,    22,    23,    57,    51,
      54,    54,    23,    55,    63,    64,    65,    66,     6,     6,
     348,    12,    17,    10,    11,    16,    17,    33,    13,    13,
      12,    17,   237,    51,   284,    14,    23,   365,    54,   367,
     245,    57,     6,   371,    33,    17,    51,    63,    64,    65,
      66,   379,    12,    33,    37,    13,    16,    17,     7,    46,
      13,   389,    14,    36,    18,     6,    16,    54,   396,   319,
      57,   399,    16,    16,    16,    14,    63,    64,    65,    66,
      16,    14,    17,    17,    52,    17,    17,     7,    13,    56,
      17,    15,    51,     6,    17,    17,    17,    52,   348,    36,
      16,    16,    16,    14,    16,    14,   311,   312,   313,   314,
      17,    56,   317,   318,     6,    17,    17,    23,    37,    17,
       6,    17,    17,    33,    17,    15,    17,    24,    13,    33,
      17,     6,    37,    17,    17,    17,    13,    15,     6,   389,
     150,    17,    10,    11,    33,    53,    14,    17,    51,   354,
      18,    19,   178,    21,    22,    23,   240,    25,    26,   241,
      28,    29,    30,     3,   396,   368,   213,   142,   149,    -1,
     208,   376,   377,    -1,    42,   380,    44,    -1,    -1,    -1,
      -1,    49,    50,    -1,    -1,    -1,    54,    -1,    -1,    57,
      -1,    -1,    -1,    61,    62,    63,    64,    65,    66,    -1,
      -1,    -1,     6,    71,    72,     9,    10,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,
       6,    25,    -1,    -1,    10,    11,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    22,    23,    42,    43,
      44,    -1,    -1,    -1,    -1,    49,    50,     6,    -1,    -1,
      54,    10,    11,    57,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    -1,    23,     6,    -1,    -1,    54,    10,
      11,    57,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    -1,    -1,
      -1,    -1,    -1,    54,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    66
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    40,    75,    76,    12,     0,     6,     9,    10,    11,
      22,    23,    25,    30,    39,    42,    43,    44,    49,    50,
      54,    57,    63,    64,    65,    66,    80,    90,    97,   105,
     109,   112,   117,   119,   123,   131,    75,    52,     6,    90,
      90,    10,    11,    90,   112,   109,    49,   112,   123,    11,
     112,    17,     6,    12,     6,     6,    43,    22,   112,   119,
     112,    54,    57,    65,    66,    54,    57,    65,    66,    54,
      54,    75,    13,    36,    23,    34,    38,   106,     6,    67,
      90,   106,   129,   130,    35,    12,    17,   124,   125,    14,
      90,    90,    12,    16,    17,    94,    17,    94,    90,    90,
      90,    38,   106,    90,    12,    90,    90,    12,     6,    98,
      99,    12,    52,     6,     6,   125,     6,    54,    54,    54,
      54,    46,   108,   112,   114,   109,   110,   111,    38,   106,
      23,    51,    55,    36,    37,    51,    68,    69,   118,    13,
      16,     6,    33,   128,    17,    90,    17,    34,   126,   127,
      94,    22,    90,    91,    92,    93,   117,    70,    73,    79,
      10,    11,    14,    18,    19,    21,    25,    26,    28,    29,
      30,    44,    61,    62,    71,    72,    77,    78,    84,    85,
      86,    87,    88,    89,    96,    97,   102,   123,   131,    13,
      49,    82,   112,   131,    12,    17,    98,    51,    14,    33,
      81,     6,    17,    51,    55,   115,   115,    15,    33,    90,
     115,   106,    37,    33,     4,     5,     7,     8,    32,    47,
      48,    53,    83,    90,   100,   101,   116,    90,   101,    13,
     108,     7,     6,   130,    13,    14,   126,    79,    36,    12,
      33,    18,    96,    90,    90,   123,    10,    90,   119,    90,
       6,    16,    16,    16,    16,    96,    96,    16,    61,    62,
      85,    16,    96,    96,    96,    96,    17,    96,    96,    96,
      96,   108,   112,    14,    90,    82,    82,    14,   101,     6,
      17,    98,    75,    17,    13,   116,   101,    17,    17,    16,
     125,   114,   115,    52,   110,     7,    13,    31,    69,    56,
     108,    15,    17,    15,    17,    96,   112,   113,    93,    91,
      17,    17,    94,    17,    94,    96,    90,    17,    17,    36,
      52,    16,    16,    16,    15,     6,    90,    14,     6,    17,
      17,    14,   112,    56,     6,   121,   122,    90,   107,   116,
     100,   100,    17,    15,    23,   104,   125,    17,    33,    37,
      96,    96,    96,    96,    17,    96,    96,   113,     6,   104,
      17,    90,    17,    17,    95,    15,    17,    13,    33,   125,
      15,    33,   104,    24,   103,   113,    17,    17,    96,    37,
      17,   103,    17,   116,   116,   120,   122,   116,   103,    13,
      96,    96,    52,    90,    96,    51,    33,    15,   108,     6,
      17,    53,   120,    15,    90,    17
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
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
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
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
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
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
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



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
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
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
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
#line 178 "yacc.yy"
    {
    ;}
    break;

  case 4:
#line 185 "yacc.yy"
    {
      printf("<INCLUDE>%s</INCLUDE>\n", (yyvsp[(2) - (2)]._str)->latin1() );
    ;}
    break;

  case 5:
#line 189 "yacc.yy"
    {
    ;}
    break;

  case 6:
#line 192 "yacc.yy"
    {
    ;}
    break;

  case 7:
#line 198 "yacc.yy"
    {
    /*empty*/
  ;}
    break;

  case 8:
#line 205 "yacc.yy"
    {
    /*empty*/
  ;}
    break;

  case 9:
#line 211 "yacc.yy"
    { (yyval._int) = 1; ;}
    break;

  case 10:
#line 212 "yacc.yy"
    { (yyval._int) = 2; ;}
    break;

  case 11:
#line 213 "yacc.yy"
    { (yyval._int) = 0; ;}
    break;

  case 12:
#line 218 "yacc.yy"
    {
      printf("<CLASS>\n    <NAME>%s</NAME>\n%s%s</CLASS>\n", MString( in_namespace + *(yyvsp[(2) - (6)]._str) ).latin1(), (yyvsp[(3) - (6)]._str)->latin1(), (yyvsp[(5) - (6)]._str)->latin1() );
      mocking_area = 0;
    ;}
    break;

  case 13:
#line 223 "yacc.yy"
    {
      printf("<CLASS>\n    <NAME>%s</NAME>\n    <LINK_SCOPE>%s</LINK_SCOPE>\n%s%s</CLASS>\n", MString( in_namespace + *(yyvsp[(3) - (7)]._str) ).latin1(), (yyvsp[(2) - (7)]._str)->latin1(),  (yyvsp[(4) - (7)]._str)->latin1(), (yyvsp[(6) - (7)]._str)->latin1() );
      mocking_area = 0;

    ;}
    break;

  case 14:
#line 229 "yacc.yy"
    {
    ;}
    break;

  case 15:
#line 232 "yacc.yy"
    {
    ;}
    break;

  case 16:
#line 235 "yacc.yy"
    {
    ;}
    break;

  case 17:
#line 238 "yacc.yy"
    {
          in_namespace += *(yyvsp[(2) - (3)]._str); in_namespace += "::";
      ;}
    break;

  case 18:
#line 242 "yacc.yy"
    {
            std::size_t pos = in_namespace.findRev( "::", -3 );
            if( pos != MString::npos )
                in_namespace = in_namespace.left( pos + 2 );
            else
                in_namespace = "";
        ;}
    break;

  case 19:
#line 250 "yacc.yy"
    {
    ;}
    break;

  case 20:
#line 253 "yacc.yy"
    {
    ;}
    break;

  case 21:
#line 256 "yacc.yy"
    {
    ;}
    break;

  case 22:
#line 259 "yacc.yy"
    {
    ;}
    break;

  case 23:
#line 262 "yacc.yy"
    {
    ;}
    break;

  case 24:
#line 265 "yacc.yy"
    {
    ;}
    break;

  case 25:
#line 268 "yacc.yy"
    {
    ;}
    break;

  case 26:
#line 271 "yacc.yy"
    {
    ;}
    break;

  case 27:
#line 274 "yacc.yy"
    {
    ;}
    break;

  case 28:
#line 277 "yacc.yy"
    {
    ;}
    break;

  case 35:
#line 289 "yacc.yy"
    {
    mocking_area = 0;
  ;}
    break;

  case 38:
#line 298 "yacc.yy"
    {
  ;}
    break;

  case 39:
#line 302 "yacc.yy"
    {
  ;}
    break;

  case 40:
#line 308 "yacc.yy"
    {
    mocking_area = 0;
  ;}
    break;

  case 41:
#line 315 "yacc.yy"
    {
    mockpp_methods_area = 1;
    mocking_area = 1;
  ;}
    break;

  case 42:
#line 320 "yacc.yy"
    {
    mockpp_methods_area = 1;
    mocking_area = 1;
  ;}
    break;

  case 43:
#line 328 "yacc.yy"
    {
    mockpp_constructor_area = 1;
    mocking_area = 1;
  ;}
    break;

  case 44:
#line 333 "yacc.yy"
    {
    mockpp_constructor_area = 1;
    mocking_area = 1;
  ;}
    break;

  case 45:
#line 341 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(1) - (1)]._str);
    ;}
    break;

  case 46:
#line 345 "yacc.yy"
    {
      MString* tmp = new MString( "%1::%2" );
            *tmp = tmp->arg(*((yyvsp[(1) - (3)]._str))).arg(*((yyvsp[(3) - (3)]._str)));
            (yyval._str) = tmp;
    ;}
    break;

  case 47:
#line 354 "yacc.yy"
    {
      MString* tmp = new MString( "    <SUPER>%1</SUPER>\n" );
      *tmp = tmp->arg( *((yyvsp[(1) - (1)]._str)) );
      (yyval._str) = tmp;
    ;}
    break;

  case 48:
#line 360 "yacc.yy"
    {
      MString* tmp = new MString( "    <SUPER>%1</SUPER>\n" );
      *tmp = tmp->arg( *((yyvsp[(1) - (4)]._str)) + "&lt;" + *((yyvsp[(3) - (4)]._str)) + "&gt;" );
      (yyval._str) = tmp;
    ;}
    break;

  case 49:
#line 369 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(3) - (3)]._str);
    ;}
    break;

  case 50:
#line 373 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(1) - (1)]._str);
    ;}
    break;

  case 51:
#line 380 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(1) - (2)]._str);
    ;}
    break;

  case 52:
#line 384 "yacc.yy"
    {
      (yyval._str) = new MString( *((yyvsp[(1) - (3)]._str)) + *((yyvsp[(3) - (3)]._str)) );
    ;}
    break;

  case 53:
#line 391 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(2) - (2)]._str);
    ;}
    break;

  case 54:
#line 395 "yacc.yy"
    {
      (yyval._str) = new MString( "" );
    ;}
    break;

  case 55:
#line 402 "yacc.yy"
    {
    ;}
    break;

  case 57:
#line 409 "yacc.yy"
    {
      (yyval._str) = new MString( "" );
    ;}
    break;

  case 58:
#line 413 "yacc.yy"
    {
      (yyval._str) = new MString( *((yyvsp[(1) - (2)]._str)) + *((yyvsp[(2) - (2)]._str)) );
    ;}
    break;

  case 59:
#line 417 "yacc.yy"
    {
      (yyval._str) = new MString( *((yyvsp[(2) - (3)]._str)) + *((yyvsp[(3) - (3)]._str)) );
    ;}
    break;

  case 60:
#line 421 "yacc.yy"
    {
      (yyval._str) = new MString( *((yyvsp[(1) - (2)]._str)) + *((yyvsp[(2) - (2)]._str)) );
    ;}
    break;

  case 61:
#line 425 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(2) - (2)]._str);
    ;}
    break;

  case 62:
#line 429 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(2) - (2)]._str);
    ;}
    break;

  case 63:
#line 433 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(2) - (2)]._str);
    ;}
    break;

  case 64:
#line 437 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(2) - (2)]._str);
    ;}
    break;

  case 65:
#line 441 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(2) - (2)]._str);
    ;}
    break;

  case 66:
#line 445 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(2) - (2)]._str);
    ;}
    break;

  case 67:
#line 449 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(2) - (2)]._str);
    ;}
    break;

  case 68:
#line 453 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(2) - (2)]._str);
    ;}
    break;

  case 69:
#line 457 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(5) - (5)]._str);
    ;}
    break;

  case 70:
#line 461 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(4) - (4)]._str);
    ;}
    break;

  case 71:
#line 465 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(4) - (4)]._str);
    ;}
    break;

  case 72:
#line 469 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(4) - (4)]._str);
    ;}
    break;

  case 73:
#line 473 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(6) - (6)]._str);
    ;}
    break;

  case 74:
#line 477 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(4) - (4)]._str);
    ;}
    break;

  case 75:
#line 481 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(6) - (6)]._str);
    ;}
    break;

  case 76:
#line 485 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(6) - (6)]._str);
    ;}
    break;

  case 83:
#line 503 "yacc.yy"
    {;}
    break;

  case 84:
#line 504 "yacc.yy"
    {;}
    break;

  case 85:
#line 508 "yacc.yy"
    {;}
    break;

  case 86:
#line 509 "yacc.yy"
    {;}
    break;

  case 87:
#line 510 "yacc.yy"
    {;}
    break;

  case 88:
#line 511 "yacc.yy"
    {;}
    break;

  case 89:
#line 512 "yacc.yy"
    {;}
    break;

  case 90:
#line 516 "yacc.yy"
    {;}
    break;

  case 91:
#line 517 "yacc.yy"
    {;}
    break;

  case 92:
#line 518 "yacc.yy"
    {;}
    break;

  case 93:
#line 523 "yacc.yy"
    {
      if (mocking_area)
      {
        MString* tmp = new MString("<TYPEDEF name=\"%1\" template=\"%2\">%3</TYPEDEF>\n");
        *tmp = tmp->arg( *((yyvsp[(6) - (7)]._str)) ).arg( *((yyvsp[(2) - (7)]._str)) ).arg( *((yyvsp[(4) - (7)]._str)) );
        (yyval._str) = tmp;
      } else
      {
        (yyval._str) = new MString("");
      }
    ;}
    break;

  case 94:
#line 535 "yacc.yy"
    {
      if (mocking_area)
        yyerror("scoped template typedefs are not supported in mock methods!");
    ;}
    break;

  case 95:
#line 543 "yacc.yy"
    {
        (yyval._str) = new MString("");
    ;}
    break;

  case 96:
#line 547 "yacc.yy"
    {
        (yyval._str) = new MString("/* todo */");
    ;}
    break;

  case 97:
#line 554 "yacc.yy"
    {
      (yyval._int) = 0;
    ;}
    break;

  case 98:
#line 558 "yacc.yy"
    {
      (yyval._int) = 1;
    ;}
    break;

  case 99:
#line 564 "yacc.yy"
    { (yyval._str) = new MString("signed int"); ;}
    break;

  case 100:
#line 565 "yacc.yy"
    { (yyval._str) = new MString("signed int"); ;}
    break;

  case 101:
#line 566 "yacc.yy"
    { (yyval._str) = new MString("unsigned int"); ;}
    break;

  case 102:
#line 567 "yacc.yy"
    { (yyval._str) = new MString("unsigned int"); ;}
    break;

  case 103:
#line 568 "yacc.yy"
    { (yyval._str) = new MString("signed short int"); ;}
    break;

  case 104:
#line 569 "yacc.yy"
    { (yyval._str) = new MString("signed short int"); ;}
    break;

  case 105:
#line 570 "yacc.yy"
    { (yyval._str) = new MString("signed long int"); ;}
    break;

  case 106:
#line 571 "yacc.yy"
    { (yyval._str) = new MString("signed long int"); ;}
    break;

  case 107:
#line 572 "yacc.yy"
    { (yyval._str) = new MString("unsigned short int"); ;}
    break;

  case 108:
#line 573 "yacc.yy"
    { (yyval._str) = new MString("unsigned short int"); ;}
    break;

  case 109:
#line 574 "yacc.yy"
    { (yyval._str) = new MString("unsigned long int"); ;}
    break;

  case 110:
#line 575 "yacc.yy"
    { (yyval._str) = new MString("unsigned long int"); ;}
    break;

  case 111:
#line 576 "yacc.yy"
    { (yyval._str) = new MString("int"); ;}
    break;

  case 112:
#line 577 "yacc.yy"
    { (yyval._str) = new MString("long int"); ;}
    break;

  case 113:
#line 578 "yacc.yy"
    { (yyval._str) = new MString("long int"); ;}
    break;

  case 114:
#line 579 "yacc.yy"
    { (yyval._str) = new MString("short int"); ;}
    break;

  case 115:
#line 580 "yacc.yy"
    { (yyval._str) = new MString("short int"); ;}
    break;

  case 116:
#line 581 "yacc.yy"
    { (yyval._str) = new MString("char"); ;}
    break;

  case 117:
#line 582 "yacc.yy"
    { (yyval._str) = new MString("signed char"); ;}
    break;

  case 118:
#line 583 "yacc.yy"
    { (yyval._str) = new MString("unsigned char"); ;}
    break;

  case 119:
#line 588 "yacc.yy"
    {
      (yyval._str) = new MString( MString("*") + *((yyvsp[(2) - (2)]._str)) );
    ;}
    break;

  case 120:
#line 592 "yacc.yy"
    {
      (yyval._str) = new MString("*");
    ;}
    break;

  case 121:
#line 599 "yacc.yy"
    {
    ;}
    break;

  case 122:
#line 602 "yacc.yy"
    {
      (yyval._str) = new MString( *((yyvsp[(1) - (3)]._str)) );
    ;}
    break;

  case 123:
#line 609 "yacc.yy"
    {
      (yyval._str) = new MString( "" );
    ;}
    break;

  case 125:
#line 614 "yacc.yy"
    {
      (yyval._str) = new MString( *((yyvsp[(1) - (3)]._str)) + *((yyvsp[(3) - (3)]._str)) );
    ;}
    break;

  case 127:
#line 622 "yacc.yy"
    { (yyval._str) = (yyvsp[(1) - (1)]._str); ;}
    break;

  case 128:
#line 623 "yacc.yy"
    { (yyval._str) = (yyvsp[(2) - (2)]._str); ;}
    break;

  case 129:
#line 624 "yacc.yy"
    { (yyval._str) = (yyvsp[(2) - (2)]._str); ;}
    break;

  case 130:
#line 626 "yacc.yy"
    {
      MString *tmp = new MString("%1&lt;%2&gt;");
      *tmp = tmp->arg(*((yyvsp[(1) - (4)]._str)));
      *tmp = tmp->arg(*((yyvsp[(3) - (4)]._str)));
      (yyval._str) = tmp;
    ;}
    break;

  case 131:
#line 633 "yacc.yy"
    {
      MString *tmp = new MString("%1&lt;%2&gt;::%3");
      *tmp = tmp->arg(*((yyvsp[(1) - (6)]._str)));
      *tmp = tmp->arg(*((yyvsp[(3) - (6)]._str)));
      *tmp = tmp->arg(*((yyvsp[(6) - (6)]._str)));
      (yyval._str) = tmp;
    ;}
    break;

  case 132:
#line 645 "yacc.yy"
    {
      (yyval._str) = new MString(*((yyvsp[(1) - (3)]._str)) + "," + *((yyvsp[(3) - (3)]._str)));
    ;}
    break;

  case 133:
#line 649 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(1) - (1)]._str);
    ;}
    break;

  case 134:
#line 657 "yacc.yy"
    {
      if (mocking_area)
        yyerror("in mock methods are no pointers allowed");
    ;}
    break;

  case 135:
#line 662 "yacc.yy"
    {
      (yyval._str) = (yyvsp[(1) - (1)]._str);
    ;}
    break;

  case 136:
#line 671 "yacc.yy"
    {
         if (mocking_area)
         {
            MString* tmp = new MString("<TYPE  qleft=\"const\" qright=\"%2\">%1</TYPE>");
            *tmp = tmp->arg( *((yyvsp[(2) - (3)]._str)) );
            *tmp = tmp->arg( *((yyvsp[(3) - (3)]._str)) );
            (yyval._str) = tmp;
         }
      ;}
    break;

  case 137:
#line 681 "yacc.yy"
    {
         if (mocking_area)
         {
            MString* tmp = new MString("<TYPE  qleft=\"const\" qright=\"" AMP_ENTITY "\">%1</TYPE>");
            *tmp = tmp->arg( *((yyvsp[(2) - (3)]._str)) );
            (yyval._str) = tmp;
         }
      ;}
    break;

  case 138:
#line 690 "yacc.yy"
    {
        MString* tmp = new MString("<TYPE qleft=\"const\" >%1</TYPE>");
        *tmp = tmp->arg( *((yyvsp[(2) - (2)]._str)) );
        (yyval._str) = tmp;
      ;}
    break;

  case 139:
#line 696 "yacc.yy"
    {
        MString* tmp = new MString("<TYPE qleft=\"const\">%1</TYPE>");
        *tmp = tmp->arg( *((yyvsp[(1) - (2)]._str)) );
        (yyval._str) = tmp;
      ;}
    break;

  case 140:
#line 702 "yacc.yy"
    {
        if (mocking_area)
        {
           MString* tmp = new MString("<TYPE qleft=\"const\" qright=\"" AMP_ENTITY "\">%1</TYPE>");
           *tmp = tmp->arg( *((yyvsp[(1) - (3)]._str)) );
           (yyval._str) = tmp;
        }
      ;}
    break;

  case 141:
#line 711 "yacc.yy"
    {
        if (mocking_area)
        {
           MString* tmp = new MString("<TYPE qright=\"" AMP_ENTITY "\">%1</TYPE>");
           *tmp = tmp->arg( *((yyvsp[(1) - (2)]._str)) );
           (yyval._str) = tmp;
        }
      ;}
    break;

  case 142:
#line 720 "yacc.yy"
    {
        MString* tmp = new MString("<TYPE>%1</TYPE>");
        *tmp = tmp->arg( *((yyvsp[(1) - (1)]._str)) );
        (yyval._str) = tmp;
      ;}
    break;

  case 143:
#line 726 "yacc.yy"
    {
        if (mocking_area)
        {
           MString* tmp = new MString("<TYPE qright=\"%2 const\">%1</TYPE>");
           *tmp = tmp->arg( *((yyvsp[(1) - (3)]._str)) );
           *tmp = tmp->arg( *((yyvsp[(2) - (3)]._str)) );
           (yyval._str) = tmp;
        }
      ;}
    break;

  case 144:
#line 736 "yacc.yy"
    {
        if (mocking_area)
        {
           MString* tmp = new MString("<TYPE qright=\"%2\">%1</TYPE>");
           *tmp = tmp->arg( *((yyvsp[(1) - (2)]._str)) );
           *tmp = tmp->arg( *((yyvsp[(2) - (2)]._str)) );
           (yyval._str) = tmp;
        }
      ;}
    break;

  case 145:
#line 749 "yacc.yy"
    {
      (yyval._str) = new MString(*((yyvsp[(1) - (3)]._str)) + "," + *((yyvsp[(3) - (3)]._str)));
    ;}
    break;

  case 146:
#line 753 "yacc.yy"
    {
       (yyval._str) = (yyvsp[(1) - (1)]._str);
    ;}
    break;

  case 147:
#line 760 "yacc.yy"
    {
      if (mocking_area)
      {
        MString* tmp = new MString("\n        <ARG>%1<NAME>%2</NAME></ARG>");
        *tmp = tmp->arg( *((yyvsp[(1) - (3)]._str)) );
        *tmp = tmp->arg( *((yyvsp[(2) - (3)]._str)) );
        (yyval._str) = tmp;
      }
      else
        (yyval._str) = new MString();
    ;}
    break;

  case 148:
#line 772 "yacc.yy"
    {
      if (mocking_area)
      {
         MString* tmp = new MString("\n        <ARG>%1</ARG>");
         *tmp = tmp->arg( *((yyvsp[(1) - (2)]._str)) );
         (yyval._str) = tmp;
      }
      else
        (yyval._str) = new MString();
    ;}
    break;

  case 149:
#line 783 "yacc.yy"
    {
      if (mocking_area)
        yyerror("variable arguments not supported in mock methods.");
      (yyval._str) = new MString("");
    ;}
    break;

  case 150:
#line 792 "yacc.yy"
    {
    ;}
    break;

  case 151:
#line 795 "yacc.yy"
    {
    ;}
    break;

  case 152:
#line 798 "yacc.yy"
    {
    ;}
    break;

  case 153:
#line 805 "yacc.yy"
    {
    ;}
    break;

  case 154:
#line 808 "yacc.yy"
    {
    ;}
    break;

  case 155:
#line 811 "yacc.yy"
    {
    ;}
    break;

  case 156:
#line 814 "yacc.yy"
    {
    ;}
    break;

  case 157:
#line 817 "yacc.yy"
    {
    ;}
    break;

  case 158:
#line 822 "yacc.yy"
    { (yyval._int) = 0;  ;}
    break;

  case 159:
#line 823 "yacc.yy"
    { (yyval._int) = 1; ;}
    break;

  case 165:
#line 832 "yacc.yy"
    {
      if (mocking_area)
      {
        MString* tmp = 0;
        tmp = new MString(
                "    <%4>\n"
                "        %2\n"
                "        <NAME>%1</NAME>"
                "%3\n"
                "    </%5>\n");
        *tmp = tmp->arg( *((yyvsp[(2) - (7)]._str)) );
        *tmp = tmp->arg( *((yyvsp[(1) - (7)]._str)) );
        *tmp = tmp->arg( *((yyvsp[(4) - (7)]._str)) );

        MString tagname = "METHOD";

        MString attr = ((yyvsp[(6) - (7)]._int)) ? " qual=\"const\"" : "";
        MString thrower (*((yyvsp[(7) - (7)]._str)));
        if (thrower.length() != 0)
          attr += MString(" throw='%1'").arg(thrower);
        *tmp = tmp->arg( MString("%1%2").arg(tagname).arg(attr) );
        *tmp = tmp->arg( MString("%1").arg(tagname) );
        (yyval._str) = tmp;
      }
      else
        (yyval._str) = new MString("");
    ;}
    break;

  case 166:
#line 860 "yacc.yy"
    {
      if (mocking_area)
      {
        MString* tmp = 0;
        tmp = new MString(
                "    <%4>\n"
                "        %2\n"
                "        <NAME>%1</NAME>"
                "%3\n"
                "    </%5>\n");
        *tmp = tmp->arg( *((yyvsp[(3) - (8)]._str)) );
        *tmp = tmp->arg( *((yyvsp[(2) - (8)]._str)) );
        *tmp = tmp->arg( *((yyvsp[(5) - (8)]._str)) );

        MString tagname = "METHOD";
        MString attr = ((yyvsp[(7) - (8)]._int)) ? " qual=\"const\"" : "";
        MString thrower (*((yyvsp[(8) - (8)]._str)));
        if (thrower.length() != 0)
          attr += MString(" throw='%1'").arg(thrower);
        attr += " virtual='1'";
        *tmp = tmp->arg( MString("%1%2").arg(tagname).arg(attr) );
        *tmp = tmp->arg( MString("%1").arg(tagname) );
        (yyval._str) = tmp;
      }
      else
        (yyval._str) = new MString("");
    ;}
    break;

  case 167:
#line 888 "yacc.yy"
    {
      if (mocking_area)
      {
        MString* tmp = 0;
        tmp = new MString(
                "    <%4>\n"
                "        %2\n"
                "        <NAME>%1</NAME>"
                "%3\n"
                "    </%5>\n");
        *tmp = tmp->arg( *((yyvsp[(3) - (10)]._str)) );
        *tmp = tmp->arg( *((yyvsp[(2) - (10)]._str)) );
        *tmp = tmp->arg( *((yyvsp[(5) - (10)]._str)) );

        MString tagname = "METHOD";

        MString attr = ((yyvsp[(7) - (10)]._int)) ? " qual=\"const\"" : "";
        attr += " virtual='1' pure='1'";
        MString thrower (*((yyvsp[(8) - (10)]._str)));
        if (thrower.length() != 0)
          attr += MString(" throw='%1'").arg(thrower);
        *tmp = tmp->arg( MString("%1%2").arg(tagname).arg(attr) );
        *tmp = tmp->arg( MString("%1").arg(tagname) );
        (yyval._str) = tmp;
      }
      else
       (yyval._str) = new MString("");
    ;}
    break;

  case 168:
#line 917 "yacc.yy"
    {
      if (mocking_area)
        yyerror("operators aren't allowed in mock methods!");
       (yyval._str) = new MString("");
    ;}
    break;

  case 169:
#line 927 "yacc.yy"
    {;}
    break;

  case 170:
#line 928 "yacc.yy"
    {;}
    break;

  case 171:
#line 929 "yacc.yy"
    {;}
    break;

  case 172:
#line 934 "yacc.yy"
    {;}
    break;

  case 173:
#line 939 "yacc.yy"
    {;}
    break;

  case 174:
#line 940 "yacc.yy"
    {;}
    break;

  case 175:
#line 945 "yacc.yy"
    {
          (yyval._str) = (yyvsp[(1) - (2)]._str);
    ;}
    break;

  case 176:
#line 959 "yacc.yy"
    {
      /* The constructor */
//      assert(!mockpp_methods_area);
      if (mockpp_constructor_area)
      {
        MString* tmp = 0;
        tmp = new MString(
                "    <%4>"
                "%2\n"
                "        <NAME>%1</NAME>\n"
                "    </%3>\n");
        *tmp = tmp->arg( *((yyvsp[(1) - (5)]._str)) );
        if (((yyvsp[(3) - (5)]._str))->length() != 0)
          *tmp = tmp->arg( *((yyvsp[(3) - (5)]._str)) );
        else
          *tmp = tmp->arg("");

        MString tagname = "CONSTRUCTOR";
        mocking_area = 0;
        mockpp_constructor_area = 0;  // only once

        *tmp = tmp->arg(tagname).arg(tagname);
        (yyval._str) = tmp;
      }
      else
        (yyval._str) = new MString("");
    ;}
    break;

  case 177:
#line 987 "yacc.yy"
    {
      if (mockpp_constructor_area)
      {
        MString* tmp = 0;
        tmp = new MString(
                "    <%4>"
                "%2\n"
                "        <NAME>%1</NAME>\n"
                "    </%3>\n");
        *tmp = tmp->arg( *((yyvsp[(1) - (7)]._str)) );
        if (((yyvsp[(3) - (7)]._str))->length() != 0)
          *tmp = tmp->arg( *((yyvsp[(3) - (7)]._str)));
        else
          *tmp = tmp->arg("");


        MString tagname = "CONSTRUCTOR";
        mocking_area = 0;
        mockpp_constructor_area = 0;  // only once

        *tmp = tmp->arg(tagname).arg(tagname);
        (yyval._str) = tmp;
      }
      else
        (yyval._str) = new MString("");
    ;}
    break;

  case 178:
#line 1014 "yacc.yy"
    {
        /* The destructor */
//        assert(!mockpp_methods_area);
//        assert(!mockpp_constructor_area);
        (yyval._str) = new MString("");
    ;}
    break;

  case 179:
#line 1021 "yacc.yy"
    {
      if (mocking_area)
         yyerror("mock methods cannot be static");

      else
      {
         (yyval._str) = new MString();
      }
    ;}
    break;

  case 180:
#line 1033 "yacc.yy"
    {
    function_mode = 1;
  ;}
    break;

  case 184:
#line 1045 "yacc.yy"
    {;}
    break;

  case 186:
#line 1047 "yacc.yy"
    {;}
    break;

  case 190:
#line 1059 "yacc.yy"
    {;}
    break;

  case 191:
#line 1060 "yacc.yy"
    {;}
    break;

  case 192:
#line 1061 "yacc.yy"
    {;}
    break;

  case 193:
#line 1064 "yacc.yy"
    {;}
    break;

  case 194:
#line 1068 "yacc.yy"
    {;}
    break;

  case 195:
#line 1069 "yacc.yy"
    {;}
    break;

  case 196:
#line 1070 "yacc.yy"
    {;}
    break;

  case 197:
#line 1071 "yacc.yy"
    {;}
    break;

  case 198:
#line 1072 "yacc.yy"
    {;}
    break;

  case 199:
#line 1073 "yacc.yy"
    {;}
    break;


/* Line 1267 of yacc.c.  */
#line 3210 "yacc.cpp"
      default: break;
    }
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
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
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
      if (yyn != YYPACT_NINF)
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

  if (yyn == YYFINAL)
    YYACCEPT;

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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
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


#line 1076 "yacc.yy"


void dcopidlParse( const char *_code )
{
    mocking_area = 0;
    mockpp_methods_area = 0;
    mockpp_constructor_area = 0;
    dcopidlInitFlex( _code );
    yyparse();
}

