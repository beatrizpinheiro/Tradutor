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

void remove_newline(char* ptr);
void cria_pilha(struct Pilha *pi, int x, int n, int ci);
void par_pilha(struct Pilha *pi, int n[3]);
void exibe_pilha(struct Pilha *pi, int n);
void adiciona_parametro(struct Pilha *pi, char x, int n);
int pega_posicao(struct Pilha *pi, char *variavel);
void chama_funcao(int pos0,int pos1,int pos2,int pos3,int nome_funcao);


int main() {
	struct Pilha pi[TAM_MAX];
	int r, r1=0, i1, i2, i4, i3, i5, i;
	int inicio, fim, passo;
	char line[LINESZ];
	int count = 0;
	int f, var, vet, ci, val, ind;
	char a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11;
	int contador_if = 0;
	char variavel[4];
	int pos[10];
	
	//Parametros da funcao
	int num[3]; 	
	char par1, par2, par3;
	
	printf(".data\n");
	printf(".text\n");

	// Lê uma linha por vez
	while (fgets(line, LINESZ, stdin) != NULL) {
		count++;
		remove_newline(line);

		//Verifica endif
		if (strncmp(line, "endif", 5) == 0) {
			printf("end_if%d:\n", contador_if);
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
			/*if( quantidade_parametros > 0) adiciona_parametro(pi,parametro[0],1);
			if( quantidade_parametros > 1) adiciona_parametro(pi,parametro[1],2);
			if( quantidade_parametros > 2) adiciona_parametro(pi,parametro[2],3);*/
			if(tamanho_pilha > 0) {
				tamanho_pilha += 16 - tamanho_pilha % 16;
				printf("subq $%d, %%rsp\n", tamanho_pilha);
				continue;
			}
		}

		// Verifica se line começa com 'end' (3 letras)
		if (strncmp(line, "end", 3) == 0) {
			indice = 0;
			indice2 = 0;
			printf("leave\nret\n");
			continue;
		}

		// Verifica se é um 'if'
		//a3 para o caso de a2 ser um sinal negativo
		r = sscanf(line, "if %c%c%c%c", &a0, &a1,&a2, &a3);
		if (r == 3) {
			variavel[0] = a0;
			variavel[1] = a1;		 
			variavel[2] = a2;
			//Ou ci de array ou pi de parametro ou vi de variavel	
			if(variavel[0] != 'c'){
				printf("cmpl -%d(%%rbp)\n", pega_posicao(pi,variavel));
			//printf("cmpl %ci%d\n", a0, i1);
			}
			else{
				printf("cmpl $%c\n" ,variavel[2]);	
			}
			contador_if++;
			printf("je end_if%d\n", contador_if);

			continue;
		}
		//Se igual a 4 é porque tem sinal negativo
		if(r == 4){
			// ??
		}
		// Verifica se é uma função e quantos parâmetros
		r1 = sscanf(line, "function f%d p%c%d p%c%d p%c%d", 
					&f, &par1, &num[0], &par2, &num[1], &par3, &num[2]);
		if (r1> 0 && r1<7) printf(".globl f%d\nf%d:\n", f, f);
			//quantidade_parametros = r-1;
			if ((r1-1) / 2 == 1) { // 1 parametro
				//printf("a funcao tem 1 parametro\n");
				par_pilha(pi, num);
				exibe_pilha(pi, 1);
				indice = 1;
			}
			else if ((r1-1) / 2 == 2) { // 2 parametros
				//printf("a funcao tem 2 parametros\n");
				par_pilha(pi, num);
				exibe_pilha(pi, 2);
				indice = 2;
			}
			else if ((r1-1) / 2 == 3) { // 3 parametros
				//printf("a funcao tem 3 parametros\n");
				par_pilha(pi, num);
				exibe_pilha(pi, 3);
				indice = 3;
			}
		

		// Verifica se a linha esta inicializando uma variável ou um vetor  
		r = sscanf(line, "var vi%d", &var);
		if (r == 1) {
			//printf("var vi%d\n", var);
			cria_pilha(pi, 0, var, 0);
			exibe_pilha(pi, 0);
		}
		r = sscanf(line, "vet va%d size ci%d", &vet, &ci);
		if (r == 2) {
			//printf("vet va%d ci%d\n", vet, ci);
			cria_pilha(pi, 1, vet, ci);
			exibe_pilha(pi, 0);
		}
		// Verifica o que está retornando
		// a variavel a3 é para o caso de a2 ser um sinal negativo
		r = sscanf(line, "return %c%c%c%c", &a0,&a1,&a2,&a3);

		if(r == 3){
			variavel[0] = a0;
			variavel[1] = a1;		 
			variavel[2] = a2;
			if(variavel[0] == 'c'){
				printf("movl $%c, %%eax\n" ,variavel[2]);		
			}
			else{
				printf("leaq -%d(%%rbp), %%rax\n",pega_posicao(pi,variavel));			
			}		
		}
		//Como no caso de return ci-1	
		if(r == 4){
			//Como fazer $-1???
			//printf("movl $-%c, %%eax\n" ,a3); ???
		}

		// Verifica se é expressão simples
		r = sscanf(line, "%ci%d = %ci%d %c %ci%d", &a0, &val, &a1, &i1, &a2, &a3, &i2);
		
		// vi1 = ci1 
		if (r == 4) {
		

		}
		// vi1 = pi1 + vi2, vi2 = vi1 * ci-5, etc   pi1 = pi2 + ci2
		if (r == 7) {
			//DIVISÃO
			if (a2 == '/') {
				
			}
			// OUTRAS OPERAÇÕES
			else {
				
			}
		}



		//Chamada de função
		r = sscanf(line, "%c%c%c = call f%d %c%c%c %c%c%c %c%c%c ", &a9, &a10, &a11, &i2, &a0, &a1, &a2, &a3, &a4, &a5,&a6, &a7, &a8);
		//Pega a posição de todos
		//Primeira variavel, que recebe a atribuição (VI1 = call f1 por exemplo)
		if(r>3){
			variavel[0] = a9;
			variavel[1] = a10;		 
			variavel[2] = a11;	
			pos[0] = pega_posicao(pi, variavel);
			//Primeiro Parametro da função, se tiver
			if(r >= 7){
				variavel[0] = a0;
				variavel[1] = a1;		 
				variavel[2] = a2;	
				pos[1] = pega_posicao(pi, variavel);
			}
			//Segundo Parametro da função, se tiver
			if(r >= 10){
				variavel[0] = a3;
				variavel[1] = a4;		 
				variavel[2] = a5;	
				pos[2] = pega_posicao(pi, variavel);
			}
			if(r >=13){
			//Terceiro Parametro da função, se tiver
				variavel[0] = a6;
				variavel[1] = a7;		 
				variavel[2] = a8;	
				pos[3] = pega_posicao(pi, variavel);
			}

			chama_funcao(pos[0],pos[1],pos[2],pos[3],i2);
			continue;
		}

		//Verifica acesso a Array
		//set with
		r = sscanf(line, "set %ca%d index ci%d with %ci%d", &a0, &i1, &i2, &a1, &i3);
		if (r == 5) {
			//va2[5] = 2
		}
		//get to
		r = sscanf(line, "get %ca%d index ci%d to %ci%d", &a0, &i1, &i2, &a1, &i3);
		if (r == 5) {
			//vi1 = va2[8]
			
		}
	}
	
	return 0;
}


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


void cria_pilha(struct Pilha *pi, int x, int n, int ci) {
	/*
		x = identificador (se é vi=0, va=1, p=3)
		n = número da var/vet/p 
		ci = indice do vetor (se for var/p ci = 0)
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

void par_pilha(struct Pilha *pi, int n[3]) {
	int i;
	i = indice;
	if(n[1] == 0 && n[2] == 0) { // 1 par
		sprintf(pi[i].nome, "px%d", n[i]);
		pi[i].tamanho = 8;
		if(i == 0) {
			pi[i].posicao = pi[i].tamanho;
		}else{
			pi[i].posicao = pi[i-1].posicao + pi[i].tamanho;
		}
		
	}
	else if(n[2] == 0) { // 2 par
		for(i = indice; i < indice+2; i++) {
			sprintf(pi[i].nome, "px%d", n[i]);
			pi[i].tamanho = 8;
			if(i == 0) {
				pi[i].posicao = pi[i].tamanho;
			}else{
				pi[i].posicao = pi[i-1].posicao + pi[i].tamanho;
			}
		}
	}
	else if(n[2] != 0 ) { // 3 par
		for(i = indice; i < indice+3; i++) {
			sprintf(pi[i].nome, "px%d", n[i]);
			pi[i].tamanho = 8;
			if(i == 0) {
				pi[i].posicao = pi[i].tamanho;
			}else{
				pi[i].posicao = pi[i-1].posicao + pi[i].tamanho;
			}
		}
	}
}

void exibe_pilha(struct Pilha *pi, int n) {
	while(indice2<indice) {
		printf("#-%d(%%rbp) -> %s\n", pi[indice2].posicao, pi[indice2].nome);
		tamanho_pilha = pi[indice2].posicao;
		indice2++;
	}
}

int pega_posicao(struct Pilha *pi, char *variavel){

	int i;
	
	for(i=0;i<indice;i++){
		if(strcmp(pi[i].nome, variavel) == 0){
			return pi[i].posicao;
		}
		
	}

	return 0;

}

void chama_funcao(int pos0,int pos1,int pos2,int pos3,int nome_funcao){

	if(pos1 != 0) printf("leaq -%d(%%rbp), %%rdi\n",pos1);
	if(pos2 != 0)printf("leaq -%d(%%rbp), %%rsi\n",pos2);
	if(pos3 != 0)printf("leaq -%d(%%rbp), %%rdx\n",pos3);

	printf("call f%d\n", nome_funcao);
	printf("movq %%rax, -%d(%%rbp)\n", pos0);

}




