#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/* Constantes */
#define ERROR_FILE_MSG	"Nao foi possivel abrir o arquivo!\n"
#define INVALID_OPTION	"Opcao invalida! Tente novamente!"

/* Definicoes de tipo */
enum boolean {
	FALSO=0, VERDADEIRO=1
};

/* Prototipos */
FILE* carregue(char quadro[9][9]);
FILE* carregue_continue_jogo (char quadro[9][9], char *nome_arquivo);
void carregue_novo_jogo(char quadro[9][9], char *nome_arquivo);
FILE* crie_arquivo_binario(char quadro[9][9]);
int determine_quadrante(int x, int y);
int eh_valido(const char quadro[9][9], int x, int y, int valor);
int eh_valido_na_coluna(const char quadro[9][9], int y, int valor);
int eh_valido_no_quadrante3x3(const char quadro[9][9], int x, int y, int valor);
int eh_valido_na_linha(const char quadro[9][9], int x, int valor);
int existe_posicao_vazia(const char quadro[9][9]);
void imprima(const char quadro[9][9]);
void jogue();
void resolve_completo(FILE*, char quadro[9][9]);
void resolve_um_passo(char quadro[9][9]);
void salve_jogada_bin(FILE* fb, char quadro[9][9]);

/* Funcoes auxiliares */
int fim_x(int quadr);
int fim_y(int quadr);
int leia_opcao();
void gen_random(char *s, const int len);
int ini_x(int quadr);
int ini_y(int quadr);
void menu();
void menu_arquivo();


/* -----------------------------------------------------------------------------
 * -----------------------------------------------------------------------------
 * MAIN
 * /////////////////////////////////////////////////////////////////////////////
 */
int main() {
	// inicia o jogo
	jogue();

	return 0;
}

/* -----------------------------------------------------------------------------
 * CARREGAR
 * Inicializa o SUDOKU a partir de um novo jogo ou estado de jogo anterior
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
FILE* carregue(char quadro[9][9]) {
	int opcao;

	menu_arquivo();
	opcao = leia_opcao();

	// TODO Função incompleta

	switch(opcao) {

		// carregar novo sudoku
		case 1:
			char nomeArquivo[30];
			printf("Informe o nome do arquivo a ser lido: ");
			scanf("%s", nomeArquivo);
			carregue_novo_jogo(quadro, nomeArquivo);
			crie_arquivo_binario(quadro);
			break;

		// continuar jogo
		case 2:
			char nome_Arquivo[30];
			printf("Informe o nome do arquivo a ser lido: ");
			scanf("%s", nome_Arquivo);
			carregue_continue_jogo(quadro, nome_Arquivo);
			break;

		// retornar ao menu anterior
		case 9:
			break;

		default:
			printf(INVALID_OPTION);
			return NULL;
			break;
	return NULL;
}
}
/* -----------------------------------------------------------------------------
 * CARREGAR CONTINUAR JOGO
 * Carrega um estado de jogo a partir de um arquivo binario
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
FILE* carregue_continue_jogo(char quadro[9][9], char *nome_arquivo) {
    strcat(nome_arquivo, ".bin");
	FILE *arquivo = fopen(nome_arquivo, "rb+");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        return NULL;
    }

    int valor;
    fread(&valor, sizeof(int), 1, arquivo);

	fseek(arquivo, ((valor-1)*81)+4, SEEK_SET);

    fread(quadro, sizeof(char), 81, arquivo);

	fclose(arquivo);
    return arquivo;
}

/* -----------------------------------------------------------------------------
 * CARREGAR NOVO JOGO
 * Carrega um novo jogo do Sudoku
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void carregue_novo_jogo(char quadro[9][9], char *nome_arquivo) {
	strcat(nome_arquivo, ".txt");

	FILE *arquivo = fopen(nome_arquivo, "r");

		for(int i = 0; i < 9; i++){
			for(int j = 0; j < 9; j++){;
				fscanf(arquivo, "%d", &quadro[i][j]);
			}
		}
	fclose(arquivo);
}

/* -----------------------------------------------------------------------------
 * CRIAR ARQUIVO BINARIO
 * Criar arquivo binario
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
FILE* crie_arquivo_binario(char quadro[9][9]) {
	int nameLength = 15;
	char fileName[nameLength + 1];
	
	gen_random(fileName, nameLength);
	strcat(fileName, ".bin");

	FILE *arquivo = fopen(fileName, "w");

	int value = 0;

	fwrite(&value, sizeof(value), 1, arquivo);
	fwrite(quadro, sizeof(char), 81, arquivo);

	return arquivo;
}

/* -----------------------------------------------------------------------------
 * DETERMINAR QUADRANTE
 * Dado as posicoes x e y, determina o quadrante do quadro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int determine_quadrante(int x, int y) {
	if (x < 3 && y < 3)
		return 1;
	else if (x < 3 && y < 6)
		return 2;
	else if (x < 3 && y < 9)
		return 3;
	else if (x < 6 && y < 3)
		return 4;
	else if (x < 6 && y < 6)
		return 5;
	else if (x < 6 && y < 9)
		return 6;
	else if (x < 9 && y < 3)
		return 7;
	else if (x < 9 && y < 6)
		return 8;
	else
		return 9;
}

/* -----------------------------------------------------------------------------
 * E VALIDO
 * Determina se um valor na posicao x e y eh valido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int eh_valido(const char quadro[9][9], int x, int y, int valor) {

	// verifica as tres condicoes
	if (!eh_valido_na_coluna(quadro, y, valor))
		return FALSO;
	if (!eh_valido_na_linha(quadro, x, valor))
		return FALSO;
	if (!eh_valido_no_quadrante3x3(quadro, x, y, valor))
		return FALSO;

	return VERDADEIRO;
}

/* -----------------------------------------------------------------------------
 * E VALIDO NA COLUNA
 * Verifica se um valor na coluna y e valido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int eh_valido_na_coluna(const char quadro[9][9], int y, int valor) {
	for(int i = 0; i < 9; i++){
		if(quadro[i][y] == valor){
			return 0;
		}
	}
	return 1;
	// TODO
}

/* -----------------------------------------------------------------------------
 * E VALIDO NA LINHA
 * Verifica se um valor na linha x e valido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int eh_valido_na_linha(const char quadro[9][9], int x, int valor) {
	for(int i = 0; i < 9; i++){
		if(quadro[x][i] == valor){
			return 0;
		}
	}
	return 1;
	// TODO
}

/* -----------------------------------------------------------------------------
 * E VALIDO NO QUADRO 3X3
 * Verifica se um valor e valido no quadrante da posicao x, y
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int eh_valido_no_quadrante3x3(const char quadro[9][9], int x, int y, int valor) {
	int quadranteAtual = determine_quadrante(x, y);
	switch(quadranteAtual){
	case 1:
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 3; j++){
				if(quadro[i][j] == valor){
					return 0;
				}
			}
		}
		return 1;
		break;
	case 2:
		for(int i = 0; i < 3; i++){
			for(int j = 3; j < 6; j++){
				if(quadro[i][j] == valor){
					return 0;
				}
			}
		}
		return 1;
		break;
	case 3:
		for(int i = 0; i < 3; i++){
			for(int j = 6; j < 9; j++){
				if(quadro[i][j] == valor){
					return 0;
				}
			}
		}
		return 1;
		break;
	case 4:
		for(int i = 3; i < 6; i++){
			for(int j = 0; j < 3; j++){
				if(quadro[i][j] == valor){
					return 0;
				}
			}
		}
		return 1;
		break;
	case 5:
		for(int i = 3; i < 6; i++){
			for(int j = 3; j < 6; j++){
				if(quadro[i][j] == valor){
					return 0;
				}
			}
		}
		return 1;
		break;
	case 6:
		for(int i = 3; i < 6; i++){
			for(int j = 6; j < 9; j++){
				if(quadro[i][j] == valor){
					return 0;
				}
			}
		}
		return 1;
		break;
	case 7:
		for(int i = 6; i < 9; i++){
			for(int j = 0; j < 3; j++){
				if(quadro[i][j] == valor){
					return 0;
				}
			}
		}
		return 1;
		break;
	case 8:
		for(int i = 6; i < 9; i++){
			for(int j = 3; j < 6; j++){
				if(quadro[i][j] == valor){
					return 0;
				}
			}
		}
		return 1;
		break;
	case 9:
		for(int i = 6; i < 9; i++){
			for(int j = 6; j < 9; j++){
				if(quadro[i][j] == valor){
					return 0;
				}
			}
		}
		return 1;
		break;
	default:
		return 0;
		break;
	}
}

/* -----------------------------------------------------------------------------
 * EXISTE CAMPO VAZIO
 * Verifica se existe um campo nao preenchido
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int existe_posicao_vazia(const char quadro[9][9]) {
	int i, j;

	for(i = 0; i < 9; i++) {
		for(j = 0; j < 9; j++) {
			if (quadro[i][j] == 0)
				return VERDADEIRO;
		}
	}

	return FALSO;
}

/* -----------------------------------------------------------------------------
 * IMPRIMIR
 * Imprime o quadro recebido do sudoku
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void imprima(const char quadro[9][9]) {
	int i, j;

//	puts("~~~~~~~~ SUDOKU ~~~~~~~~");
	puts("    0 1 2   3 4 5   6 7 8");
	for (i = 0; i < 9; i++) {
		if (i % 3 == 0)
			puts("  -------------------------");
		for (j = 0; j < 9; j++) {
			if (j == 0)
				printf("%d | ", i);
			else if (j % 3 == 0)
				printf("| ");

			if (int(quadro[i][j]) == 0)
				printf("- ");
			else
				printf("%d ", quadro[i][j]);
		}
		puts("|");
	}
	puts("  -------------------------");
}

/* -----------------------------------------------------------------------------
 * JOGAR
 * Realiza toda a logica do jogo
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void jogue() {
	int opcao;
	char quadro[9][9] = { {0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
	FILE *fb = NULL;
	int jogadas = 0;
	opcao = 0;

	while (opcao != 9) {
		// imprima na tela o quadro atual
		imprima(quadro);

		// apresente um menu com as opcoes
		menu();
		opcao = leia_opcao();

		switch (opcao) {

		// carregue sudoku
		case 1:
			fb = carregue(quadro);

			if (fb == NULL) {
				fb = crie_arquivo_binario(quadro);
			}
			break;

		// preencha quadro com um valor
		case 2: {
			int x, y, valor;

			printf("Entre com a posicao e o valor (linha, coluna, valor): ");
			scanf("%d %d %d", &x, &y, &valor);


			if (eh_valido(quadro, x, y, valor)) {
				quadro[x][y] = valor;
				jogadas++;
				salve_jogada_bin(fb, quadro);
			}
			else
				puts("Valor ou posicao incorreta! Tente novamente!");
		}
			break;

		// resolva 1 passo
		case 3:
			resolve_um_passo(quadro);
			salve_jogada_bin(fb, quadro);
			puts("Um passo resolvido!");
			break;

		// resolva o sudoku completo
		case 4:
			resolve_completo(fb, quadro);
			break;

		case 9:
			puts("Programa finalizado ..");
			break;

		default:
			puts(INVALID_OPTION);
			break;
		}
	}
}

/* -----------------------------------------------------------------------------
 * RESOLVER
 * Resolve o sudoku
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void resolve_completo(FILE *fb, char quadro[9][9]) {
	while(existe_posicao_vazia(quadro)) {
		resolve_um_passo(quadro);
		salve_jogada_bin(fb, quadro);
	}
}

/* -----------------------------------------------------------------------------
 * RESOLVER UM PASSO
 * Preenche apenas um campo vazio
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void resolve_um_passo(char quadro[9][9]) {
	int quadro_aux[9][9] = {0};
	int i,j,k;
	if(existe_posicao_vazia(quadro)){
		for(i = 0; i < 9; i++){
			for(j = 0; j < 9; j++){
				if(((int)quadro[i][j]) == 0){
					for(k = 1; k <= 9; k++){
						if(eh_valido(quadro, i, j, k)){
							int aumenta = 0;
							aumenta = quadro_aux[i][j];
							aumenta += 1;
							quadro_aux[i][j] = aumenta;
						}
					}
				}
			}
		}
	}
	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){
			if(quadro_aux[i][j] == 1){
				for(k = 1; k <= 9; k++){
					if(eh_valido(quadro, i, j, k)){
						quadro[i][j] = k;
						return;
					}
				}
			}
		}
	}
}

/* -----------------------------------------------------------------------------
 * SALVAR JOGADA BINARIO
 * Salva o estado atual do quadro no arquivo binario
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void salve_jogada_bin(FILE *fb, char quadro[9][9]) {
    if (fb == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

	int valor;

	fread(&valor, sizeof(int), 1, fb);

	valor += 1;
	fseek(fb, 0, SEEK_SET);
	fwrite(&valor, sizeof(int), 1, fb);

	fseek(fb, (((valor - 1) * 81) + sizeof(int)), SEEK_SET);

    fwrite(quadro, sizeof(char), 81, fb);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 							FUNCOES AUXILIARES
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/* -----------------------------------------------------------------------------
 * FIM X
 * Indice final da linha para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int fim_x(int quadr) {
	switch(quadr) {
		case 1:
		case 2:
		case 3:
			return 2;

		case 4:
		case 5:
		case 6:
			return 5;

		default:
			return 8;
	}
}

/* -----------------------------------------------------------------------------
 * FIM Y
 * Indice final da coluna para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int fim_y(int quadr) {
	switch(quadr) {
		case 1:
		case 4:
		case 7:
			return 2;

		case 2:
		case 5:
		case 8:
			return 5;

		default:
			return 8;
	}
}

/* -----------------------------------------------------------------------------
 * GEN_RANDOM
 * Gera uma cadeia de caracteres randomica de tamanho len
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void gen_random(char *s, const int len) {
	srand(time(NULL));
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
	int i;

    for (i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

/* -----------------------------------------------------------------------------
 * INI X
 * Indice inicial da linha para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int ini_x(int quadr) {
	switch(quadr) {
		case 1:
		case 2:
		case 3:
			return 0;

		case 4:
		case 5:
		case 6:
			return 3;

		default:
			return 6;
	}
}

/* -----------------------------------------------------------------------------
 * INI Y
 * Indice inicial da coluna para o quadrante recebido como parametro
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int ini_y(int quadr) {
	switch(quadr) {
		case 1:
		case 4:
		case 7:
			return 0;

		case 2:
		case 5:
		case 8:
			return 3;

		default:
			return 6;
	}
}

/* -----------------------------------------------------------------------------
 * LE OPCAO
 * Imprime a mensagem a faz a leitura da opcao
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
int leia_opcao () {
	int opcao;

	printf("Opcao: ");
	scanf("%d", &opcao);

	return opcao;
}

/* -----------------------------------------------------------------------------
 * MENU
 * Imprime o menu de opcoes
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void menu() {
	puts("\n~~~~~~~~ SUDOKU ~~~~~~~~");
	puts("[1] - Carregar jogo");
	puts("[2] - Jogar");
	puts("[3] - Resolver um passo");
	puts("[4] - Resolver");
	puts("[9] - Finalizar");
	puts("--------");
}

/* -----------------------------------------------------------------------------
 * MENU ARQUIVO
 * Imprime o menu de opcoes do arquivo
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void menu_arquivo() {
	puts("\n~~~~~ MENU ARQUIVO ~~~~~");
	puts("[1] - Novo jogo");
	puts("[2] - Continuar jogo");
	puts("[9] - Retornar ao menu anterior");
	puts("--------");
}
