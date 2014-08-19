
// Testar se funciona corretamente o empilhamento de parâmetros
// passados por valor ou por referência.


%{
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

%}

%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES GOTO
%token VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO
%token T_BEGIN T_END VAR IDENT ATRIBUICAO FALSE TRUE
%token MAIS MENOS MULT DIV NOT AND OR MAIOR MENOR IGUAL DIFERENTE
%token NUMERO READ WRITE WRITELN INTEGER BOOLEAN STRING
%token WHILE DO REPEAT UNTIL FOR TO DOWNTO IF THEN ELSE PROCEDURE FUNCTION LABEL

%nonassoc LOWER_THAN_ELSE /* dummy token */
%nonassoc ELSE

%%

programa    : PROGRAM IDENT
              {
                geraCodigo (NULL, "INPP");
                iniciaTS(&ts, 8);
                iniciaTipo(&pt, 8);
                iniciaRotulo(&pr, 8);
                iniciaAux(&pa, 8);
                inserePROCTS(&ts, token, procedimento, nivel_lexico, numRotulo);
                numRotulo++;
              }
              ABRE_PARENTESES lista_idents
              {
                tipoTS(&ts, unkwon, num_vars);
                deslocamentoPARAMTS(&ts);
                num_vars = 0;
              }
              FECHA_PARENTESES PONTO_E_VIRGULA bloco PONTO 
              {
                geraCodigo (NULL, "PARA");
                protocoloZero(&ts, &pt, &pr, &pa);
              }
;

lista_idents: lista_idents VIRGULA ident
            | ident
;

ident   : IDENT { inserePARAMTS(&ts, token, parametro_formal, nivel_lexico, referencia); num_vars++; }
;

bloco       : parte_declara_labels
              parte_declara_vars
              {
                if (num_vars) {
                    sprintf(str, "AMEM %d", num_vars);
                    geraCodigo (NULL, str);
                }
              }
              parte_declara_procs
              T_BEGIN
              comandos
              T_END
              {
                aux = removeTS(&ts, nivel_lexico);
                if (aux) {
                    sprintf(str, "DMEM %d", aux);
                    geraCodigo (NULL, str);
                }
              }
;

parte_declara_labels : parte_declara_label
                     |
;

parte_declara_label : parte_declara_label label
                    | label
;

label   : LABEL lista_id_labels PONTO_E_VIRGULA
;

lista_id_labels  : lista_id_labels VIRGULA id_label
                 | id_label
;

id_label    : IDENT insere_label
            | NUMERO insere_label
;

insere_label    : {
                    if(insereROTTS(&ts, token, rotulo, nivel_lexico, numRotulo)) {
                        sprintf(str, "redeclaration of '%s'", token);
                        imprimeErro(str);
                    }
                    numRotulo++;
                  }
;

parte_declara_vars  : parte_declara_var
                    |
;

parte_declara_var   : parte_declara_var var
                    | var
;

var         : VAR declara_vars
;

declara_vars: declara_vars declara_var PONTO_E_VIRGULA
            | declara_var PONTO_E_VIRGULA
;

declara_var : { aux = num_vars; }
              lista_id_var DOIS_PONTOS tipo
;

lista_id_var: lista_id_var VIRGULA id_var
            | id_var
;

id_var  : IDENT
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
;

tipo        : INTEGER { tipoTS(&ts, inteiro, num_vars-aux); }
            | BOOLEAN { tipoTS(&ts, boolean, num_vars-aux); }
            | STRING { tipoTS(&ts, string, num_vars-aux); }
;

parte_declara_procs : {
                        sprintf(str, "DSVS R%d", numRotulo);
                        if(numRotulo < 10)
                        	sprintf(str, "DSVS R0%d", numRotulo);
                        empilhaRotulo(&pr, numRotulo);
                        geraCodigo(NULL, str);
                        numRotulo++;
                        varlocal = 1;
                      }
                      declara_procs
                      {
                        varlocal = 0;
                        num_vars = 0;
                        r = desempilhaRotulo(&pr);
                        sprintf(fim, "R%d", r);
                        if(r < 10)
                        	sprintf(fim, "R0%d", r);
                        geraCodigo(fim, "NADA");
                      }
                    |
;

declara_procs   : declara_procs declara_proc
                | declara_proc
;

declara_proc    : PROCEDURE
                  { aux = 0; }
                  assinatura PONTO_E_VIRGULA bloco
                  {
                    element = buscaPROCTS(&ts, "");
                    sprintf(str, "RTPR %d, %d", nivel_lexico, element->parametros[0]);
                    geraCodigo(NULL, str);
                    nivel_lexico--;
                  }
                  PONTO_E_VIRGULA
                | FUNCTION
                  { aux = 1; proc++; }
                  assinatura DOIS_PONTOS tipo_retorno PONTO_E_VIRGULA bloco
                  {
                    element = buscaPROCTS(&ts, "");
                    sprintf(str, "RTPR %d, %d", nivel_lexico, element->parametros[0]);
                    geraCodigo(NULL, str);
                    nivel_lexico--;
                    proc--;
                  }
                  PONTO_E_VIRGULA
;

assinatura  : IDENT
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
              declara_params
;

declara_params  : ABRE_PARENTESES qtde_params FECHA_PARENTESES
                | 
;

qtde_params : lista_params
              { deslocamentoPARAMTS(&ts); }
            |
;

lista_params    : lista_params PONTO_E_VIRGULA passagem_params
                | passagem_params
;

passagem_params : VAR
                  { ref = 1; }
                  declara_param
                  { ref = 0; }
                | declara_param
;

declara_param   : { aux = element->parametros[0]; }
                  lista_id_param DOIS_PONTOS tipo_param
;

lista_id_param  : lista_id_param VIRGULA param
                | param
;

param   : IDENT
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
;

tipo_param  : INTEGER
              {
                tipoTS(&ts, inteiro, element->parametros[0]-aux);
                for (;aux < element->parametros[0]; aux++)
                    element->parametros[TIPO(aux)] = inteiro;
              }
            | BOOLEAN
              {
                tipoTS(&ts, boolean, element->parametros[0]-aux);
                for (;aux < element->parametros[0]; aux++)
                    element->parametros[TIPO(aux)] = boolean;
              }
            | STRING
              {
                tipoTS(&ts, string, element->parametros[0]-aux);
                for (;aux < element->parametros[0]; aux++)
                    element->parametros[TIPO(aux)] = string;
              }
;

tipo_retorno  : INTEGER
                { element->tipo = inteiro; }
              | BOOLEAN
                { element->tipo = boolean; }
              | STRING
                { element->tipo = string; }
;

comando_composto    : T_BEGIN comandos T_END
                    | comando
                      
;

comandos    : comandos PONTO_E_VIRGULA comando 
            | comando 
;

comando : goto
        | rot
        | atrib_proc_rot
        | write
        | read
        | repetitivo
        | if
        |
;

goto    : GOTO rotulo
;

rotulo  : IDENT simbora
        | NUMERO simbora
;

simbora : {
            element = buscaTS(&ts, token);
            if (element->categoria != rotulo)
                imprimeErro("Illegal expression");
            sprintf(str, "DSVR R%d, %d, %d", element->rotulo, element->nivel, nivel_lexico);
            if(element->rotulo < 10)
            	sprintf(str, "DSVR R0%d, %d, %d", element->rotulo, element->nivel, nivel_lexico);
            geraCodigo(NULL, str);
          }
;

rot : NUMERO 
      { strcpy(ident, token); }
      goto_dest
;

atrib_proc_rot  : IDENT
                  {
                    strcpy(ident, token);
                    element = buscaTS(&ts, token);
                    if (element == NULL) {
                        sprintf(str, "'%s' undeclared (first use in this function)", token);
                        imprimeErro(str);
                    }
                    empilhaAux(&pa, element);
                  }
                  proc_ou_atrib
;

proc_ou_atrib   : ATRIBUICAO expressao
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
                | goto_dest
                | ABRE_PARENTESES
                  {
                    if ((element->categoria != procedimento) && (element->categoria != funcao))
                        imprimeErro("Illegal expression");
                  }
                  inicia_params lista_parametros finaliza_params FECHA_PARENTESES
                | {
                    if ((element->categoria != procedimento) && (element->categoria != funcao))
                        imprimeErro("Illegal expression");
                  }
                  inicia_params finaliza_params
;

goto_dest   : DOIS_PONTOS
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
              comando
;

lista_parametros    : parametros
                    | 
;

parametros : parametros VIRGULA parametro
           | parametro
;

parametro   : expressao
              {
                 if (desempilhaTipo(&pt) != p->parametros[TIPO(num_vars)])  {
                     sprintf(str, "Incompatible types %d", p->parametros[TIPO(num_vars)]);
                     imprimeErro(str);
                 }
                 num_vars++;
              }
;

inicia_params   : {
                    p = buscaAux(&pa);
                    pa.num[pa.topo] = num_vars;
                    num_vars = 0;
                    ref++;
                    if (p->categoria == funcao)
                        geraCodigo(NULL, "AMEM 1");
                  }
;

finaliza_params : {
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
;

expressao   : NOT logica
              {
                if (desempilhaTipo(&pt) != boolean)
                    imprimeErro("Incompatible types");
                empilhaTipo(&pt, boolean);
                geraCodigo(NULL, "NEGA");
              }
            | expressao AND logica
              {
                if ((desempilhaTipo(&pt) != boolean) || (desempilhaTipo(&pt) != boolean))
                    imprimeErro("Incompatible types");
                empilhaTipo(&pt, boolean);
                geraCodigo(NULL, "CONJ");
              }
            | expressao OR logica
              {
                if ((desempilhaTipo(&pt) != boolean) || (desempilhaTipo(&pt) != boolean))
                    imprimeErro("Incompatible types");
                empilhaTipo(&pt, boolean);
                geraCodigo(NULL, "DISJ");
              }
            | logica
;

logica  : logica MAIOR aritmetica
          {
            if ((desempilhaTipo(&pt) != inteiro) || (desempilhaTipo(&pt) != inteiro))
                imprimeErro("Incompatible types");
            empilhaTipo(&pt, boolean);
            geraCodigo(NULL, "CMMA");
          }
        | logica MENOR aritmetica
          {
            if ((desempilhaTipo(&pt) != inteiro) || (desempilhaTipo(&pt) != inteiro))
                imprimeErro("Incompatible types");
            empilhaTipo(&pt, boolean);
            geraCodigo(NULL, "CMME");
          }
        | logica IGUAL aritmetica
          {
          if (desempilhaTipo(&pt) !=  desempilhaTipo(&pt))
            imprimeErro("Incompatible types");
          empilhaTipo(&pt, boolean);
          geraCodigo(NULL, "CMIG");
          }
        | logica MENOR MAIOR aritmetica
          {
            if (desempilhaTipo(&pt) !=  desempilhaTipo(&pt))
              imprimeErro("Incompatible types");
             empilhaTipo(&pt, boolean);
            geraCodigo(NULL, "CMDG");
          }
        | logica DIFERENTE aritmetica
          {
              if (desempilhaTipo(&pt) !=  desempilhaTipo(&pt))
                imprimeErro("Incompatible types");
              empilhaTipo(&pt, boolean);
              geraCodigo(NULL, "CMDG");
          }
        | logica MAIOR IGUAL aritmetica
          {
              if ((desempilhaTipo(&pt) != inteiro) || (desempilhaTipo(&pt) != inteiro))
                imprimeErro("Incompatible types");
              empilhaTipo(&pt, boolean);
              geraCodigo(NULL, "CMAG");
          }
        | logica MENOR IGUAL aritmetica
          {
              if ((desempilhaTipo(&pt) != inteiro) || (desempilhaTipo(&pt) != inteiro))
                imprimeErro("Incompatible types");
              empilhaTipo(&pt, boolean);
              geraCodigo(NULL, "CMEG");
          }
        | aritmetica
;

aritmetica  : aritmetica MAIS termo
              {
                  if ((desempilhaTipo(&pt) != inteiro) || (desempilhaTipo(&pt) != inteiro))
                    imprimeErro("Incompatible types");
                  empilhaTipo(&pt, inteiro);
                  geraCodigo(NULL, "SOMA");
              }
            | aritmetica MENOS termo
              {
                  if ((desempilhaTipo(&pt) != inteiro) || (desempilhaTipo(&pt) != inteiro))
                    imprimeErro("Incompatible types");
                  empilhaTipo(&pt, inteiro);
                  geraCodigo(NULL, "SUBT");
              }
            | termo
;

termo   : termo MULT fator
          {
              if ((desempilhaTipo(&pt) != inteiro) || (desempilhaTipo(&pt) != inteiro))
                imprimeErro("Incompatible types");
              empilhaTipo(&pt, inteiro);
              geraCodigo(NULL, "MULT");
          }
        | termo DIV fator
          {
              if ((desempilhaTipo(&pt) != inteiro) || (desempilhaTipo(&pt) != inteiro))
                imprimeErro("Incompatible types");
              empilhaTipo(&pt, inteiro);
              geraCodigo(NULL, "DIVI");
          }
        | fator
;

fator   : IDENT
          {
            element = buscaTS(&ts, token);
            if (element == NULL) {
                sprintf(str, "'%s' undeclared (first use in this function)", token);
                imprimeErro(str);
            }
          }
          funcao_ou_nao
        | NUMERO
          {
              if (ref && p->parametros[PASSAGEM(num_vars)])
                imprimeErro("Variable identifier expected");
              empilhaTipo(&pt, inteiro);
              sprintf(str, "CRCT %s", token);
              geraCodigo(NULL, str);
          }
        | TRUE
          {
              if (ref && p->parametros[PASSAGEM(num_vars)])
                imprimeErro("Variable identifier expected");
              empilhaTipo(&pt, boolean);
              geraCodigo(NULL, "CRCT 1");
          }
        | FALSE
          {
              if (ref && p->parametros[PASSAGEM(num_vars)])
                imprimeErro("Variable identifier expected");
              empilhaTipo(&pt, boolean);
              geraCodigo(NULL, "CRCT 0");
          }
        | ABRE_PARENTESES expressao FECHA_PARENTESES
;

funcao_ou_nao   : ABRE_PARENTESES
                  {
                    empilhaAux(&pa, element);
                    empilhaTipo(&pt, element->tipo);
                    if ((element->categoria != procedimento) && (element->categoria != funcao))
                        imprimeErro("Illegal expression");
                  }
                  inicia_params lista_parametros finaliza_params FECHA_PARENTESES
                | {
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
;

write   : WRITE ABRE_PARENTESES escrita FECHA_PARENTESES
        | WRITELN ABRE_PARENTESES escrita FECHA_PARENTESES
;

escrita : escrita VIRGULA impr
        | impr
;

impr    : expressao
          {
              desempilhaTipo(&pt);
              geraCodigo(NULL, "IMPR");
          }
;

read    : READ ABRE_PARENTESES leitura FECHA_PARENTESES
;

leitura : leitura VIRGULA leit
        | leit
;

leit    : IDENT 
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
;

repetitivo  : while
            | repeat
            | for
;

while   : WHILE
          {
              sprintf(inicio, "R%d", numRotulo);
              if(numRotulo < 10)
              	sprintf(inicio, "R0%d", numRotulo);
              empilhaRotulo(&pr, numRotulo);
              numRotulo++;
              geraCodigo(inicio, "NADA");
          }
          expressao
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
          DO comando_composto
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
;

repeat   : REPEAT
           {
               sprintf(inicio, "R%d", numRotulo);
               if(numRotulo < 10)
               	sprintf(inicio, "R0%d", numRotulo);
               empilhaRotulo(&pr, numRotulo);
               numRotulo++;
               geraCodigo(inicio, "NADA");
           }
           comandos UNTIL expressao
           {
               if (desempilhaTipo(&pt) != boolean)
                imprimeErro("Incompatible types");
               r = desempilhaRotulo(&pr);
               sprintf(str, "DSVF R%d", r);
               if(r < 10)
               	sprintf(str, "DSVF R0%d", r);
               geraCodigo(NULL, str);
           }
;

for   : FOR IDENT
        { strcpy(ident, token); busy++; }
        ATRIBUICAO expressao
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
        to_ou_downto DO comando_composto
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
;

to_ou_downto    : {
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
                  TO expressao
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
                | {
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
                  DOWNTO expressao
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
                  
                  
;

if  : if_then could_else
      {
      	  r = desempilhaRotulo(&pr);
          sprintf(fim, "R%d", r);
          if(r < 10)
          	sprintf(fim, "R0%d", r);
          geraCodigo(fim, "NADA");
      }
;

if_then : IF expressao
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
          THEN comando_composto
;

could_else  : ELSE
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
              comando_composto
            | %prec LOWER_THAN_ELSE
;


%%

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

