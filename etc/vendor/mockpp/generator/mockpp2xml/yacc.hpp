/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 70 "yacc.yy"
{
  long                 _int;
  MString             *_str;
  unsigned short       _char;
  double               _float;
}
/* Line 1489 of yacc.c.  */
#line 202 "yacc.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

