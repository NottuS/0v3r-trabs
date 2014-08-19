/* -------------------------------------------------------------------
 *            Arquivo: compilaodr.h
 * -------------------------------------------------------------------
 *              Autor: Bruno Muller Junior
 *               Data: 08/2007
 *      Atualizado em: [15/03/2012, 08h:22m]
 *
 * -------------------------------------------------------------------
 *
 * Tipos, protótipos e vaiáveis globais do compilador
 *
 * ------------------------------------------------------------------- */

#define TAM_TOKEN 16


typedef enum simbolos { 
  simb_program, simb_var, simb_begin, simb_end,
  simb_proc, simb_func,
  simb_while, simb_do,
  simb_repeat, simb_until,
  simb_for, simb_to, simb_downto,
  simb_if, simb_then, simb_else,
  simb_identificador, simb_numero,
  simb_ponto, simb_virgula, simb_ponto_e_virgula, simb_dois_pontos,
  simb_atribuicao, simb_abre_parenteses, simb_fecha_parenteses,
  simb_soma, simb_subtrai, simb_divide, simb_multiplica,
  simb_maior, simb_maior_igual, simb_menor, simb_menor_igual, 
  simb_ou, simb_e, simb_igual, simb_diferente,
  simb_true, simb_false,
  simb_write, simb_read,
  simb_goto, simb_label,
} simbolos;

/* Pilha da Tabela de Símbolos */
typedef enum tipos{
	Inteiro, String, Real, Boolean
} tipos;

/*typedef struct varSimples{
	tipos tipo;
	int deslocamento;
} varSimples;*/

typedef enum categorias {
	variavelSimples, parametroFormal, procedimento, programaPrincipal, rotulo, funcao,
} categorias;

typedef enum tipoPassagem {
	valor, referencia, 
} tipoPassagem;


typedef struct vParamTR {
	tipos tipo;
	tipoPassagem pfTipoPassagem;
} vParamTR;

typedef struct elementoTS {
	char id[256];
	categorias cat;
	int nivel;
	
	/* Procedimentos */
	int rotulo;
	int nparam;
	
	/* No Procedimento: Parametros formais */
	vParamTR vParam[10];/* Tipo/Passagem */
	
	/* Na variável de pf da TS */
	tipoPassagem pfTipoPassagem;
	
	tipos tipo; // VS e PF
	int deslocamento; // VS e  PF
	/*union {
		varSimples var;
	};*/
} elementoTS;

typedef struct tabS {
	int total;
	int tam;
	elementoTS *eTS;
} tabS;

/* Pilha de Rótulos */
typedef struct p_rot {
	int total;
	int tam;
	int *r;
} p_rot;

/* Pilha de Rótulos */
typedef struct p_tipo {
	tipos *tipo;
	int tam; 
	int total;

} p_tipo;

typedef struct p_proc_func {
	int tam;
	int total;
	elementoTS *eTS;
} p_proc_func;


/* -------------------------------------------------------------------
 * variáveis globais
 * ------------------------------------------------------------------- */

extern simbolos simbolo, relacao;
extern char token[TAM_TOKEN];
extern int nivel_lexico;
extern int desloc;
extern int nl;


simbolos simbolo, relacao;
char token[TAM_TOKEN];



