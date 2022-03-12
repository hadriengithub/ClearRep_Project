#include <gtk/gtk.h>
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
	int index;
}pers;

void input_person(pers *sp);
void input_rep(void);
void print_rep(void);
int find_in_rep(void);
void save_rep(void);
int load_rep(void);
int get_file_size(FILE *file);
void delete_row(void);
void remove_from_list(void);
void remove_from_prog(void);
void delete_user(int n);
void actualize_index_of_rep();

void add_profil(void); // fonction ultime

void add_research_result(signed char *result);

pers *myRep;
int rep_size=1;

GtkApplication *application;
int status;
GtkWidget *window;
GtkWidget *hbox;
GtkWidget *scrollbar;

GtkListStore *store;
GtkWidget *tree_view;
GtkCellRenderer *renderer;
GtkTreeViewColumn *column;

// add popup assets
GtkWidget *add_entry1,*add_entry2;
GtkWidget *validate_add_button;

// research popup assets
GtkWidget *popup_research_window;
GtkWidget *research_entry;
GtkWidget *vbox;

// delete row stuff
GtkTreeModel *model;
GtkTreeSelection *selection;

void activate_print_list(void);
void refresh_list(void);
void activate_add_popup(void);
void activate_research_popup(void);
void if_delete_button(GtkWidget *widget, gpointer data);
void if_research_button(GtkWidget *widget, gpointer data);
void if_save_button(GtkWidget *widget, gpointer data);
void if_add_button(GtkWidget *widget, gpointer data);
void if_validate_add_button(GtkWidget *widget, gpointer data);
void if_validate_research_button(GtkWidget *widget, gpointer data);

void activate_app(GtkApplication *application, gpointer data)
{
  printf("Activate !\n");
  GtkWidget *grid;
  GtkWidget *delete_button,*research_button,*save_button,*add_button;
  GtkWidget *entry;

  window=gtk_application_window_new(application);

  hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
  gtk_container_add(GTK_CONTAINER(window),hbox);

  grid=gtk_grid_new();
  gtk_box_pack_start(GTK_BOX(hbox),grid,FALSE,TRUE,0);

  delete_button=gtk_button_new_with_label("delete_button");
  research_button=gtk_button_new_with_label("research_button");
  save_button=gtk_button_new_with_label("save_button");
  add_button=gtk_button_new_with_label("add_button");
  gtk_grid_attach(GTK_GRID(grid),delete_button,1,1,1,1);
  gtk_grid_attach(GTK_GRID(grid),research_button,2,1,1,1);
  gtk_grid_attach(GTK_GRID(grid),save_button,1,2,1,1);
  gtk_grid_attach(GTK_GRID(grid),add_button,2,2,1,1);

  entry=gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid),entry,1,3,2,1);

  g_signal_connect(window,"destroy",gtk_main_quit,NULL);
  g_signal_connect(delete_button,"clicked",G_CALLBACK(if_delete_button),NULL);
  g_signal_connect(research_button,"clicked",G_CALLBACK(if_research_button),NULL);
  g_signal_connect(save_button,"clicked",G_CALLBACK(if_save_button),NULL);
  g_signal_connect(add_button,"clicked",G_CALLBACK(if_add_button),NULL);

  scrollbar=gtk_scrolled_window_new(NULL,NULL);
  gtk_box_pack_start(GTK_BOX(hbox),scrollbar,FALSE,TRUE,0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar),GTK_POLICY_NEVER,GTK_POLICY_ALWAYS);

  activate_print_list();

  gtk_widget_show_all(window);
  gtk_main();
  return;
}

int main(int argc,char *argv[])
{
  printf("Hello World !\n");

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

  application=gtk_application_new(NULL,G_APPLICATION_FLAGS_NONE);
  g_signal_connect(application,"activate",G_CALLBACK(activate_app),NULL);
  status=g_application_run(G_APPLICATION(application),argc,argv);

  return 1;
}

void if_delete_button(GtkWidget *widget, gpointer data)
{
  printf("Delete !\n");
  delete_row();
  return;
}
void if_research_button(GtkWidget *widget, gpointer data)
{
  printf("Research !\n");
  activate_research_popup();
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
  printf("Add !\n");
	activate_add_popup();
  return;
}

void activate_print_list(void)
{
  int i;
	GtkTreeIter iter;
  store=gtk_list_store_new(3,G_TYPE_STRING,G_TYPE_STRING, G_TYPE_INT);
  for(i=0;i<rep_size;i++)
  {
    gtk_list_store_append(store,&iter);
    gtk_list_store_set(store,&iter,0,myRep[i].name,1,myRep[i].tel,2,myRep[i].index,-1);
  }
  tree_view=gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  renderer=gtk_cell_renderer_text_new();
	column=gtk_tree_view_column_new_with_attributes("Index",renderer,"text",2,NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view),column);
  column=gtk_tree_view_column_new_with_attributes("Nom",renderer,"text",0,NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view),column);
  column=gtk_tree_view_column_new_with_attributes("Tel",renderer,"text",1,NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view),column);
  gtk_container_add(GTK_CONTAINER(scrollbar),tree_view);

  return;
}

void refresh_list(void)
{
	GtkTreeIter iter;
  printf("Refresh !\n");
  gtk_list_store_insert(store,&iter,-1);
  gtk_list_store_set(store,&iter,0,myRep[rep_size-1].name,1,myRep[rep_size-1].tel,-1);

  tree_view=gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  renderer=gtk_cell_renderer_text_new();
  column=gtk_tree_view_column_new_with_attributes("Nom",renderer,"text",0,NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view),column);
  column=gtk_tree_view_column_new_with_attributes("Tel",renderer,"text",1,NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view),column);
  gtk_box_pack_start(GTK_BOX(hbox),tree_view,FALSE,TRUE,0);
  return;
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
	const signed char *name_to_find;
	name_to_find=gtk_entry_get_text(GTK_ENTRY(research_entry));
	for(i=0;i<rep_size;i++)
	{
		if(strcmp(name_to_find, myRep[i].name)==0)
		{
			printf("Trouvé!\n");
			printf("Num : %s\n", myRep[i].tel);
			add_research_result(myRep[i].tel);
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
		myRep[i].index=i;
		printf("NAME : %s | TEL : %s | INDEX : %d\n", myRep[i].name, myRep[i].tel, myRep[i].index);
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
	const signed char *new_name, *new_tel;
	new_name=gtk_entry_get_text(GTK_ENTRY(add_entry1));
	new_tel=gtk_entry_get_text(GTK_ENTRY(add_entry2));
	myRep = realloc(myRep, (rep_size+1)*sizeof(pers));
	++rep_size;
	strcpy(myRep[rep_size-1].name, new_name);
	strcpy(myRep[rep_size-1].tel, new_tel);
	print_rep();
  refresh_list();
	return;
}

void activate_add_popup(void)
{
	printf("Add Popup !\n");
	GtkWidget *popup_add_window=gtk_application_window_new(application);
	GtkWidget *add_grid=gtk_grid_new();
	GtkWidget *add_label1,*add_label2;

	validate_add_button=gtk_button_new_with_label("add");
	g_signal_connect(validate_add_button,"clicked",G_CALLBACK(if_validate_add_button),NULL);

	add_label1=gtk_label_new("Nom : ");
	add_label2=gtk_label_new("Tel : ");
	add_entry1=gtk_entry_new();
	add_entry2=gtk_entry_new();

	gtk_container_add(GTK_CONTAINER(popup_add_window),add_grid);

	gtk_grid_attach(GTK_GRID(add_grid),add_label1,1,1,1,1);
	gtk_grid_attach(GTK_GRID(add_grid),add_label2,2,1,1,1);
	gtk_grid_attach(GTK_GRID(add_grid),add_entry1,1,2,1,1);
	gtk_grid_attach(GTK_GRID(add_grid),add_entry2,2,2,1,1);
	gtk_grid_attach(GTK_GRID(add_grid),validate_add_button,1,3,2,1);

	gtk_widget_show_all(popup_add_window);
}

void if_validate_add_button(GtkWidget *widget, gpointer data)
{
	printf("Validate !\n");
	add_profil();
	return;
}

void activate_research_popup(void)
{
	popup_research_window=gtk_application_window_new(application);
	vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	GtkWidget *research_grid=gtk_grid_new();
	GtkWidget *research_label=gtk_label_new("Veuillez entrer un nom");
	research_entry=gtk_entry_new();
	GtkWidget *validate_research_button=gtk_button_new_with_label("research");
	g_signal_connect(validate_research_button,"clicked",G_CALLBACK(if_validate_research_button),NULL);
	gtk_container_add(GTK_CONTAINER(popup_research_window),vbox);
	gtk_container_add(GTK_CONTAINER(vbox),research_grid);
	gtk_grid_attach(GTK_GRID(research_grid),research_label,1,1,1,1);
	gtk_grid_attach(GTK_GRID(research_grid),research_entry,2,1,1,1);
	gtk_grid_attach(GTK_GRID(research_grid),validate_research_button,1,2,2,1);
	gtk_widget_show_all(popup_research_window);
	return;
}

void if_validate_research_button(GtkWidget *widget, gpointer data)
{
	printf("Validate Research !\n");
	find_in_rep();
	return;
}

void add_research_result(signed char *result)
{
	printf("Add research result to tab\n");
	GtkWidget *research_result_grid=gtk_grid_new();
	GtkWidget *result_label1, *result_label2;
	result_label1=gtk_label_new("result:");
	result_label2=gtk_label_new(result);
	gtk_box_pack_start(GTK_BOX(vbox),research_result_grid, FALSE,TRUE,0);
	gtk_grid_attach(GTK_GRID(research_result_grid),result_label1,1,1,1,1);
	gtk_grid_attach(GTK_GRID(research_result_grid),result_label2,2,1,1,1);
	gtk_widget_show_all(popup_research_window);
	return;
}

void delete_row(void)
{
	remove_from_prog();
	remove_from_list();
	return;
}

void remove_from_list(void)
{
	// get selection row, remove selected row, set index;
	GtkTreeIter iter;
	selection=gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
	model=gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));
	gtk_tree_selection_get_selected(selection,&model,&iter);

	gtk_list_store_remove(store,&iter);

	return;
}
void remove_from_prog(void)
{
	GtkTreeIter iter;
	int i,rn;
	selection=gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
	model=gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));
	gtk_tree_selection_get_selected(selection,&model,&iter);
	gtk_tree_model_get(model,&iter,2,&rn,-1);
	printf("result:%d\n",rn);
	FILE *save=fopen("save.data","w");
	for(i=0;i<rep_size;i++)
	{
		if(i!=rn)
		{
			fputs(myRep[i].name, save);
			fputc(' ', save);
			fputs(myRep[i].tel, save);
			fputc(' ', save);
			fputc('\n', save);
			printf("Profil %d enregistrer dans le fichier data.\n", i+1);
		}
	}
	rep_size--;
  actualize_index_of_rep();
	fclose(save);
	load_rep();
	return;
}

void actualize_index_of_rep()
{
	int i;
	GtkTreeIter iter;
	for(i=0;i<rep_size;i++)
		myRep[i].index=i;
		gtk_list_store_clear(store);
	for(i=0;i<rep_size;i++)
	{
		gtk_list_store_append(store,&iter);
		gtk_list_store_set(store,&iter,0,myRep[i].name,1,myRep[i].tel,2,myRep[i].index,-1);
	}
	return;
}


/*
gcc main.c -o prog `pkg-config --cflags --libs gtk+-3.0`
*/
