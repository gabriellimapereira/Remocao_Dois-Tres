#include <stdio.h>
#include <stdlib.h>

// CÓDIGO DO DIABO

typedef struct DoisTres  {
    int infoUm, infoDois, quantInfo;
    struct DoisTres *esq, *cen, *dir;
} DoisTres;

DoisTres* criarNo(int info, DoisTres *fEsq, DoisTres *fCen) {
    DoisTres *no;
    no = (DoisTres*) malloc(sizeof(DoisTres));

    if (no != NULL) {
        no->infoUm = info;
        no->esq = fEsq;
        no->cen = fCen;
        no->dir = NULL;
        no->quantInfo = 1;
    }
    return no;
}

void imprimirArv(DoisTres *raiz, int nivel)  {
    if (raiz == NULL) return;
    // imprime da direita pra esquerda (como uma árvore "virada")

    imprimirArv(raiz->dir, nivel + 1);

    for (int i = 0; i < nivel; i++) printf("   "); //espaçamento visual 
    
    if (raiz->quantInfo == 1) 
        printf("[%d]\n", raiz->infoUm);
    else 
        printf("[%d|%d]\n", raiz->infoUm, raiz->infoDois);
    
    imprimirArv(raiz->cen, nivel + 1);
    imprimirArv(raiz->esq, nivel + 1);
}

void liberarArv(DoisTres **raiz)
{
    if (*raiz) {
        liberarArv(&((*raiz)->esq));
        liberarArv(&((*raiz)->cen));
        if ((*raiz)->quantInfo == 2) liberarArv(&((*raiz)->dir));

        free(*raiz);
        *raiz = NULL;
    }
}

void adicionarInfo(DoisTres **no, int info, DoisTres *subArvInfo)
{
    if (info > (*no)->infoUm) {
        (*no)->infoDois = info;
        (*no)->dir = subArvInfo;
    } else {
        (*no)->infoDois = (*no)->infoUm;
        (*no)->infoUm = info;
        (*no)->dir = (*no)->cen;
        (*no)->cen = subArvInfo;
    }
    (*no)->quantInfo = 2;
}

DoisTres* quebrarNo(DoisTres **no, int info, int *sobe, DoisTres *filhoDir) {
    DoisTres *maior;

    if(info > (*no)->infoDois) {
        *sobe = (*no)->infoDois;
        maior = criarNo(info, (*no)->dir, filhoDir);
        (*no)->quantInfo = 1;
    } else if(info > (*no)->infoUm) {
        *sobe = info;
        maior = criarNo((*no)->infoDois, filhoDir, (*no)->dir);
        (*no)->quantInfo = 1;
    } else {
        *sobe = (*no)->infoUm;
        maior = criarNo((*no)->infoDois, (*no)->cen, (*no)->dir);
        (*no)->infoUm = info;
        (*no)->cen = filhoDir;
        (*no)->quantInfo = 1;
    }
    return maior;
}

DoisTres* inserirNo(DoisTres **raiz, DoisTres *pai, int info, int *sobe) 
{
    DoisTres *maiorNo = NULL;

    if (*raiz == NULL) {
        *raiz = criarNo(info, NULL, NULL);
    }
    else {
        if ((*raiz)->esq == NULL) {
            if ((*raiz)->quantInfo == 1) {
                adicionarInfo(raiz, info, NULL);
            } else {
                maiorNo = quebrarNo(raiz, info, sobe, NULL);
                if (pai == NULL) {
                    *raiz = criarNo(*sobe, *raiz, maiorNo);
                    maiorNo = NULL;
                }
            }
        } 
        else {
            if (info < (*raiz)->infoUm)
                maiorNo = inserirNo(&((*raiz)->esq), *raiz, info, sobe);
            else if((*raiz)->quantInfo == 1 || info < (*raiz)->infoDois)
                maiorNo = inserirNo(&((*raiz)->cen), *raiz, info, sobe);
            else
                maiorNo = inserirNo(&((*raiz)->dir), *raiz, info, sobe);

            if (maiorNo != NULL) {
                if ((*raiz)->quantInfo == 1) {
                    adicionarInfo(raiz, *sobe, maiorNo);
                    maiorNo = NULL;
                } 
                else {
                    maiorNo = quebrarNo(raiz, *sobe, sobe, maiorNo);
                    if (pai == NULL) {
                        *raiz = criarNo(*sobe, *raiz, maiorNo);
                        maiorNo = NULL;
                    }
                }
            }
        }
    }

    return maiorNo;
}

int verificaInfos(DoisTres *raiz, int valor) 
{
    int existe = 0;

    if (raiz->infoUm == valor /*|| raiz->infoDois == valor*/) 
        existe = 1;        
    else if(raiz->quantInfo == 2)
        if(raiz->infoDois == valor) existe = 1;

    return existe;
}

int remover(DoisTres **raiz, DoisTres **pai, int valor, int valorRaiz) {
    int removeu = 1;
    DoisTres **filho;

    if (*raiz) { // se a raiz não é nula
        if (verificaInfos((*raiz), valor)) { // se o valor a ser removido foi encontrado
            if ((**raiz).quantInfo == 2) { // se o nó tem duas chaves
                if ((**raiz).esq == NULL) { // se o nó é folha
                    if ((**raiz).infoUm == valor) { 
                        (**raiz).infoUm = (**raiz).infoDois;
                    } 
                        (**raiz).quantInfo = 1;
                }
                
                if ((**raiz).infoDois == valor) { // se o valor está na info dois
                    if (((**raiz).dir)->quantInfo == 2) {
                        filho = &((**raiz).dir);

                        (**raiz).infoDois = (**raiz).dir->infoUm;
                        (**raiz).dir->infoUm = (**raiz).dir->infoDois;
                        (**raiz).dir->quantInfo = 1;
                    } else if (((**raiz).dir)->quantInfo == 1 && ((**raiz).cen)->quantInfo == 2){
                        filho = &((**raiz).cen);

                        (**raiz).infoDois = (**raiz).cen->infoDois;
                        (**raiz).cen->quantInfo = 1;
                    } else if (((**raiz).dir)->quantInfo == 1 && ((**raiz).cen)->quantInfo == 1) {
                        (**raiz).cen->infoDois = (**raiz).dir->infoUm;
                        (**raiz).dir == NULL;
                        (**raiz).quantInfo = 1;
                    }
                } else if ((**raiz).infoUm == valor) { // se o valor está na info um
                    if (((**raiz).cen)->quantInfo == 2) {
                        (**raiz).infoUm = (**raiz).cen->infoUm;
                        (**raiz).cen->infoUm = (**raiz).cen->infoDois;
                        (**raiz).cen->quantInfo = 1;
                    } else if (((**raiz).cen)->quantInfo == 1 && ((**raiz).dir)->quantInfo == 2) {
                        (**raiz).infoUm = (**raiz).cen->infoUm;
                        (**raiz).cen->infoUm = (**raiz).infoDois;
                        (**raiz).cen->infoDois = (**raiz).dir->infoUm;
                        (**filho).quantInfo = 1;
                    } else if (((**raiz).cen)->quantInfo == 1 && ((**raiz).dir)->quantInfo == 1) {
                        (**raiz).infoUm = (**raiz).cen->infoUm;
                        (**raiz).cen->infoUm = (**raiz).infoDois;
                        (**raiz).cen->infoDois = (**raiz).dir->infoUm;
                        (**raiz).dir = NULL;
                        (**raiz).quantInfo = 1;
                        (**raiz).cen->quantInfo = 2;
                    }
                }
            } else {
                if ((**raiz).esq) {
                    if ((**raiz).esq->quantInfo == 2 && (**raiz).cen->quantInfo == 2) {
                        (**raiz).infoUm = (**raiz).cen->infoUm;
                        (**raiz).cen->infoUm = (**raiz).cen->infoDois;
                        (**raiz).cen->quantInfo = 1;
                    } else if ((**raiz).esq->quantInfo == 2 && (**raiz).cen->quantInfo == 1) {
                        (**raiz).infoUm = (**raiz).esq->infoDois;
                        (**raiz).esq->quantInfo = 2;
                    }
                } else {
                    if ((**pai).quantInfo == 2) {
                        if ((**raiz).infoUm > (**pai).infoDois) {
                            if ((**pai).cen->quantInfo == 2) {
                                (**raiz).infoUm = (**pai).infoDois;
                                (**pai).infoDois = (**pai).cen->infoDois;
                                (**pai).cen->quantInfo = 1;
                            } else {
                                (**pai).cen->infoDois = (**pai).infoDois;
                                (**pai).quantInfo = 1;
                                (**pai).cen->quantInfo = 2;
                                (*raiz) = NULL;
                                (**pai).dir = NULL;
                            }
                        } else if ((**raiz).infoUm < (**pai).infoDois && (**raiz).infoUm > (**pai).infoUm) {
                            if ((**pai).dir->quantInfo == 2) {
                                (**raiz).infoUm = (**pai).infoDois;
                                (**pai).infoDois = (**pai).dir->infoUm;
                                (**pai).dir->infoUm = (**pai).dir->infoDois;
                                (**pai).dir->quantInfo = 1;
                            } else {
                                (**raiz).infoUm = (**pai).infoDois;
                                (**raiz).infoUm = (**pai).dir->infoUm;
                                (**pai).quantInfo = 1;
                                (**pai).dir = NULL;
                            }
                        } else {
                            if ((**pai).quantInfo == 2) {
                                if ((**pai).cen->quantInfo == 2 && (**pai).dir->quantInfo == 2) {
                                    (**raiz).infoUm = (**pai).infoUm;
                                    (**pai).infoUm = (**pai).cen->infoUm;
                                    (**pai).cen->infoUm = (**pai).cen->infoDois;
                                    (**pai).cen->infoDois = (**pai).infoDois;
                                    (**pai).infoDois = (**pai).dir->infoUm;
                                    (**pai).dir->infoUm = (**pai).infoDois;
                                    (**pai).dir->quantInfo = 1;
                                } else if ((**pai).cen->quantInfo == 2 && (**pai).dir->quantInfo == 1) {
                                    (**raiz).infoUm = (**pai).infoUm;
                                    (**pai).infoUm = (**pai).cen->infoUm;
                                    (**pai).cen->infoUm = (**pai).cen->infoDois;
                                    (**pai).cen->quantInfo = 1;
                                } else if ((**pai).cen->quantInfo == 1 && (**pai).dir->quantInfo == 2) {
                                    (**raiz).infoUm = (**pai).infoUm;
                                    (**pai).infoUm = (**pai).cen->infoUm;
                                    (**pai).cen->infoUm = (**pai).infoDois;
                                    (**pai).infoDois = (**pai).dir->infoUm;
                                    (**pai).dir->infoUm = (**pai).infoDois;
                                    (**pai).dir->quantInfo = 1;
                                } else if ((**pai).cen->quantInfo == 1 && (**pai).dir->quantInfo == 1) {
                                    (**raiz).infoUm = (**pai).infoUm;
                                    (**pai).infoUm = (**pai).cen->infoUm;
                                    (**pai).cen->infoUm = (**pai).infoDois;
                                    (**pai).cen->infoDois = (**pai).dir->infoUm;
                                    (**pai).dir = NULL;
                                    (**pai).cen->quantInfo = 2;
                                }
                            }
                        }
                    } 
                }
            }
        } else {
            if (valor < (*raiz)->infoUm)
                removeu = remover(&((*raiz)->esq), &(*raiz), valor, valorRaiz);
            else if((*raiz)->quantInfo == 1 || valor < (*raiz)->infoDois)
                removeu = remover(&((*raiz)->cen), &(*raiz), valor, valorRaiz);
            else
                removeu = remover(&((*raiz)->dir), &(*raiz), valor, valorRaiz);
        }
    } else {
        removeu = 0;
    }

    return removeu;
}

int main() {
    printf("inicializando a main...\n");

    DoisTres *raiz = NULL;
    int sobe;

    int valores[] = {1000, 300, 250, 200, 350, 2000, 400, 3000};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++) {
        inserirNo(&raiz, NULL, valores[i], &sobe);
    }
    
    imprimirArv(raiz, 0);

    printf("\n");

    DoisTres *pai = NULL;
    int valor = 2000;
    int removeu = remover(&raiz, &pai, valor, (*raiz).infoUm);
    if (removeu) {
            printf("\nárvore depois da remoção:\n");
        imprimirArv(raiz, 0);
    } else {
        printf("\nremoção deu errado, pra variar\n");
    }

    printf("\n");

    liberarArv(&raiz);
    return 0;
}