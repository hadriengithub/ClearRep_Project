#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
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

GtkBuilder *builder;
pers *myRep;
int rep_size=1;

void if_print_button(GtkWidget *widget, gpointer data);
void if_research_button(GtkWidget *widget, gpointer data);
void if_save_button(GtkWidget *widget, gpointer data);
void if_add_button(GtkWidget *widget, gpointer data);

void input_person(pers *sp);
void input_rep(void);
void print_rep(void);
int find_in_rep(void);
void save_rep(void);
int load_rep(void);
int get_file_size(FILE *file);

void add_profil(void); // fonction ultime

void active_printed_list(void);

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

  gtk_init(&argc,&argv);

  GtkWidget *window;
  GtkWidget *print_button;
  GtkWidget *research_button;
  GtkWidget *save_button;
  GtkWidget *add_button;
  GtkWidget *hbox;

  builder=gtk_builder_new();
  if(gtk_builder_add_from_file(builder,"maquette.glade",NULL)==0)
  {
    fprintf(stderr,"ERROR\n");
    exit(EXIT_FAILURE);
  }
  gtk_builder_connect_signals(builder,NULL);

  active_printed_list();

  window=GTK_WIDGET(gtk_builder_get_object(builder,"window"));
  gtk_window_set_title(GTK_WINDOW(window),"default");
  gtk_window_set_default_size(GTK_WINDOW(window),800,600);
  g_signal_connect(window,"destroy",gtk_main_quit,NULL);

  print_button=GTK_WIDGET(gtk_builder_get_object(builder,"print_button"));
  research_button=GTK_WIDGET(gtk_builder_get_object(builder,"research_button"));
  save_button=GTK_WIDGET(gtk_builder_get_object(builder,"save_button"));
  add_button=GTK_WIDGET(gtk_builder_get_object(builder,"add_button"));
  g_signal_connect(print_button,"clicked",G_CALLBACK(if_print_button),NULL);
  g_signal_connect(research_button,"clicked",G_CALLBACK(if_research_button),NULL);
  g_signal_connect(save_button,"clicked",G_CALLBACK(if_save_button),NULL);
  g_signal_connect(add_button,"clicked",G_CALLBACK(if_add_button),NULL);

  gtk_widget_show_all(window);

  gtk_main();
  g_object_unref(builder);
  return 1;
}

void if_print_button(GtkWidget *widget, gpointer data)
{
  printf("Print !\n");
  print_rep();
  return;
}

void if_research_button(GtkWidget *widget, gpointer data)
{
  printf("Research !\n");
  find_in_rep();
  return;
}

void if_save_button(GtkWidget *widget, gpointer data)
{
  printf("Save !\n");
  save_rep();
  return;
}

void if_add_button(GtkWidget *widget, gpointer data)
{
  add_profil();
  printf("Add !\n");
  return;
}

/*
gcc main.c -o prog `pkg-config --cflags --libs gtk+-3.0`
*/

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
			printf("Num : %s\n", myRep[i].tel);
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

void active_printed_list(void)
{
  int i;
  GtkWidget *hbox;

  GtkListStore *store;
  GtkTreeIter iter;
  GtkWidget *tree_view;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;

  hbox=GTK_WIDGET(gtk_builder_get_object(builder,"hbox"));

  store=gtk_list_store_new(2,G_TYPE_STRING,G_TYPE_STRING);
  for(i=0;i<rep_size;i++)
  {
    gtk_list_store_append(store,&iter);
    gtk_list_store_set(store,&iter,0,myRep[i].name,1,myRep[i].tel,-1);
  }
  tree_view=gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  renderer=gtk_cell_renderer_text_new();
  column=gtk_tree_view_column_new_with_attributes("Nom",renderer,"text",0,NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view),column);
  column=gtk_tree_view_column_new_with_attributes("Tel",renderer,"text",1,NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view),column);
  gtk_box_pack_start(GTK_BOX(hbox),tree_view,FALSE,TRUE,0);

  return;
}
















/**/
