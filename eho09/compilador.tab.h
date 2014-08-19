/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
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

extern YYSTYPE yylval;


