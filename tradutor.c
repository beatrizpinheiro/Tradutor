#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINESZ 256 
#define TAM_MAX 99999

int pos_pilha = 0;

// Remove o '\n' do fim da linha
void remove_newline(char* ptr)
{
	while (*ptr) {
		if (*ptr == '\n')
			*ptr = 0;
		else
			ptr++;
	}
}

void variaveis(char variavel, int numero_variavel) {

	if (variavel == 'p') {
		if (numero_variavel == 1) {
			printf("%%edi");
		}
		if (numero_variavel == 2) {
			printf("%%esi");
		}
		if (numero_variavel == 3) {
			printf("%%edx");
		}
	}
	if (variavel == 'v') {
		//variavel local
	}
	if (variavel == 'c') {
		printf("$%d", numero_variavel);
	}

}

int main()
{
	char v1;
	int r, i1, i2, i4, i3, i5;
	int inicio, fim, passo;
	char line[LINESZ];
	int count = 0;
	int f, var, vet, ci, val, ind;
	char a0, a1, a2, a3, a4, a5, a6;
	int contador_if = 0;

	// Lê uma linha por vez
	while (fgets(line, LINESZ, stdin) != NULL) {
		count++;
		remove_newline(line);

		//Verifica endif
		if (strncmp(line, "endif", 5) == 0) {
			printf("end_if%d\n", contador_if);
			continue;
		}
		//Verifica def
		if (strncmp(line, "def", 3) == 0) {
			printf("pushq %%rbp\n");
			printf("movq  %%rsp, %%rbp\n");
			continue;
		}

		//Verifica enddef
		if (strncmp(line, "enddef", 6) == 0) {
			
			continue;
		}

		// Verifica se line começa com 'end' (3 letras)
		if (strncmp(line, "end", 3) == 0) {
			printf("leave\nret\n");
			continue;
		}

		// Verifica se é 'for' e quanto elementos possui
		r = sscanf(line, "for %c = %d, %d, %d", &v1, &inicio, &fim, &passo);
		if (r == 4) {
			printf("Linha %d: %s\n", count, line);
			printf("Lido '%c' valores: %d, %d, %d\n", v1, inicio, fim, passo);
			printf("---\n");
			continue;
		}
		if (r == 3) {
			printf("Linha %d: %s\n", count, line);
			printf("Lido '%c' valores: %d, %d\n", v1, inicio, fim);
			printf("---\n");
			continue;
		}
		if (r == 2) {
			printf("Linha %d: %s\n", count, line);
			printf("Lido '%c' valor: %d\n", v1, inicio);
			printf("---\n");
			continue;
		}

		// Verifica se é um 'if'
		r = sscanf(line, "if %ci%d", &a0, &i1);
		if (r == 2) {
			//Ou ci de array ou pi de parametro ou vi de variavel
			printf("cmpl %ci%d\n", a0, i1);
			contador_if++;
			printf("je end_if%d\n", contador_if);

			continue;
		}


		// Verifica se é uma função e quantos parâmetros
		r = sscanf(line, "function f%d p%c1 p%c2 p%c3", &f, &a1, &a2, &a3);
		if (r > 0 && r < 5) {
			printf(".globl f%d\nf%d:\n", f, f);
		}
		/*if(r == 3){
		  printf("funcao com 3 elementos\n");
		}
		if(r == 2){
		  printf("funcao com 2 elementos\n");
		}
		if(r == 1){
		  printf("funcao com 1 elementos\n");
		}*/

		// Verifica se a linha esta inicializando uma variável ou um vetor  
		r = sscanf(line, "var vi%d", &var);
		if (r == 1) {
			//printf("var %d", var);
		}
		r = sscanf(line, "vet va%d size ci%d", &vet, &ci);
		if (r == 2) {
			//printf("vet");
		}
		// Verifica o que está retornando
		r = sscanf(line, "return ci%d", &vet);
		if (r == 1) {
			printf("movl  %%eax,  ");
		}

		r = sscanf(line, "return vi%d", &var);
		if (r == 1) {}

		r = sscanf(line, "return p%c%d", &a0, &ind);
		if (r == 2) {}

		// Verifica se é expressão simples
		r = sscanf(line, "%ci%d = %ci%d %c %ci%d", &a0, &val, &a1, &i1, &a2, &a3, &i2);
		// vi1 = ci1 
		if (r == 4) {
			printf("movl ");
			variaveis(a1, i1);
			printf(", ");
			variaveis(a0, val);
			printf("\n");

		}
		// vi1 = pi1 + vi2, vi2 = vi1 * ci-5, etc   pi1 = pi2 + ci2
		if (r == 7) {
			if (a2 == '/') {
				printf("movl %%edx, %%ebx\n");
				printf("movl ");
				variaveis(a1, i1);
				printf(", %%eax\n");
				printf("divl ");
				variaveis(a3, i2);
				printf("\nmovl %%eax, ");
				variaveis(a0, val);
				printf("\nmovl %%ebx, %%edx\n");

			}
			else {
				printf("movl ");
				variaveis(a1, i1);
				printf(", ");
				variaveis(a0, val);
				if (a2 == '+') printf("\naddl ");
				if (a2 == '-') printf("\nsubl ");
				if (a2 == '*') printf("\nimull ");
				variaveis(a3, i2);
				printf(", ");
				variaveis(a0, val);
				printf("\n");
			}
		}

		// Verificar se vriável recebe retorno de função
		r = sscanf(line, "%ci%d = call f%d  %c%c%d %c%c%d %c%c%d", &a0, &i1, &i2, &a1, &a2, &i3, &a3, &a4, &i4, &a5, &a6, &i5);
		if (r == 3) {
			printf("nenhum parametro\n");
		}
		if (r == 6) {
			printf("um parametro\n");
		}
		if (r == 9) {
			printf("dois parametros\n");
		}
		if (r == 12) {
			printf("tres parametros\n");
		}
		//Verifica acesso a Array
		//set with
		r = sscanf(line, "set %ca%d index ci%d with %ci%d", &a0, &i1, &i2, &a1, &i3);
		if (r == 5) {
			printf("set %ca%d index ci%d with %ci%d", a0, i1, i2, a1, i3);
		}
		//get to
		r = sscanf(line, "get %ca%d index ci%d to %ci%d", &a0, &i1, &i2, &a1, &i3);
		if (r == 5) {
			printf("get %ca%d index ci%d to %ci%d", a0, i1, i2, a1, i3);
		}

	}

	return 0;
}

