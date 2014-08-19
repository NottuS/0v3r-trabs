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

extern YYSTYPE yylval;


