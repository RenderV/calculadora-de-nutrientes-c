#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#define TAM_BUFFER 6144
#define TAM_LINHA 1000
#define MAX_ING 100
const char FORMATO_CSV[] = "%i::%[^::]::%f::%f::%f::%f::%f::%f::%f::%f::%f::%f::%f::";
const char SEPARADOR_1[] = "---------------------------------------------------------";
const char SEPARADOR_2[] = "=========================================================";
const char menu_default[] = "COMANDOS: \n"
            "a: adicionar\n"
            "r: remover\n"
            "c: sair\n"
            "*Diferencia maiúsculas e maiúsculas na pesquisa";
char INPUT_USUARIO[1000];
typedef struct ingrediente {
    int id;
    char nome[TAM_LINHA];
    float umidade;
    float kcal;
    float kj;
    float proteina;
    float lipideos;
    float colesterol;
    float carboidrato;
    float fibra;
    float cinzas;
    float calcio;
    float magnesio;
} ingrediente;

typedef struct ingredientes_selecionado{
    ingrediente *selecionado;
    float quantidade;
} ingrediente_selecionado;

int cont_linhas(FILE* fp);

int main(int argc, char *argv[])
{
  	 setlocale(LC_ALL, "Portuguese");

    // Abre e lê o arquivo, aloca memória
    FILE *fp = fopen("taco.txt", "r");
    if(fp == NULL){
        fputs("Erro ao abrir o arquivo.\n\n\n", stderr);
        return -1;
    }
    int num_linhas = cont_linhas(fp);
    ingrediente *ings = (ingrediente*)calloc(num_linhas, sizeof(ingrediente)*num_linhas);
    ingrediente_selecionado *ing_selecionados = (ingrediente_selecionado*)calloc(MAX_ING, sizeof(ingrediente_selecionado)*MAX_ING);
    char linha_lida[TAM_LINHA+10];

     for (int i = 0; i < num_linhas; i++){
        fgets(linha_lida, TAM_LINHA+10, fp);
        if(strlen(linha_lida)>=TAM_LINHA){
            fprintf(stderr, "A linha %i excedeu o limite de %i caracteres.\n", i+1, TAM_LINHA);
            fclose(fp);
            free(ings);
            return -1;
        }
        sscanf(linha_lida, FORMATO_CSV,
        &ings[i].id,
        ings[i].nome,
        &ings[i].umidade,
        &ings[i].kcal,
        &ings[i].kj,
        &ings[i].proteina,
        &ings[i].lipideos,
        &ings[i].colesterol,
        &ings[i].carboidrato,
        &ings[i].fibra,
        &ings[i].cinzas,
        &ings[i].calcio,
        &ings[i].magnesio);
   }
        // PROGRAMA
        int num_selecionado = 0;
        ingrediente soma;
        while(1){
            puts(SEPARADOR_2);
            soma.kcal = 0;
            soma.proteina = 0;
            soma.lipideos = 0;
            soma.colesterol = 0;
            soma.carboidrato = 0;
            soma.fibra = 0;
            soma.calcio = 0;
            for (int i = 0; i < num_selecionado; i++)
            {
                soma.kcal += (ing_selecionados[i].selecionado->kcal/MAX_ING) * ing_selecionados[i].quantidade;
                soma.proteina += (ing_selecionados[i].selecionado->proteina/MAX_ING) * ing_selecionados[i].quantidade;
                soma.lipideos+= (ing_selecionados[i].selecionado->lipideos/MAX_ING) * ing_selecionados[i].quantidade;
                soma.colesterol+= (ing_selecionados[i].selecionado->colesterol/MAX_ING) * ing_selecionados[i].quantidade;
                soma.carboidrato+= (ing_selecionados[i].selecionado->carboidrato/MAX_ING) * ing_selecionados[i].quantidade;
                soma.fibra+= (ing_selecionados[i].selecionado->fibra/MAX_ING) * ing_selecionados[i].quantidade;
                soma.calcio+= (ing_selecionados[i].selecionado->calcio/MAX_ING) * ing_selecionados[i].quantidade;
                printf("[%i] %.1fg de %s\n", i+1, ing_selecionados[i].quantidade, ing_selecionados[i].selecionado->nome);
            }
            if(!num_selecionado){
                puts(menu_default);
            } else {
                puts(SEPARADOR_1);
                printf("Valor energético: %.2fkcal\nProteínas: %.2fg\nLipídeos: %.2fg\nColesterol: %.2fmg\nCarboidratos: %.2fmg\nFibras: %.2fmg\nCalcio: %.2fmg\n", 
                soma.kcal, 
                soma.proteina, 
                soma.lipideos,
                soma.colesterol,
                soma.carboidrato,
                soma.fibra,
                soma.calcio);
                puts(SEPARADOR_1);
            }
            printf("[%i/%i]", num_selecionado, MAX_ING);
            puts(SEPARADOR_2);
            printf("\n>>");
            fgets(INPUT_USUARIO, TAM_LINHA, stdin);
            fflush(stdin);
            INPUT_USUARIO[strlen(INPUT_USUARIO)-1] = '\0';
            // define o comando
            int coincidentes = 0;
            if(toupper(INPUT_USUARIO[0]) == 'A' && INPUT_USUARIO[1] == ' ' && num_selecionado <= MAX_ING){
                ingrediente* resultados[num_linhas];
                if(num_selecionado < MAX_ING){
                    for (int i = 0; i < num_linhas; i++)
                    {
                        char *coincidencia = strstr(ings[i].nome, INPUT_USUARIO+2);
                        if(coincidencia){
                            printf("[%i] %s\n", coincidentes+1, ings[i].nome);
                            resultados[coincidentes] = ings+i;
                            coincidentes++;
                        }
                    }
                    if(coincidentes){
                        char selecao[10];
                        printf("Selecione o item\n>>");
                        fgets(selecao, 10, stdin);
                        fflush(stdin);
                        if(atoi(selecao) > 0 && atoi(selecao) <= coincidentes){
                            if(strcmp(selecao, "\n")){
                                selecao[strlen(selecao)-1] = '\0';
                                ing_selecionados[num_selecionado].selecionado = resultados[atoi(selecao)-1];
                            }
                            printf("Quantidade (g): ");
                            scanf("%f", &ing_selecionados[num_selecionado].quantidade);
                            fflush(stdin);
                            num_selecionado++;
                        }
                    }
                    system("cls");
                } else {
                    system("cls");
                    puts("[!] Número máximo de ingredientes atingido. Remova um ingrediente ou conclua o programa.");
                }
            } else if(toupper(INPUT_USUARIO[0]) == 'R' && INPUT_USUARIO[1] == ' '){
                int selecionado = atoi(INPUT_USUARIO+2);
                if(selecionado>0 && num_selecionado > 0 && selecionado <= num_selecionado){
                    memmove(ing_selecionados+selecionado-1, ing_selecionados+selecionado, (MAX_ING-selecionado)*sizeof(ing_selecionados));
                    num_selecionado--;
                }
                system("cls");
            } else if(!strcmp(INPUT_USUARIO, "c") || !strcmp(INPUT_USUARIO, "C")){
                break;
            } else {
                system("cls");
            }
        }
}

int cont_linhas(FILE* fp)
{
    char dados[TAM_BUFFER];
    int cont = 0;
    while(1)
    {
        size_t tam_lido = fread(dados, 1, TAM_BUFFER, fp);
        int i;
        for(i = 0; i < tam_lido; i++){
            if (dados[i] == '\n'){
                cont++;
            }
        }
        if (feof(fp)){
            break;
        }
    }
    fseek(fp, 0, SEEK_SET);
    return cont;
}
