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
#define TAM 256
#define TIPO(index) (index+1)*2
#define PASSAGEM(index) (index)*2+1

typedef enum simbolos { 
  simb_program, simb_var, simb_begin, simb_end, 
  simb_identificador, simb_numero,
  simb_ponto, simb_virgula, simb_ponto_e_virgula, simb_dois_pontos,
  simb_atribuicao, simb_abre_parenteses, simb_fecha_parenteses, simb_false, simb_true, 
  simb_mais, simb_menos, simb_mult, simb_div, simb_and, simb_or, simb_maior, simb_menor, simb_not, simb_igual,
  simb_write, simb_writeln, simb_read, simb_integer, simb_boolean, simb_string, simb_diferente,
  simb_while, simb_do, simb_repeat, simb_until, simb_for, simb_to, simb_downto,
  simb_if, simb_then, simb_else, simb_procedure, simb_function, simb_label, simb_goto,
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

typedef enum categorias {
    variavel_simples, parametro_formal, procedimento, funcao, rotulo, 
} categorias;

typedef enum tipos {
    inteiro, boolean, string, unkwon,
} tipos;

typedef enum passagens {
    valor, referencia, 
} passagens;

typedef struct elemenTS {
    char id[TAM_TOKEN];
    categorias categoria;
    int nivel, rotulo, parametros[21], deslocamento, busy;
    tipos tipo;
    passagens passagem;
} elemenTS;

typedef struct tabela {
    int topo, tam;
    elemenTS *elements;
} tabela;

typedef struct pilhaTipo {
    int topo, tam;
    tipos *elements;
} pilhaTipo;

typedef struct pilhaRotulo {
    int topo, tam;
    int *elements;
} pilhaRotulo;

typedef struct pilhaAux {
    int topo, tam;
    elemenTS **elements;
    int *num;
} pilhaAux;

void iniciaTS(tabela *, int);

int insereVSTS(tabela *, char *, categorias, int, int);
void tipoTS(tabela *, tipos, int);
elemenTS *buscaTS(tabela *, char *);
int insereVLTS(tabela *, char *, categorias, int, int);
elemenTS *buscaVLTS(tabela *, char *);

int inserePARAMTS(tabela *, char *, categorias, int, passagens);
void deslocamentoPARAMTS(tabela *);
elemenTS *buscaPARAMTS(tabela *, char *);

int insereROTTS(tabela *, char *, categorias, int, int);
int qtdeVL(tabela *);

int inserePROCTS(tabela *, char *, categorias, int, int);
elemenTS *buscaPROCTS(tabela *, char *);
int buscaFUNCTS(tabela *, char *, int);

void liberaFOR(tabela *, int);

int removeTS(tabela *, int);

void iniciaTipo(pilhaTipo *, int);
void empilhaTipo(pilhaTipo *, tipos);
tipos desempilhaTipo(pilhaTipo *);

void iniciaRotulo(pilhaRotulo *, int);
void empilhaRotulo(pilhaRotulo *, int);
int desempilhaRotulo(pilhaRotulo *);

void iniciaAux(pilhaAux *, int);
void empilhaAux(pilhaAux *, elemenTS *);
elemenTS *desempilhaAux(pilhaAux *);
elemenTS *buscaAux(pilhaAux *);

void protocoloZero(tabela *, pilhaTipo *, pilhaRotulo *, pilhaAux *);
