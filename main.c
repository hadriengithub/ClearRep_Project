#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_NAME_CHAR 20
#define MAX_TEL_CHAR 12
#define REP_SIZE 3

typedef struct person
{
	char name[MAX_NAME_CHAR];
	char tel[MAX_TEL_CHAR];
}pers;

void input_person(pers *sp);
void input_rep(void);
void print_rep(void);
int find_in_rep(void);
void save_rep(void);
int load_rep(void);
int get_file_size(FILE *file);

void add_profil(void); // fonction ultime

pers *myRep;
size_t rep_size=1;

int main(int argc, char *argv[])
{
	bool load = false;
	int choose, s1, s2, size;
	FILE *tryfindfile = fopen("save.data", "r");
	if(tryfindfile==NULL)
	{
		printf("Echec de la recherche du fichier data\n");
		input_rep();
		myRep = calloc(rep_size, sizeof(pers));
	}
	else
		load = true;
	if(load==true)
	{
		rep_size=get_file_size(tryfindfile);
		myRep = calloc(rep_size, sizeof(pers));
		load_rep();
	}
	print_rep();
	do
	{
		do
		{
			printf("\nMenu:\n");
			printf("1-afficher le contenu du répertoire\n");
			printf("2-chercher le numéro d'une personne\n");
			printf("3-sauvegarder le répértoire\n");
			printf("4-ajouter un profil\n");
			printf("5-quitter le programme\n");
			printf("Votre choix : ");
			scanf("%d", &choose);
		}
		while(choose < 1 || choose > 5);
		switch(choose)
		{
			case 1:
				print_rep();
				break;
			case 2:
				if(find_in_rep()==0)
					printf("ERREUR : NOM INTROUVABLE\n");
				break;
			case 3:
				save_rep();
				break;
			case 4:
				add_profil();
				break;
			default: break;
		}
	}
	while(choose!=5);
	save_rep();
	return 1;
}

void input_person(pers *sp)
{
	printf("Name : ");
	scanf("%s", sp->name);
	printf("Tel : ");
	scanf("%s", sp->tel);
	return;
}

void input_rep(void)
{
	int i;
	printf("Veuillez saisir votre premier contact :\n");
	input_person(&myRep[0]);
	printf("Rep saisit avec succe!\n");
	return;
}

void print_rep(void)
{
	printf("Lancement de l'affichage du rep\n");
	int i;
	for(i=0;i<rep_size;i++)
	{
		printf("-------------------------------------\n");
		printf("Name : %s\n", myRep[i].name);
		printf("Tel : %s\n", myRep[i].tel);
		printf("-------------------------------------\n");
	}
	printf("Fin de l'affichage du repertoire\n");
	return;
}

int find_in_rep(void)
{
	int i;
	char name_to_find[MAX_NAME_CHAR];
	printf("Outil de recherche dans le rep :\n");
	printf("Entrer un nom :");
	scanf("%s", name_to_find);
	printf("\n");
	for(i=0;i<rep_size;i++)
	{
		if(strcmp(name_to_find, myRep[i].name)==0)
		{
			printf("Trouvé!\n");
			printf("Num : %s", myRep[i].tel);
			return 1;
		}
	}
	return 0;
}

void save_rep(void)
{
	FILE *save = fopen("save.data", "w");
	int i;
	for(i=0;i<rep_size;i++)
	{
		fputs(myRep[i].name, save);
		fputc(' ', save);
		fputs(myRep[i].tel, save);
		fputc(' ', save);
		fputc('\n', save);
		printf("Profil %d enregistrer dans le fichier data.\n", i+1);
	}
	fclose(save);
	return;
}

int load_rep(void)
{
	int i;
	signed char n_name[MAX_NAME_CHAR], n_tel[MAX_TEL_CHAR];
	FILE *save = fopen("save.data", "r");
	if(save==NULL)
		return 1;
	printf("size : %d\n", rep_size);
	printf("Fichier data trouvée!\n");
	printf("Lancement de la récupération des données\n");
	for(i=0;i<rep_size;i++)
	{
		fscanf(save, "%s %s ", myRep[i].name, myRep[i].tel);
		printf("NAME : %s | TEL : %s\n", myRep[i].name, myRep[i].tel);
	}
	printf("Données récupérer avec succée\n");
	fclose(save);
	return 0;
}

int get_file_size(FILE *file)
{
	int size=0;
	signed char str[MAX_NAME_CHAR+MAX_TEL_CHAR];
	while(fgets(str, MAX_NAME_CHAR+MAX_TEL_CHAR-1, file)!=NULL)
		size++;
	return size;
}

void add_profil(void)
{
	signed char new_name[MAX_NAME_CHAR];
	signed char new_tel[MAX_TEL_CHAR];
	printf("Veuillez entrer le nom du nouveau profil : ");
	scanf("%s", new_name);
	printf("Veuillez entrer le tel du nouveau profil : ");
	scanf("%s", new_tel);
	myRep = realloc(myRep, (rep_size+1)*sizeof(pers));
	++rep_size;
	strcpy(myRep[rep_size-1].name, new_name);
	strcpy(myRep[rep_size-1].tel, new_tel);
	print_rep();
	return;
}
