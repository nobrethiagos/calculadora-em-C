#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct _corpoPilha{
 char elemento;
 struct _corpoPilha *prox;
} corpoPilha;

typedef corpoPilha* pilha;

typedef unsigned char bool;

#define true 1
#define false 0

corpoPilha *alocaPilha();
pilha criaPilha();
void liberaPilha(pilha p);
void empilha(pilha p, char x);
char desempilha(pilha p);
int prioridade(char c1, char c2);
void convertPosf(char expr[], char infix[]);
void zeroEsquerda(int vetor[], int tam);
int resolver(int n1, int n2, char op);

int main(int argc, char *argv[]) {
  if(argc != 2) {
		printf("\nERRO! As entradas não foram passadas corretamente.\nO programa deve ser iniciado seguindo o modelo:\n\n./calculadora \"2+2*(10/5)\"\n\n");
		return 1;
	}
  int i, j, res = 0, tam, qntN = 0, qntOp = 0, *num, *ajst, indexN = 0, indexOp = -1;
  char *posfix, *op;
  bool rep = false;
  tam = strlen(argv[1]);
  posfix = (char*) malloc((tam)*sizeof(char));
  convertPosf(argv[1], posfix);
  for(i = 0; i < tam; i++) {
    if(argv[1][i] >= '0' && argv[1][i] <= '9') {
      if(!rep) {
        qntN++;
        rep = true;
      }
    } else if(argv[1][i] != '(' && argv[1][i] != ')') {
      qntOp++;
      rep = false;
    }
  }
  ajst = (int*) calloc((qntN), sizeof(int));
  num = (int*) calloc((qntN + qntOp), sizeof(int));
  op = (char*) malloc((qntOp+1)*sizeof(char));
  rep = false;
  for(i = 0; i < tam; i++) {
    if(argv[1][i] >= '0' && argv[1][i] <= '9') {
      ajst[res]++;
      rep = false;
    } else {
      if(!rep) res++;
      rep = true;
    }
  }
  res = 0;
  i = 0;
  for(i = 0; i < tam+1; i++) {
    if(posfix[i] >= '0' && posfix[i] <= '9') {
      if(ajst[res] == 0) {
        res++;
        indexN++;
      }
      num[indexN] = 10*num[indexN] + posfix[i] - '0';
      ajst[res]--;
    } else if(posfix[i] != 0) {
      indexOp++;
      op[indexOp] = posfix[i];
      indexN++;
    }
  }
  indexOp = -1;
  for(i = 0; i < qntN + qntOp; i++) {
    if(num[i] == 0) {
      indexOp++;
      num[i] = resolver(num[i - 2], num[i - 1], op[indexOp]);
      num[i - 1] = 0;
      num[i - 2] = 0;
      zeroEsquerda(num, i);
    }
    if(i == qntN + qntOp - 1) printf("Resultado: %d\n", num[i]);
  }
  return 0;
}

corpoPilha *alocaPilha() {
  pilha pilha;
  pilha = (corpoPilha*)calloc(1, sizeof(corpoPilha));
  if(pilha == NULL) exit(-1);
  return pilha;
}

pilha criaPilha() {
  pilha pilha;
  pilha = alocaPilha();
  pilha->prox = NULL;
  return pilha;
}

void liberaPilha(pilha pilha){
  corpoPilha *q,*t;
  q = pilha;
  while(q != NULL){
  t = q;
  q = q->prox;
  free(t);
  }
}

void empilha(pilha pilha, char x){
  corpoPilha *q;
  q = alocaPilha();
  q->elemento = x;
  q->prox = pilha->prox;
  pilha->prox = q;
}

char desempilha(pilha pilha){
  corpoPilha *q;
  char x;
  q = pilha->prox;
  if(q == NULL) exit(-1);
  x = q->elemento;
  pilha->prox = q->prox;
  free(q);
  return x;
}

int prioridade(char c1, char c2){
  int p1,p2;
  if(c1 == '^')
    p1 = 4;
  else if(c1 == '*' || c1 == '/')
    p1 = 2;
  else if(c1 == '+' || c1 == '-')
    p1 = 1;
  else if(c1 == '(')
    p1 = 4;
  if(c2 == '^')
    p2 = 3;
  else if(c2 == '*' || c2 == '/')
    p2 = 2;
  else if(c2 == '+' || c2 == '-')
    p2 = 1;
  else if(c2 == '(')
    p2 = 0;
  return (p1 > p2);
}

void convertPosf(char expr[], char posfix[]){
  pilha p;
  int i = 0, indexPos = 0;
  char c,t;
  p = criaPilha();
  empilha(p, '(');
  do{
    c = expr[i];
    i++;
    if(c >= '0' && c <= '9') {
      posfix[indexPos] = c;
      indexPos++;
    }
    else if(c == '('){
      empilha(p, '(');
    }
    else if(c == ')' || c == '\0') {
      do{
        t = desempilha(p);
        if(t != '(')
          posfix[indexPos] = t;
          indexPos++;
      }while(t != '(');
    }
    else if(c == '+' || c == '-' || c == '*' || c == '/' || c == '^' ){
      while(1){
        t = desempilha(p);
        if(prioridade(c,t)) {
          empilha(p, t);
          empilha(p, c);
          break;
        }
        else {
          posfix[indexPos] = t;
          indexPos++;
        }
      }
    }
  } while(c != '\0');
  liberaPilha(p);
}

void zeroEsquerda(int vetor[], int tam) {
  int i = 0, j;
  bool troca;
  do {
    troca = false;
    for(i = 1; i < tam; i++) {
      if(vetor[i] == 0 && vetor[i - 1] != 0) {
        vetor[i] = vetor[i - 1];
        vetor[i - 1] = 0;
        troca = true;
      }
    }
  } while(troca);
}

int resolver(int n1, int n2, char op) {
  int pot = 1, i;
  switch(op) {
    case '+':
      return n1+n2;
    case '-':
      return n1-n2;
    case '*':
      return n1*n2;
    case '/':
      return n1/n2;
    case '^':
      for(i = 0; i < n2; i++) {
        pot *= n1;
      }
      return pot;
  }
}