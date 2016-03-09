#include <gtk/gtk.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <argz.h>
#include <wctype.h>
#include "editor.h"
#include "word_list.h"
#include "dictionary.h"

static int make_lowercase(wchar_t *word)
{
    for (wchar_t *w = word; *w; ++w)
        if (!iswalpha(*w))
            return 0;
        else
            *w = towlower(*w);
    return 1;
}

void show_about () {
  GtkWidget *dialog = gtk_about_dialog_new();

  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "Text Editor");
  //gtk_window_set_title(GTK_WINDOW(dialog), "About Text Editor");
  
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), 
     "Text Editor for IPP exercises\n");
    
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}

void show_help (void) {
  GtkWidget *help_window;
  GtkWidget *label;
  char help[5000];

  help_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW (help_window), "Help - Text Editor");
  gtk_window_set_default_size(GTK_WINDOW(help_window), 300, 300);
 
  strcpy(help,
         "\nAby podłączyć usługę spell-checkera do programu trzeba:\n\n"
         "Dołączyć ją do menu 'Spell' w menubar.\n\n"
         "Pobrać zawartość bufora tekstu z edytora: całą lub fragment,\n"
         "  zapamiętując pozycję.\n\n");
  strcat(help, "\0");

  label = gtk_label_new(help);
    
  gtk_container_add(GTK_CONTAINER(help_window), label); 

  gtk_widget_show_all(help_window);
}

static void WhatCheck (GtkMenuItem *item, gpointer data) {

  GtkWidget *dialog;
  GtkTextIter start, end;
  char *word;
  gunichar *wword; 
  if (dict == NULL)
  {
    dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_OK,
                                    "Nie wybrano słownika.\nNajpierw wybierz słownik.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  // Znajdujemy pozycję kursora
  gtk_text_buffer_get_iter_at_mark(editor_buf, &start,
                                   gtk_text_buffer_get_insert(editor_buf));

  // Jeśli nie wewnątrz słowa, kończymy
  if (!gtk_text_iter_inside_word(&start)) {
    dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_OK,
                                    "Kursor musi być w środku słowa");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }

  // Znajdujemy początek i koniec słowa, a potem samo słowo 
  end = start;
  gtk_text_iter_backward_word_start(&start);
  gtk_text_iter_forward_word_end(&end); 
  word = gtk_text_iter_get_text(&start, &end);

  // Zamieniamy na wide char (no prawie)
  wword = g_utf8_to_ucs4_fast(word, -1, NULL);

  // Sprawdzamy
  wchar_t * wwword = (wchar_t *) wword;
  int a = make_lowercase(wwword);
  if (dictionary_find(dict, wwword)) {
    dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                    "Wszystko w porządku,\nśpij spokojnie");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }
  else 
  {
    // Czas korekty
    GtkWidget *vbox, *label, *combo;

    struct word_list hints;
    int i;
    wchar_t **words;

    dictionary_hints(dict, (wchar_t *)wword, &hints);
    words = word_list_get(&hints);
    dialog = gtk_dialog_new_with_buttons("Korekta", NULL, 0, 
                                         GTK_STOCK_OK,
                                         GTK_RESPONSE_ACCEPT,
                                         GTK_STOCK_CANCEL,
                                         GTK_RESPONSE_REJECT,
                                         NULL);
    // W treści dialogu dwa elementy
    vbox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    // Tekst
    label = gtk_label_new("Coś nie tak, mam kilka propozycji");
    gtk_widget_show(label);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
    
    combo = gtk_combo_box_text_new();    
    for (i = 0; i < word_list_size(&hints); i++) {
      char *uword = g_ucs4_to_utf8((gunichar *)words[i], -1, NULL, NULL, NULL);
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), uword);
      g_free(uword);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
    gtk_box_pack_start(GTK_BOX(vbox), combo, FALSE, FALSE, 1);
    gtk_widget_show(combo);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
      char *korekta =
        gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));
      gtk_text_buffer_delete(editor_buf, &start, &end);
      gtk_text_buffer_insert(editor_buf, &start, korekta, -1);
      g_free(korekta);
      gtk_widget_destroy(dialog);
    }
    
    else {
      gtk_widget_destroy(dialog);
      GtkWidget *vbox2, *label2, *dialog2;
      dialog2 = gtk_dialog_new_with_buttons("Dodawanie słowa", NULL, 0, 
                                         GTK_STOCK_OK,
                                         GTK_RESPONSE_ACCEPT,
                                         GTK_STOCK_CANCEL,
                                         GTK_RESPONSE_REJECT,
                                         NULL);
    
      vbox2 = gtk_dialog_get_content_area(GTK_DIALOG(dialog2));
      label2 = gtk_label_new("Czy chcesz może dodać to słowo do słownika?");
      gtk_widget_show(label2);
      gtk_box_pack_start(GTK_BOX(vbox2), label2, FALSE, FALSE, 1);      
      gtk_dialog_run(GTK_DIALOG(dialog2));
      
      if (gtk_dialog_run(GTK_DIALOG(dialog2)) == GTK_RESPONSE_ACCEPT){        
        dictionary_insert(dict, (wchar_t *) wword);                      
        dictionary_save_lang(dict, nazwaSlownika);
      }
      gtk_widget_destroy(dialog2);
    }    
  }
  g_free(word);
  g_free(wword);
}

static void AddDictionary (GtkMenuItem *item, gpointer data)
{
  GtkWidget *dialog, *vbox, *label, *combo;
  if (dict != NULL)
    dictionary_done(dict);
  
  dialog = gtk_dialog_new_with_buttons("Wybór", NULL, 0, 
                                         GTK_STOCK_OK,
                                         GTK_RESPONSE_ACCEPT,
                                         GTK_STOCK_CANCEL,
                                         GTK_RESPONSE_REJECT,
                                         NULL);  
  label = gtk_label_new("Wybierz słownik");
  gtk_widget_show(label);
  vbox = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 1);
  combo = gtk_combo_box_text_new();    
  char *lang_list;
  size_t list_len;
  if (dictionary_lang_list(&lang_list, &list_len) == 0)
  {
    char *entry = 0;
    while ((entry = argz_next(lang_list, list_len, entry)))            
      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), entry);
  }  
  gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
  gtk_box_pack_start(GTK_BOX(vbox), combo, FALSE, FALSE, 1);
  gtk_widget_show(combo);
  gtk_dialog_run(GTK_DIALOG(dialog));
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *korekta =
      gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));
    dict = dictionary_load_lang(korekta);
    nazwaSlownika = korekta;
  }
  gtk_widget_destroy(dialog);
}

static void HighlightMissing (GtkMenuItem *item, gpointer data)
{
  gunichar *wword;   
  GtkWidget * dialog;
  if (dict == NULL)
  {
    dialog = gtk_message_dialog_new(NULL, 0, GTK_MESSAGE_ERROR,
                                    GTK_BUTTONS_OK,
                                    "Nie wybrano słownika.\nNajpierw wybierz słownik.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  
  GtkTextBuffer *buffer = editor_buf;
  
  GtkTextIter start, end;
  if (gtk_text_tag_table_lookup(gtk_text_buffer_get_tag_table(buffer), "red_fg") == NULL)
    gtk_text_buffer_create_tag(buffer, "red_fg", 
                             "foreground", "red", 
                             "weight", PANGO_WEIGHT_BOLD, NULL);
  

  gtk_text_buffer_get_end_iter(buffer, &end);

  char *word;
  while(gtk_text_iter_backward_word_start(&end))
  {          
    start = end;
    gtk_text_iter_forward_word_end(&start); 
    word = gtk_text_iter_get_text(&start, &end);
    wword = g_utf8_to_ucs4_fast(word, -1, NULL);
    wchar_t * wwword = (wchar_t *) wword;
    int a = make_lowercase(wwword);
    if (!dictionary_find(dict, wwword))
      gtk_text_buffer_apply_tag_by_name(buffer, "red_fg", &start, &end); 
    else
      gtk_text_buffer_remove_tag_by_name(buffer, "red_fg", &start, &end);     
    g_free(word);  
  }
}

void extend_menu (GtkWidget *menubar) {
  GtkWidget *spell_menu_item, *spell_menu, *check_item, *add_dictionary, 
    *highlight_missing;

  spell_menu_item = gtk_menu_item_new_with_label("Spell");
  spell_menu = gtk_menu_new();
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(spell_menu_item), spell_menu);
  gtk_widget_show(spell_menu_item);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), spell_menu_item);

  check_item = gtk_menu_item_new_with_label("Check Word");
  g_signal_connect(G_OBJECT(check_item), "activate", 
                   G_CALLBACK(WhatCheck), NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(spell_menu), check_item);
  gtk_widget_show(check_item);

  add_dictionary = gtk_menu_item_new_with_label("Choose Dictionary");
  g_signal_connect(G_OBJECT(add_dictionary), "activate", 
                   G_CALLBACK(AddDictionary), NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(spell_menu), add_dictionary);
  gtk_widget_show(add_dictionary);

  highlight_missing = gtk_menu_item_new_with_label("Highlight Missing");
  g_signal_connect(G_OBJECT(highlight_missing), "activate", 
    G_CALLBACK(HighlightMissing), NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(spell_menu), highlight_missing);
  gtk_widget_show(highlight_missing); 
}

/*EOF*/