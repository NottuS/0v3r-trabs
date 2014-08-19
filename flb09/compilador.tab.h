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
     PROCEDURE = 261,
     FUNCTION = 262,
     VIRGULA = 263,
     PONTO_E_VIRGULA = 264,
     DOIS_PONTOS = 265,
     PONTO = 266,
     T_BEGIN = 267,
     T_END = 268,
     VAR = 269,
     IDENT = 270,
     ATRIBUICAO = 271,
     SOMA = 272,
     SUBTRAI = 273,
     DIVIDE = 274,
     MULTIPLICA = 275,
     MENOR = 276,
     MENOR_IGUAL = 277,
     MAIOR = 278,
     MAIOR_IGUAL = 279,
     IGUAL = 280,
     DIFERENTE = 281,
     OU = 282,
     E = 283,
     WHILE = 284,
     DO = 285,
     REPEAT = 286,
     UNTIL = 287,
     FOR = 288,
     TO = 289,
     DOWNTO = 290,
     IF = 291,
     THEN = 292,
     ELSE = 293,
     NUMERO = 294,
     TRUE = 295,
     FALSE = 296,
     WRITE = 297,
     READ = 298,
     GOTO = 299,
     LABEL = 300,
     LOWER_THAN_ELSE = 301
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


