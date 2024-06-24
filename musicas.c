#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct artist {
    char artistname[100];
    char nacionality[100];
};

struct date {
    int day;
    int month;
    int year;
};

struct music {
    char name[100];
    int duration;
    char style[100];
    struct artist artist;
    struct date date;
};

void readFromFile(struct music **musics, int *size){
    FILE *f = fopen("cadastros.txt", "rt");
    
    int count = 0;

    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);
    fseek(f, 0, SEEK_SET);

    if(fileSize == 0){
        fclose(f);
        return;
    }

    *musics = malloc(sizeof(struct music));

    struct music musicHolder;

    char line[100];

    int lineCount = 0;
    
    while (fgets(line, sizeof(line), f)) {
        
        line[strcspn(line, "\n")] = '\0'; // Remove o caractere de nova linha
        
        switch (lineCount) {
            case 0:
                strcpy(musicHolder.name, line);
                break;
            case 1:
                musicHolder.duration = atoi(line);
                break;
            case 2:
                strcpy(musicHolder.style, line);
                break;
            case 3:
                strcpy(musicHolder.artist.artistname, line);
                break;
            case 4:
                strcpy(musicHolder.artist.nacionality, line);
                break;
            case 5:
                musicHolder.date.day = atoi(line);
                break;
            case 6:
                musicHolder.date.month = atoi(line);
                break;
            case 7:
                musicHolder.date.year = atoi(line);
                break;
        }
        
        lineCount++;

        if (lineCount == 8){
            lineCount = 0;
            *musics = realloc(*musics, sizeof(struct music) * (count+1));
            (*musics)[count] = musicHolder;
            count++;
        }
    }

    *size = count;
    fclose(f);

}

void writeInFile(struct music newMusic){
    FILE *f = fopen("cadastros.txt", "at");

    fprintf(f, "%s\n%d\n%s\n%s\n%s\n%d\n%d\n%d\n",
        newMusic.name,
        newMusic.duration,
        newMusic.style,
        newMusic.artist.artistname,
        newMusic.artist.nacionality,
        newMusic.date.day,
        newMusic.date.month,
        newMusic.date.year
    );

    fclose(f);
}

void reWriteFile(struct music **musics, int *size){
    
    FILE *f = fopen("cadastros.txt", "wt");

    for(int i = 0; i < *size; i++){
        fprintf(f, "%s\n%d\n%s\n%s\n%s\n%d\n%d\n%d\n",
            (*musics)[i].name,
            (*musics)[i].duration,
            (*musics)[i].style,
            (*musics)[i].artist.artistname,
            (*musics)[i].artist.nacionality,
            (*musics)[i].date.day,
            (*musics)[i].date.month,
            (*musics)[i].date.year
        );
    }

    fclose(f);

}

void callActions(int choice, struct music **musics, int *size);
void insert_music(struct music **musics, int *size);
void remove_music(struct music **musics, int *size, char* name);
void list_everymusic(struct music **musics, int *size);
void show_music_byname(struct music **musics, int *size, char* name);

int main (){

    int choice, size = 0;

    struct music *musics = NULL;

    readFromFile(&musics, &size);
    
    do {
        printf("\nSelecione alguma das acoes a seguir:\n");
        printf("1) Inserir novo registro\n");
        printf("2) Remover registro existente\n");
        printf("3) Listar todos os registros\n");
        printf("4) Consultar um registro pelo nome\n");
        printf("5) Sair\n");
        scanf("%d", &choice);
        fflush(stdin);
        if (choice > 0 && choice < 5) {
            callActions(choice, &musics, &size);
        }
    } while(choice > 0 && choice < 5);
    free(musics);
    return 1;
}

void callActions(int choice, struct music **musics, int *size){
    char removeMusicName[100];
    char showMusicName[100];
    switch (choice) {
        case 1:
            insert_music(musics, size);
            break;
        case 2:
            printf("\nInsira o nome da musica a qual deseja remover\n");
            gets(removeMusicName);
            remove_music(musics, size, removeMusicName);
            break;
        case 3:
            list_everymusic(musics, size);
            break;
        case 4:
            printf("\nInsira o nome da musica a qual deseja ver os dados\n");
            gets(showMusicName);
            show_music_byname(musics, size, showMusicName);
            break;
        default:
            break;
    }
}

int verify_primarykey(char* name, struct music **musics, int *size){
    for (int i = 0; i < *size; i++){
        if(strcmp(strupr((*musics)->name), strupr(name)) == 0){
            printf("Musica ja inserida, tente outra.\n");
            return 1;
        }
    }
    return 0;
}

void insert_music(struct music **musics, int *size){
    struct music newMusic;
    printf("\n------------Dados da Musica------------\n");
    do {
        printf("Insira o nome da musica:\n");
        gets(newMusic.name);
    } while(verify_primarykey(newMusic.name, musics, size));
    fflush(stdin);
    printf("Insira a duracao da musica (min):\n");
    scanf("%d", &newMusic.duration);
    fflush(stdin);
    printf("Insira o estilo da musica:\n");
    gets(newMusic.style);
    fflush(stdin);

    printf("\n------------Dados do Artista------------\n");
    printf("Insira o nome do artista:\n");
    gets(newMusic.artist.artistname);
    printf("Insira a nacionalidade do artista:\n");
    gets(newMusic.artist.nacionality);
    
    printf("\n------------Data de publicacao------------\n");
    printf("Insira o dia em que a musica foi publicada:\n");
    scanf("%d", &newMusic.date.day);
    printf("Insira o mes em que a musica foi publicada:\n");
    scanf("%d", &newMusic.date.month);
    printf("Insira o ano em que a musica foi publicada:\n");
    scanf("%d", &newMusic.date.year);

    (*size)++;
    *musics = realloc(*musics, sizeof(struct music) * (*size));
    (*musics)[*size - 1] = newMusic;
    writeInFile(newMusic);
}

void list_everymusic(struct music **musics, int *size){
    if(*size > 0){
        printf("\n%-23s %-19s %-15s %-6s\n", "Musica", "Artista", "Nacionalidade", "Cadastramento");
        printf("%-23s %-19s %-15s %-6s\n", "-----------------------", "-------------------", "---------------", "-------------");

        for(int i = 0; i < *size; i++){
            printf(
                "%-23s %-19s %-15s %d/%d/%d\n",
                (*musics)[i].name,
                (*musics)[i].artist.artistname, 
                (*musics)[i].artist.nacionality,
                (*musics)[i].date.day,
                (*musics)[i].date.month,
                (*musics)[i].date.year
            );
        }
        return;
    }
    printf("\nNao ha musicas registradas, insira uma e tente novamente.\n");
}

void remove_music(struct music **musics, int *size, char* name){
    
    struct music *newMusicArray;

    newMusicArray = malloc(sizeof(struct music) * (*size - 1));

    int musicIndex = -1;

    for(int i = 0; i < *size; i++){
        if(strcmp(strupr((*musics)[i].name), strupr(name)) == 0){
            musicIndex = i;
        }
    }

    if(musicIndex == -1){
        printf("\nEsta musica nao esta cadastrada.\n");
        free(newMusicArray);
        return;
    }

    int newIndex = 0;
    for (int i = 0; i < *size; i++) {
        if (i != musicIndex) {
            newMusicArray[newIndex] = (*musics)[i];
            newIndex++;
        }
    }

    (*size)--;

    *musics = realloc(*musics, sizeof(struct music) * (*size));

    *musics = newMusicArray;

    free(newMusicArray);

    reWriteFile(musics, size);

    printf("\nMusica %s removida.\n", name);
}

void show_music_byname(struct music **musics, int *size, char* name){

    if(*size > 0){
        printf("\n%-23s %-19s %-15s %-6s\n", "Musica", "Artista", "Nacionalidade", "Cadastramento");
        printf("%-23s %-19s %-15s %-6s\n", "-----------------------", "-------------------", "---------------", "-------------");

        for(int i = 0; i < *size; i++){
            if(strcmp(strupr((*musics)[i].name), strupr(name)) == 0){
                printf(
                    "%-23s %-19s %-15s %d/%d/%d\n",
                    (*musics)[i].name,
                    (*musics)[i].artist.artistname, 
                    (*musics)[i].artist.nacionality,
                    (*musics)[i].date.day,
                    (*musics)[i].date.month,
                    (*musics)[i].date.year
                );
            }
        }
        return;
    }
    printf("\nMusica nao encontrada.\n");
}