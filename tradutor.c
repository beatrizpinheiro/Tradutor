#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define LINESZ 256 
#define TAM_MAX 99999



int indice = 0;
int indice2 = 0;
int indice3 = 0;
int tamanho_pilha=0;

struct Pilha{
	char nome[4];
	int tamanho;
	int posicao;
};

void remove_newline(char* ptr);
void cria_pilha(struct Pilha *pi, int x, int n, int ci);
int par_pilha(struct Pilha *pi, int *n, char *p);
void exibe_pilha(struct Pilha *pi, int n);
void adiciona_parametro(struct Pilha *pi, char x, int n);
int pega_posicao(struct Pilha *pi, char *variavel);
void chama_funcao(struct Pilha *pi, int *existe, int *pos, int nome_funcao);
void inicia_traducao(int f);
void inicia_salvamento(struct Pilha *pi);
int calcula_vetor(struct Pilha *pi, int index);


int main() {
	struct Pilha pi[TAM_MAX];
	int r, i1, i2, i4, i3, i5, i;
	int inicio, fim, passo;
	char line[LINESZ];
	int count = 0;
	int f, var, vet, ci, val, ind;
	char a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11;
	int contador_if = 0;
	char variavel[4], variavel2[4];
	int pos[10], existe[4];
	
	//Parametros da funcao
	int num[3];	
	char par[3];
	
	

	// Lê uma linha por vez
	printf(".data\n");
	printf(".text\n");
	while (fgets(line, LINESZ, stdin) != NULL) {
		count++;
		remove_newline(line);
	
		// Verifica se é uma função e quantos parâmetros
		r = sscanf(line, "function f%d p%c%d p%c%d p%c%d", 
				&f, &par[0], &num[0], &par[1], &num[1], &par[2], &num[2]);
				//printf("r antes = %d\n", r);
		if ((r-1) / 2 == 1) { // 1 parametro
			par_pilha(pi, num, par);
			indice = 1;
			tamanho_pilha = 8;
			//exibe_pilha(pi, 1);
		}
		else if ((r-1) / 2 == 2) { // 2 parametros
			par_pilha(pi, num, par);
			//exibe_pilha(pi, 2);
			indice = 2;
			tamanho_pilha = 16;
		}
		else if ((r-1) / 2 == 3) { // 3 parametros
			par_pilha(pi, num, par);
			//exibe_pilha(pi, 3);
			indice = 3;
			tamanho_pilha = 24;
		}
			
		//==DEF==//
		if (strncmp(line, "def", 3) == 0) {
			inicia_traducao(f);
		}
		
		//==VARIAVEIS==//
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
		
		//==ENDDEF==//
		if (strncmp(line, "enddef", 6) == 0) {
			if(tamanho_pilha > 0) {
				if(tamanho_pilha % 16 == 0) {
					printf("subq $%d, %%rsp\n", tamanho_pilha);
				}else {
					tamanho_pilha += 16 - tamanho_pilha % 16;
					printf("subq $%d, %%rsp\n", tamanho_pilha);
				}
			}
				inicia_salvamento(pi);
		}
		

		//==IF==//
		r = sscanf(line, "if ci%d", &i1);
		//If com ci
		if(r == 1){
			printf("cmpl $0, $%d\n" ,i1);	
			printf("je end_if%d\n", contador_if);
			continue;
		}
		
		r = sscanf(line, "if %c%c%c", &a0,&a1,&a2);
		//If com variavel ou parametro
		if (r == 3) {
			sprintf(variavel, "%c%c%c", a0,a1,a2);	
			printf("cmpl $0, -%d(%%rbp)\n", pega_posicao(pi,variavel));
			printf("je end_if%d\n", contador_if);
			continue;
		}
		
		//==ENDIF==//
		if (strncmp(line, "endif", 5) == 0) {
			printf("end_if%d:\n", contador_if);
			contador_if++;
			continue;
		}
		
		//Se igual a 4 é porque tem sinal negativo
		if(r == 4){
			// ??
		}
	
		// Verifica o que está retornando
		//Retorno constante
		r = sscanf(line, "return ci%d", &i1);
		
		if( r == 1){
			printf("movl $%d, %%eax\n" ,i1);
			printf("leave\nret\n");
			continue;
		}
		//Retorno variavel ou parametro inteiro
		r = sscanf(line, "return %c%c%c", &a0,&a1,&a2);

		if(r == 3){
			sprintf(variavel, "%c%c%c", a0,a1,a2);	
			printf("movl -%d(%%rbp), %%eax\n",pega_posicao(pi,variavel));	
			printf("leave\nret\n");		
			continue;		
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
		r = sscanf(line, "%c%c%d = call f%d %c%c%d %c%c%d %c%c%d ", &a9, &a10, &i1, &i2, &a0, &a1, &i3, &a3, &a4, &i4,&a6, &a7, &i5);
		//Pega a posição de todos
		//Primeira variavel, que recebe a atribuição (VI1 = call f1 por exemplo)
		if(r>3){
			existe[1] = 0;
			existe[2] = 0;
			existe[3] = 0;
			sprintf(variavel, "%c%c%d", a9,a10,i1);	
			pos[0] = pega_posicao(pi, variavel);
			pos[0] = indice3;
			//Primeiro Parametro da função, se tiver
			if(r >= 7){
				if(a0 != 'c'){
					sprintf(variavel, "%c%c%d", a0,a1,i3);	
					pos[1] = pega_posicao(pi, variavel);
					pos[1] = indice3;
					existe[1] = 2;
				}
				else{	
					pos[1] = i3;
					existe[1] = 1;
				}	
			}
			//Segundo Parametro da função, se tiver
			if(r >= 10){
				if(a3 != 'c'){
					sprintf(variavel, "%c%c%d", a3,a4,i4);
					pos[2] = pega_posicao(pi, variavel);
					pos[2] = indice3;
					existe[2] = 2;
				}
				else{	
					pos[2] = i4;
					existe[2] = 1;
				}	
			}
			//Terceiro Parametro da função, se tiver
			if(r >=13){
				if(a6 != 'c'){
					sprintf(variavel, "%c%c%d", a6,a7,i5);	
					pos[3] = pega_posicao(pi, variavel);
					pos[3] = indice3;
					existe[3] = 2;
				}
				else{	
					pos[3] = i5;
					existe[3] = 1;
				}
			}

			chama_funcao(pi,existe,pos,i2);
			continue;
		}

		//===REALIZA AS OPERAÇÕES SET WITH===//
		r = sscanf(line, "set %ca%d index ci%d with %ci%d", &a0, &i1, &i2, &a1, &i3);
		if (r == 5) {
			sprintf(variavel, "%ca%d", a0,i1);
			pega_posicao(pi, variavel);
			printf("#set %ca%d index ci%d with %ci%d\n", a0, i1, i2, a1, i3);
			//#set "a0"a"i1" index ci"i2" with "a1"i"i3"
			//printf("%ca%d = -%d --> ci%d\n",a0,i1,calcula_vetor(pi, i2), i2);
			
			//Operaçao com vetor//
			if(variavel[0] == 'v') {
				printf("leaq -%d(%%rbp), %%r8\n", calcula_vetor(pi, i2));
				if(a1 == 'c'){
					/*
						with -> constante
						utiliza a calcula vetor p/ pegar o vetor no indice correto
					*/
					printf("movq $%d, (%%r8)\n", i3);
				}else {
					/*
						with -> variavel 
						utiliza a calcula_vetor p/ pegar o vetor no indice correto
						pega_posicao pra pegar a posicao da variavel na estrutura
					*/ 
					sprintf(variavel, "%ci%d", a1,i3);
					pega_posicao(pi, variavel);
					printf("movq -%d(%%rbp), %%r10\n", pi[indice3].posicao);
					printf("movq %%r10, (%%r8)\n");
				}
			}
			//Operação com parametro//
			if(variavel[0] == 'p') {
				/*

					pega_posicao pra pegar a posicao do parametro na estrutura
					r8 recebe parametro, r9 recebe indice
					multiplicamos r9 por 4(tamanho de um int)
					adicionamos r9 a r8 para ter a posição exata do parametro	
				*/
				printf("movq -%d(%%rbp), %%r8\n", pi[indice3].posicao);
				printf("movabs $%d, %%r9\n", i2);
				printf("imulq $4, %%r9\n");
				printf("addq %%r9, %%r8\n");
				if(a1 == 'c') {
					printf("movl $%d, (%%r8)\n", i3);
				}else {
					sprintf(variavel, "%ci%d", a1,i3);
					pega_posicao(pi, variavel);
					printf("movq -%d(%%rbp), %%r10\n", pi[indice3].posicao);
					printf("movq %%r10, (%%r8)\n");
				}
			}
			printf("\n");
			
		}
		//===REALIZA AS OPERAÇÕES GET TO===//
		r = sscanf(line, "get %ca%d index ci%d to %ci%d", &a0, &i1, &i2, &a1, &i3);
		if (r == 5) {
			sprintf(variavel, "%ca%d", a0,i1);
			sprintf(variavel2, "%ci%d", a1, i3);
			pega_posicao(pi, variavel);
			//int calcula_vetor(struct Pilha *pi, int index, int pos)
			//printf("%ca%d = -%d --> ci%d\n",a0,i1,calcula_vetor(pi, i2), i2);
			
		}
		
		
		
		
		//===FINALIZA O PROGRAMA===//
		if (strncmp(line, "end", 4) == 0) {
			indice = 0;
			indice2 = 0;
			contador_if = 0;
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
		x = identificador (se é vi=0, va=1)
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

int par_pilha(struct Pilha *pi, int *n, char *p) {
	int i, c;
	char str[4] = {'d', 's', 'd', 'c'};
	char str2[4] = {'i', 'i', 'x', 'x'};
	
	i = indice;
	if(n[1] == 0 && n[2] == 0) { // 1 par
		sprintf(pi[i].nome, "p%c%d", p[i], n[i]);
		pi[i].tamanho = 8;
		if(i == 0) {
			pi[i].posicao = pi[i].tamanho;
		}else{
			pi[i].posicao = pi[i-1].posicao + pi[i].tamanho;
		}
		//printf("movq %%rdi, -%d(rbp)\n", pi[i].posicao);
	}
	else if(n[2] == 0) { // 2 par
		c=0;
		for(i = indice; i < indice+2; i++) {
			sprintf(pi[i].nome, "p%c%d", p[i], n[i]);
			pi[i].tamanho = 8;
			if(i == 0) {
				pi[i].posicao = pi[i].tamanho;
			}else{
				pi[i].posicao = pi[i-1].posicao + pi[i].tamanho;
			}
			//printf("movq %%r%c%c, -%d(rbp)\n", str[c], str2[c], pi[i].posicao);
			c++;
		}
	}
	else if(n[2] != 0 ) { // 3 par
		for(i = indice; i < indice+3; i++) {
			sprintf(pi[i].nome, "p%c%d", p[i], n[i]);
			pi[i].tamanho = 8;
			if(i == 0) {
				pi[i].posicao = pi[i].tamanho;
			}else{
				pi[i].posicao = pi[i-1].posicao + pi[i].tamanho;
			}
			//printf("movq %%r%c%c, -%d(rbp)\n", str[c], str2[c], pi[i].posicao);
			c++;
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
			indice3 = i;
			return pi[i].posicao;
		}
		
	}
	
	return 0;

}

void chama_funcao(struct Pilha *pi, int *existe, int *pos, int nome_funcao){
	
	char variavel[4];
	int pos0 = pos[0];
	int pos1 = pos[1];
	int pos2 = pos[2];
	int pos3 = pos[3];

	//Se existe == 1 é constante
	//Se existe == 2 é variavel ou parametro
	//Se existe == 0 não tem esse parametro
		
	if(existe[1] == 1) printf("movl $%d, %%edi\n",pos1);
	if(existe[1] == 2){
			sprintf(variavel, "%s" ,pi[pos1].nome);
			//Parametro array
			if(variavel[1] == 'a'){
				if(variavel[0] == 'p') printf("movq ");
				if(variavel[0] == 'v') printf("leaq ");
				printf("-%d(%%rbp), %%rdi\n",pi[pos1].posicao);
			}

			//Parametro int
			if(variavel[1] == 'i' && variavel[0] != 'c'){
				printf("movl -%d(%%rbp), %%edi\n",pi[pos1].posicao);

			}
	}

	if(existe[2] == 1) printf("movl $%d, %%edi\n",pos2);
	if(existe[2] == 2){
			sprintf(variavel, "%s" ,pi[pos2].nome);
			//Parametro array
			if(variavel[1] == 'a'){
				if(variavel[0] == 'p') printf("movq ");
				if(variavel[0] == 'v') printf("leaq ");
				printf(" -%d(%%rbp), %%rsi\n",pi[pos2].posicao);
			}
			//Parametro int
			if(variavel[1] == 'i' && variavel[0] != 'c'){
				printf("movl-%d(%%rbp), %%esi\n",pi[pos2].posicao);

			}
	}
	if(existe[3] == 1) printf("movl $%d, %%edx\n",pos3);
	if(existe[3] == 2){
			sprintf(variavel, "%s" ,pi[pos3].nome);
			//Parametro array
			if(variavel[1] == 'a'){
				if(variavel[0] == 'p') printf("movq ");
				if(variavel[0] == 'v') printf("leaq ");
				printf(" -%d(%%rbp), %%rdx\n",pi[pos3].posicao);
			}
			//Parametro int
			if(variavel[1] == 'i' && variavel[0] != 'c'){
				printf("movl-%d(%%rbp), %%edx\n",pi[pos3].posicao);

			}
	}

	printf("call f%d\n", nome_funcao);
	printf("movl %%eax, -%d(%%rbp)\n", pi[pos0].posicao);

}

void inicia_traducao(int f) {
	printf(".globl f%d\nf%d:\n", f, f);
	printf("pushq %%rbp\n");
	printf("movq  %%rsp, %%rbp\n");
}

void inicia_salvamento(struct Pilha *pi) {
	int i, c=0;
	char str[4] = {'d', 's', 'd', 'c'};
	char str2[4] = {'i', 'i', 'x', 'x'};
	
	for(i=0; i<indice; i++) {
		if(pi[i].nome[0] == 'p') {
			if(pi[i].nome[2] == '1') printf("\n#salvando os parametros\n");
			printf("movq %%r%c%c, -%d(%%rbp)\n", str[c], str2[c], pi[i].posicao);
			c++;
		}
	}
	
	printf("\n");
}

int calcula_vetor(struct Pilha *pi, int index) {
	/*
		Essa funcao calcula a posicao de um vetor com base no indice
		index = indice desejado 
		indice3 = indice da estrutura
	*/
	int a, ci, i;
	
	i = index;
	ci = pi[indice3].tamanho / 4;	
	a = pi[indice3].posicao - (ci*4);
	for(i; i<ci; i++){
		a = a + 4;
	}
	return a;
}



