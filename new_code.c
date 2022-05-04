#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#define PROFIL_NAME_MAX_LENGTH 20
#define PROFIL_NUM_MAX_LENGTH 20

typedef struct profil
{
  signed char name[PROFIL_NAME_MAX_LENGTH];
  signed char num[PROFIL_NUM_MAX_LENGTH];
} prf;

// prototype
void print_rep(void);
int find_index_by_name(void);

prf *myRep;
int size=0, i;
int launch_gtk=1;



void find_in_rep(void)
{
  printf("--- Lancement du programme de recherche ---\n");
  signed char profil_name_to_find[PROFIL_NAME_MAX_LENGTH];
  printf("Veuillez entrer le nom du profil rechercher : ");
  scanf("%s",profil_name_to_find);
  for(i=1;i<=size;i++)
  {
    if(strcmp(myRep[i-1].name, profil_name_to_find)==0)
    {
        printf("Name find !\n");
        printf("Num : %s\n", myRep[i-1].num);
    }
  }
  printf("--- Fin du programme de recherche ---\n");
  return;
}

void add_profil(void)
{
  size++;
  myRep=realloc(myRep,sizeof(prf)*size);
  signed char str_name[PROFIL_NAME_MAX_LENGTH], str_num[PROFIL_NUM_MAX_LENGTH];
  printf("nom : ");
  scanf("%s",str_name);
  printf("num : ");
  scanf("%s",str_num);
  strcpy(myRep[size-1].name, str_name);
  strcpy(myRep[size-1].num, str_num);
  print_rep();
  return;
}

int get_file_size(void)
{
  int file_size=0;
  FILE *file=fopen("save.data","r");
  if(file==NULL)
  {
    printf("ERROR : no data file found !\n");
    exit(EXIT_FAILURE);
  }
  signed char c;
  for (c = getc(file); c != EOF; c = getc(file))
        if (c == '\n') // Increment count if this character is newline
            file_size++;
  fclose(file);
  return file_size;
}

void load_rep(void)
{
  FILE *file=fopen("save.data", "r");
  if(file==NULL)
  {
    printf("--- Aucun fichier de data ---\n");
    size=1;
    myRep=malloc(sizeof(prf));
    printf("Veuillez entrer le premier profil de votre nouveau repertoire !\n");
    printf("nom : ");
    scanf("%s", myRep[0].name);
    printf("num : ");
    scanf("%s", myRep[0].num);
    printf("--- Fin de la saisie du nouveau profil ---\n");
  }
  else
  {
    printf("--- Lancement de la recuperation des donénes ...\n");
    size=get_file_size()/2;
    myRep=malloc(sizeof(prf)*size);
    for(i=1;i<=size;i++)
    {
      printf("--- Recup du profil %d\n", i);
      fscanf(file, "%s\n", myRep[i-1].name);
      fscanf(file, "%s\n", myRep[i-1].num);
    }
  };
  printf("size : %d\n", size);
  print_rep();
  return;
}

void save_rep(void)
{
  printf("--- Lancement de la sauvegarde du rep ---\n");
  FILE *file=fopen("save.data","w");
  for(i=1;i<=size;i++)
  {
    printf("--- Sauvegarde du profil %d ---\n", i);
    fprintf(file,"%s\n",myRep[i-1].name);
    fprintf(file,"%s\n",myRep[i-1].num);
  }
  fclose(file);
  return;
}

void print_rep(void)
{
  printf("--- Lancement de l'affichage du rep ---\n");
  for(i=1;i<=size;i++)
  {
    printf("--- Lancement de l'affichage du profil %d ---\n", i);
    printf("name : %s\n", myRep[i-1].name);
    printf("num : %s\n", myRep[i-1].num);
    printf("--- Fin de l'affichage du profil --- \n");
  }
  printf("--- Fin de l'affichage du rep ---\n");
  return;
}

void quit_application(GtkWidget *widget, gpointer data)
{
  printf("Quit Application !\n");
  gtk_main_quit();
  return;
}

void if_add_button(GtkWidget *widget, gpointer data)
{
  printf("add button !\n");
  add_profil();
  return;
}

void delete_in_rep(void)
{
  int index_to_del=find_index_by_name();
  if(index_to_del==0)
    return;
  size--;
  printf("size : %d\n",size);
  for (i = index_to_del - 1; i < size -1; i++)
  {
    printf("IIII\n");
    strcpy(myRep[i].name, myRep[i+1].name);
    strcpy(myRep[i].num, myRep[i+1].num);
  }
  print_rep();
  return;
}

void if_delete_button(GtkWidget *widget, gpointer data)
{
  printf("delete button\n");
  delete_in_rep();
  return;
}

void if_research_button(GtkWidget *widget, gpointer data)
{
  printf("research button !\n");
  find_in_rep();
  return;
}

void launch_application(GtkApplication *application, gpointer data)
{
  GtkWidget *window=gtk_application_window_new(application);
  gtk_window_set_title(GTK_WINDOW(window),"default_title");
  gtk_window_set_default_size(GTK_WINDOW(window),200,200);
  g_signal_connect(window,"destroy",G_CALLBACK(quit_application),NULL);
  GtkWidget *vbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  gtk_container_add(GTK_CONTAINER(window),vbox);
  GtkWidget *button_add, *button_delete, *button_research;
  button_add=gtk_button_new_with_label("Add");
  button_delete=gtk_button_new_with_label("Delete");
  button_research=gtk_button_new_with_label("Research");
  gtk_box_pack_start(GTK_BOX(vbox),button_add,FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox),button_delete,FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox),button_research,FALSE, TRUE, 0);
  g_signal_connect(button_add,"clicked", G_CALLBACK(if_add_button), NULL);
  g_signal_connect(button_delete,"clicked",G_CALLBACK(if_delete_button),NULL);
  g_signal_connect(button_research,"clicked",G_CALLBACK(if_research_button),NULL);
  gtk_widget_show_all(window);
  gtk_main();
  return;
}
int find_index_by_name(void)
{
  int index;
  printf("Veuillez entrer le nom : ");
  signed char name_to_find[PROFIL_NAME_MAX_LENGTH];
  scanf("%s",name_to_find);
  for(i=1;i<=size;i++)
  {
    if(strcmp(name_to_find,myRep[i-1].name)==0)
    {
      printf("User find !\n");
      printf("num : %s\n",myRep[i-1].num);
      printf("index : %d\n",i);
      return index;
    }
  }
  printf("Aucun profil trouver !\n");
  return 0;
}



int main(int argc, char *argv[])
{
  printf("Hello World !\n");
  load_rep();
  if(launch_gtk==1)
  {
    GtkApplication *application=gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
    g_signal_connect(application,"activate",G_CALLBACK(launch_application),NULL);
    g_application_run(G_APPLICATION(application),argc,argv);
  }
  save_rep();
  return 1;
}


/*
gcc main.c -o prog `pkg-config --cflags --libs gtk+-3.0`
*/
