#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define LINESZ 256 
#define TAM_MAX 99999

int indice = 0;
int indice2 = 0;
int tamanho_pilha=0;

struct Pilha{
	char nome[4];
	int tamanho;
	int posicao;
};

void cria_pilha(struct Pilha *pi, int x, int n, int ci);
void exibe_pilha(struct Pilha *pi, int x, int n, int ci);
void adiciona_parametro(struct Pilha *pi, char x, int n);
int pega_posicao(struct Pilha *pi, char nome1, char nome2, int nome3);
void chama_funcao(int funcao, int posicao1, int posicao2, int posicao3);

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

void arrays(char variavel, int numero_variavel, int index) {
	
	if(variavel == 'p') {
		//Array parâmetro
		printf("movabs $%d, %%rcx \n",index);	
		printf("imulq $4, %%rcx\n");
		if (numero_variavel == 1) {
			printf("addq %%rdi, %%rcx\n");
		}
		if (numero_variavel == 2) {
			printf("addq %%rsi, %%rcx\n");
		}
		if (numero_variavel == 3) {
			printf("addq %%rdx, %%rcx\n");
		}
		
	}

}

int main() {
	struct Pilha pi[TAM_MAX];
	char v1;
	int r, i1, i2, i4, i3, i5;
	int inicio, fim, passo;
	char line[LINESZ];
	int count = 0;
	int f, var, vet, ci, val, ind;
	char a0, a1, a2, a3, a4, a5, a6,a7;
	int contador_if = 0;
	int quantidade_parametros = 0;
	char parametro[3];
	
	printf(".data\n\n");
	printf(".text\n");

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
			//char x, int n
			if( quantidade_parametros > 0) adiciona_parametro(pi,parametro[0],1);
			if( quantidade_parametros > 1) adiciona_parametro(pi,parametro[1],2);
			if( quantidade_parametros > 2) adiciona_parametro(pi,parametro[2],3);
			
			printf("subq $%d, %%rsp\n", tamanho_pilha);

			if(quantidade_parametros > 0) printf("movq %%rdi, -%d(%%rbp)\n" ,pega_posicao(pi,'p',parametro[0],1));
			if(quantidade_parametros > 1) printf("movq %%rsi, -%d(%%rbp)\n" ,pega_posicao(pi,'p',parametro[1],2));
			if(quantidade_parametros > 2) printf("movq %%rdx, -%d(%%rbp)\n" ,pega_posicao(pi,'p',parametro[2],3));
			continue;
		}

		// Verifica se line começa com 'end' (3 letras)
		if (strncmp(line, "end", 3) == 0) {
			indice = 0;
			indice2 = 0;
			//Tem primeiro parametro?
			i1 = pega_posicao(pi, 'p','i','1');
			i2 =  pega_posicao(pi, 'p','a','1');
			if(i1 != 0) printf("movq -%d(%%rbp), %%rdi\n",i1);
			if(i2 != 0) printf("movq -%d(%%rbp), %%rdi\n",i2);

			//Tem segundo parametro?
			i1 = pega_posicao(pi, 'p','i','2');
			i2 =  pega_posicao(pi, 'p','a','2');
			if(i1 != 0) printf("movq -%d(%%rbp), %%rsi\n", i1);
			if(i2 != 0) printf("movq -%d(%%rbp), %%rsi\n", i2);

			//Tem tercerio parametro?
			i1 = pega_posicao(pi, 'p','i','3');
			i2 =  pega_posicao(pi, 'p','a','3');
			if(i1 != 0) printf("movq -%d(%%rbp), %%rdx\n", i1);
			if(i2 != 0) printf("movq -%d(%%rbp), %%rdx\n", i2);
			
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
		r = sscanf(line, "function f%d p%c1 p%c2 p%c3", &f, &parametro[0], &parametro[1], &parametro[2]);
		if (r > 0 && r < 5) {
			printf(".globl f%d\nf%d:\n", f, f);
			quantidade_parametros = r-1;
		}

		// Verifica se a linha esta inicializando uma variável ou um vetor  
		r = sscanf(line, "var vi%d", &var);
		if (r == 1) {
			//printf("var vi%d\n", var);
			//(struct Pilha *pi, int x, bool local, int n, int ci) 
			cria_pilha(pi, 0, var, 0);
			exibe_pilha(pi, 0, var, 0);
		}
		r = sscanf(line, "vet va%d size ci%d", &vet, &ci);
		if (r == 2) {
			//printf("vet va%d ci%d\n", vet, ci);
			cria_pilha(pi, 1, vet, ci);
			exibe_pilha(pi, 0, var, 0);
		}
		// Verifica o que está retornando
		r = sscanf(line, "return ci%d", &vet);
		if (r == 1) {
			printf("movl  %%eax,  \n");
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
			if (a1 == 'c') printf("$%d ",i1);
			else printf("-%d(%%rbp)" ,pega_posicao(pi,a1,'i',i1) );	
			printf(", ");
			printf("-%d(%%rbp)" ,pega_posicao(pi,a0,'i',val) );
			printf("\n");

		}
		// vi1 = pi1 + vi2, vi2 = vi1 * ci-5, etc   pi1 = pi2 + ci2
		if (r == 7) {
			//DIVISÃO
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
			// OUTRAS OPERAÇÕES
			else {
				printf("movl ");
				if (a1 == 'c') printf("$%d ",i1);
				else printf("-%d(%%rbp)" ,pega_posicao(pi,a1,'i',i1) );	
				printf(", ");
				printf("-%d(%%rbp)" ,pega_posicao(pi,a0,'i',val) );
				if (a2 == '+') printf("\naddl ");
				if (a2 == '-') printf("\nsubl ");
				if (a2 == '*') printf("\nimull ");
				if (a3 == 'c') printf("$%d ",i2);
				else printf("-%d(%%rbp)" ,pega_posicao(pi,a3,'i',i2) );
				printf(", ");
				printf("-%d(%%rbp)" ,pega_posicao(pi,a0,'i',val) );
				printf("\n");
			}
		}

		//Verifica acesso a Array
		//set with
		r = sscanf(line, "set %ca%d index ci%d with %ci%d", &a0, &i1, &i2, &a1, &i3);
		if (r == 5) {
			//va2[5] = 2
			arrays(a0,i1,i2);
			printf("movl ");
			variaveis(a1,i3);
			printf(", (%%rcx)");
		}
		//get to
		r = sscanf(line, "get %ca%d index ci%d to %ci%d", &a0, &i1, &i2, &a1, &i3);
		if (r == 5) {
			//vi1 = va2[8]
			arrays(a0,i1,i2);
			printf("movl (%%rcx), ");
			variaveis(a1,i3);
			
		}
		//	Chamada de função
		r = sscanf(line, "%c%c%d = call f%d %c%c%d %c%c%d %c%c%d ", &a0, &a1, &i1, &i2, &a2, &a3, &i3, &a4, &a5, &i4,&a6, &a7, &i5);
		//Manda 3 parametros
		if(r == 13){
			chama_funcao(i2, pega_posicao(pi, a2,a3,i3), pega_posicao(pi, a4,a5,i4), pega_posicao(pi, a6,a7,i5));
			printf("movq %%rax, -%d(%%rbp)\n", pega_posicao(pi, a0,a1,i1));
		}
		//Manda 2 parametros
		if(r == 10){
			chama_funcao(i2, pega_posicao(pi, a2,a3,i3), pega_posicao(pi, a4,a5,i4),0);
			printf("movq %%rax, -%d(%%rbp)\n", pega_posicao(pi, a0,a1,i1));
		}
		//Manda 1 parametro
		if(r == 7){
			chama_funcao(i2, pega_posicao(pi, a2,a3,i3),0, 0);
			printf("movq %%rax, -%d(%%rbp)\n", pega_posicao(pi, a0,a1,i1));
		}
		//Manda 0 parametros
		if(r == 4){
			chama_funcao(i2,0,0,0);
			printf("movq %%rax, -%d(%%rbp)\n", pega_posicao(pi, a0,a1,i1));
		}
	}
	
	return 0;
}

void cria_pilha(struct Pilha *pi, int x, int n, int ci) {
	/*
		x = identificador (se é vi=0 ou va=1)
		local = boolean. Se for true, é variavel local. False, é parametro 
		n = número da var/vet 
		ci = indice do vetor (se for var ci = 0)
		indice = posicao atual da pilha
	*/
	
	if(x == 0){
		sprintf(pi[indice].nome, "vi%d", n);
		pi[indice].tamanho = 4;
		if(indice == 0) {
			pi[indice].posicao = pi[indice].tamanho;
		}else{
			pi[indice].posicao = pi[indice-1].posicao + pi[indice].tamanho;
		}
		indice++;
	}
	else if(x == 1){
		sprintf(pi[indice].nome, "va%d", n);
		pi[indice].tamanho = (4 * ci);
		if(indice == 0) {
			pi[indice].posicao = pi[indice].tamanho;
		}else{
			pi[indice].posicao = pi[indice-1].posicao + pi[indice].tamanho;
		}
		indice++;
	}
}

void adiciona_parametro(struct Pilha *pi, char x, int n){
	//x = identificador (se é pi ou pa)
	//n = numero do parametro
	//variavel
	if (x == 'i'){
		sprintf(pi[indice].nome, "pi%d", n);
		pi[indice].tamanho = 4;
		if(indice == 0) {
			pi[indice].posicao = pi[indice].tamanho;
		}else{
			pi[indice].posicao = pi[indice-1].posicao + pi[indice].tamanho;
		}

		indice++;
		tamanho_pilha+=4;
	}
	// array
	/*else{
		sprintf(pi[indice].nome, "pa%d", n);

	}*/

	

}

void exibe_pilha(struct Pilha *pi, int x, int n, int ci) {
	printf("#-%d(%%rbp) -> %s\n", pi[indice2].posicao, pi[indice2].nome);
	tamanho_pilha = pi[indice2].posicao;
	indice2++;
}

int pega_posicao(struct Pilha *pi,char nome1, char nome2, int nome3){
	
	int i;
	char nome[4];
	sprintf(nome, "%c%c%d", nome1,nome2,nome3);

	for(i=0;i<=indice;i++){
		if (strcmp(nome, pi[i].nome) == 0) return pi[i].posicao;
	}
	return 0;
}

void chama_funcao(int funcao, int posicao1, int posicao2, int posicao3){

	if(posicao1 != 0)printf("leaq -%d(%%rbp), %%rdi\n",posicao1);
	if(posicao2 != 0)printf("leaq -%d(%%rbp), %%rsi\n",posicao2);
	if(posicao3 != 0)printf("leaq -%d(%%rbp), %%rdx\n",posicao3);

	printf("call f%d\n", funcao);

}

