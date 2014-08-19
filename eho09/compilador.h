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
#define Init_TS 256
#define str_max 50

typedef enum simbolos { 
  simb_program, simb_var, simb_begin, simb_end, simb_label, simb_goto,
  simb_identificador, simb_numero, simb_write, simb_read, simb_func, simb_proc, 
  simb_ponto, simb_virgula, simb_ponto_e_virgula, simb_dois_pontos,
  simb_atribuicao, simb_abre_parenteses, simb_fecha_parenteses,
  simb_maior, simb_menor, simb_igual, simb_diferente,
  simb_mais, simb_menos, simb_div, simb_div2, simb_mul, simb_maior_igual, simb_menor_igual, 
  simb_not, simb_and, simb_or, simb_true, simb_false,
  simb_int, simb_real, simb_chr, simb_str, simb_bool,  
  simb_while, simb_do, simb_for, simb_to, simb_downto, simb_repeat, simb_until, simb_if, simb_then, simb_else
} simbolos;



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

typedef enum Categoria{
	Func, Proc, VS, PF, VF, RT
} Categoria;

typedef enum Tipo{
	Int, Str, Chr, Real, /*majin*/Bool, RefInt, RefStr, RefChr, RefReal, RefBool
} Tipo;

typedef struct ETdS{
	int tipo;
	int num_param; 
	Categoria categoria;
	int nivel;
	int deslocamento;
	char id[str_max];
	int params[10];
} ETdS;

typedef struct TdS{
	ETdS *elementos;
	int tam;
	ETdS *topo;
} TdS;

typedef struct PdT{
	Tipo *elementos;
	int tam;
	Tipo *topo;
}PdT;

typedef struct PdR{
	int *elementos;
	int tam;
	int *topo;
}PdR;

typedef struct EPdF{
	int ind_param;
	ETdS *func;
}EPdF;

typedef struct PdF{
	EPdF *elementos;
	int tam;
	EPdF *topo;
}PdF;

TdS *inicializa( );
ETdS *buscaTs(TdS *ts, int nivel, char *id);
PdT *inicializaPdT( );
PdR *inicializaPdR( );
PdF *inicializaPdF( );
EPdF *desempilhaPdF(PdF *pfunc);
Tipo desempilhaPdT(PdT *pt);
ETdS *desempilha(TdS *ts);
int numVar(TdS *ts, int nivel);
