#include <iostream>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <cstdlib>

#include <ncurses.h>
#include <panel.h>
#include <menu.h>
#include <form.h>

#include "CCadena.h"
#include "ListaAb.h"
#include "DobleEnlace.h"

#include <string>
#include <fstream>
#include <sstream>
using namespace std;
// Lista output
std::string lista_output = "";
std::string listad1_output = "";
std::string listad2_output = "";

Lista<Cadena> ListaCad;
lista<Cadena> iLista1;

// Definided by panel hide.
typedef struct _PANEL_DATA {
        int hide;       /* TRUE if panel is hidden */
}PANEL_DATA;

// Plantilla de funci�n que mustra el valor del objeto
// dado como par�metro formando una lista separada con comas
template<class DATO>
void Mostrar(DATO &d)
{
  lista_output.append(d.Lee());
  lista_output.append("|->|");
  //printw(":%s",d.Lee());
}

template<class TIPO>
void Mostrard1(TIPO &d)
{
  listad1_output.append(d.Lee());
  listad1_output.append("|<->|");
  //printw(":%s",d.Lee());
}

template<class TIPO>
void Mostrard2(TIPO &d)
{
  listad2_output.append(d.Lee());
  listad2_output.append("|<->|");
  //printw(":%s",d.Lee());
}

/*template<class DATO>
FIELD *toMostrar(DATO &d)
{
  //return d.Lee();
  //set_field_buffer(field, 0, d.Lee());
  return d.get_field();
}*/

#define NLINES 10
#define NCOLS 40

// Definided by menu options.
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
const char *choices[] = {
                        "Window 1",
                        "Window 2",
                        "Window 3",
                        "Window 4",
                        "Window 5",
                        "Window 6",
                        "Window 7",
                        "Exit",
                  };

WINDOW *create_timber_win(int height, int width, int starty, int startx);
WINDOW *create_timber_menu_win(int height, int width, int starty, int startx);

MENU *create_timber_menu(WINDOW * win);

void init_wins(WINDOW **wins, int n, int height, int width, int starty, int startx);
void win_show(WINDOW *win, char *label, int label_color);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

WINDOW *charge_lista_win(WINDOW * local_win);
WINDOW *charge_listad_win(WINDOW * local_win);
WINDOW *charge_welcome_win(WINDOW * local_win);
FORM *create_form_ccadena(const char *cadena_dat, const char *cadena_dir);
static char* trim_whitespaces(char *str);

string intTOstring(int entero);

int main()
{
  WINDOW *timber_win, *timber_menu_win;
  MENU *timber_menu;
  int startx=0, starty=0, width=0, height=0;
  int size_menu=14, number_wins=7;
  int my_choice = -1;	/* the zero based numeric user choice */

  WINDOW *my_wins[number_wins];
  PANEL  *my_panels[number_wins];
  PANEL_DATA panel_datas[number_wins];
  PANEL_DATA *temp;
  int ch;

  /* Initialize curses */
  initscr();
  getmaxyx(stdscr, height, width);
  start_color();
  cbreak();
  noecho();               // To suppress the automatic echoing of typed characters.
  keypad(stdscr, TRUE);   //In order to capture special keystrokes like Backspace, Delete and the four arrow keys.
	printw("Press F1 to exit {y=%d x=%d}",height,width);
	refresh();

  /* Initialize menu */
  timber_win = create_timber_win(0, 0, starty + 1, startx + 0);
  timber_menu_win = create_timber_menu_win(height-3, size_menu, starty + 2, startx + 1);
  timber_menu = create_timber_menu(timber_menu_win);
  post_menu(timber_menu);
  wrefresh(timber_menu_win);

  /* Initialize panel */
  init_wins(my_wins, number_wins, height-3, width - size_menu - 4, starty + 2, size_menu + startx + 1);

  /* Attach a panel to each window */     /* Order is bottom up */
  for(int i = 0; i < number_wins; ++i){
    my_panels[i] = new_panel(my_wins[i]);   /* Push 0, order: stdscr-0 */
    panel_datas[i].hide = FALSE;            /* Initialize panel datas saying that nothing is hidden */
    set_panel_userptr(my_panels[i], &panel_datas[i]);
  }

  /* Update the stacking order. 2nd panel will be on top */
  update_panels();

  my_wins[6] = charge_welcome_win(my_wins[6]);
  while((ch = wgetch(timber_menu_win)) != KEY_F(1))
  {

    switch(ch)
    {
      case KEY_DOWN:
      menu_driver(timber_menu, REQ_DOWN_ITEM);
      //printw("test ch = %d",ch);
      //hide_panel(my_panels[0]);
      //temp->hide = TRUE;
      break;
      case KEY_UP:
      menu_driver(timber_menu, REQ_UP_ITEM);
      break;
      case KEY_LEFT:
      menu_driver(timber_menu, REQ_LEFT_ITEM);
      break;
      case KEY_RIGHT:
      menu_driver(timber_menu, REQ_RIGHT_ITEM);
      break;
      case KEY_NPAGE:
      menu_driver(timber_menu, REQ_SCR_DPAGE);
      break;
      case KEY_PPAGE:
      menu_driver(timber_menu, REQ_SCR_UPAGE);
      //printw("test ch = %d",ch);
      break;
      case 97: // Case "a"
          printw("Debug: ch = %d and my_choice = %d",ch, my_choice);
          my_wins[0] = charge_listad_win(my_wins[0]);
      break;
      case 10:
        my_choice = item_index(current_item(timber_menu));

        if (my_choice < number_wins){
          temp = (PANEL_DATA *)panel_userptr(my_panels[my_choice]);

          if(temp->hide == TRUE)
          {       hide_panel(my_panels[my_choice]);
            temp->hide = FALSE;
          }
          else
          {       show_panel(my_panels[my_choice]);
            temp->hide = TRUE;
          }
        } else { printw("Debug: ch = %d and my_choice = %d",ch, my_choice); }

		  break;
      default: {
          //my_wins[6] = charge_welcome_win(my_wins[6]);
          mvprintw(0, 50, "Item selected is : ");
        //break;
      }
    }
    update_panels();
    wrefresh(timber_menu_win);
    //update_panels();
  }

  //ch = getch();

  endwin();
  return 0;
}

string intTOstring(int entero)
	    {
		    stringstream out;
			string str;

			out << entero;
			return out.str();
	    }

WINDOW *create_timber_win(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);

  use_default_colors();     // keep the transparency and not apply a dark black opaque background.
	box(local_win, 0 , 0);		/* 0, 0 gives default characters
					 * for the vertical and horizontal
					 * lines			*/
  waddstr(local_win, "[[Timber lab]]");   //Set title window.
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

MENU *create_timber_menu(WINDOW * local_win)
{ MENU *local_menu;

  ITEM **my_items;
  int c;
  int n_choices, i;
  ITEM *cur_item;

  /* Create items */
  n_choices = ARRAY_SIZE(choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for(i = 0; i < n_choices; ++i){
    my_items[i] = new_item(choices[i], choices[i]);
  }
  my_items[n_choices] = (ITEM *)NULL;
  /* Crate menu */
  local_menu = new_menu((ITEM **)my_items);

  /* Set menu option not to show the description */
  menu_opts_off(local_menu, O_SHOWDESC);

  /* Create the window to be associated with the menu */
  ////local_win = create_timber_menu(height, width, starty, startx);
  //local_win = newwin(10, 70, 4, 4);
  keypad(local_win, TRUE);

  /* Set main window and sub window */
  /* Bug: The menu overide the title of the window.*/
  set_menu_win(local_menu, local_win);
  set_menu_sub(local_menu, derwin(local_win, 6, 68, 3, 1));
  set_menu_format(local_menu, 10, 1);
  set_menu_mark(local_menu, " > ");

  //refresh();

  /* Post the menu */
  post_menu(local_menu);

	return local_menu;
}

WINDOW *create_timber_menu_win(int height, int width, int starty, int startx)
{	WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);

  //use_default_colors();     // keep the transparency and not apply a dark black opaque background.
	box(local_win, 0 , 0);		/* 0, 0 gives default characters
					 * for the vertical and horizontal
					 * lines			*/
  waddstr(local_win, "[[Options]]");   //Set title window.
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

/* Put all the windows */
void init_wins(WINDOW **wins, int n, int height, int width, int starty, int startx)
{       int i;
        char label[80];

        for(i = 0; i < n; ++i)
        {       wins[i] = newwin(height, width, starty, startx);
                sprintf(label, "Window %d", i + 1);
                win_show(wins[i], label, i + 1);
        }
}

/* Show the window with a border and a label */
void win_show(WINDOW *win, char *label, int label_color)
{       int startx, starty, height, width;

        getbegyx(win, starty, startx);
        getmaxyx(win, height, width);

        box(win, 0, 0);
        mvwaddch(win, 2, 0, ACS_LTEE);
        mvwhline(win, 2, 1, ACS_HLINE, width - 2);
        mvwaddch(win, 2, width - 1, ACS_RTEE);

        print_in_middle(win, 1, 0, width, label, COLOR_PAIR(label_color));
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{       int length, x, y;
        float temp;

        if(win == NULL)
                win = stdscr;
        getyx(win, y, x);
        if(startx != 0)
                x = startx;
        if(starty != 0)
                y = starty;
        if(width == 0)
                width = 80;

        length = strlen(string);
        temp = (width - length)/ 2;
        x = startx + (int)temp;
        mvwprintw(win, y, x, "%s", string);
        refresh();
}

WINDOW *charge_welcome_win(WINDOW * local_win){
  int field_num = 3;
  FIELD *field[field_num];
  FORM  *my_form;
  int ch, rows, cols, height=0, width=0;
  int reserv_to_cadena=4;

  //getmaxyx(local_win, height, width);

  /* (numero de lineas, ancho o characters, altura, ) */
  //field[0] = new_field(2, width-1, reserv_to_cadena, 1, 8, 8);
  field[0] = new_field(1, 24, 1, 1, 8, 8);
  field[1] = new_field(1, 0, 2, 1, 8, 8);
  field[2] = new_field(2, 9, 4, 1, 8, 8);

  for (int i=0; field[i]; i++){
    /* Set field options */
  	set_field_back(field[i], A_UNDERLINE);
  	field_opts_off(field[i], O_AUTOSKIP); /* Don't go to next field when this */
  }

  /* SET of LABELS */
  set_field_buffer(field[0], 0,"Welcome to Timber lab.");
  //field_opts_off(field[0], O_ACTIVE); /* This field is a static label */
  set_field_just(field[0], JUSTIFY_CENTER); /* Center Justification */

  /* SET of LABELS */
  set_field_buffer(field[2], 0,"TEXT---- o el README");
  //field_opts_off(field[1], O_ACTIVE); /* This field is a static label */
  set_field_just(field[2], JUSTIFY_CENTER); /* Center Justification */

  /* Create the form and post it */
  my_form = new_form(field);

  /* Calculate the area required for the form */
  scale_form(my_form, &rows, &cols);

  //keypad(local_win, TRUE);

  /* Set main window and sub window */
  set_form_win(my_form, local_win);
  set_form_sub(my_form, derwin(local_win, rows, cols,3,1));

  //mvprintw(4, 10, "Value 1:");
  //post_form(my_form_cadena);
  post_form(my_form);
  wrefresh(local_win);

  return local_win;
}

WINDOW *charge_lista_win(WINDOW * local_win){
  int field_num = 8;
  FIELD *field[field_num];
	FORM  *my_form;
  int ch, rows, cols, height=0, width=0;

  getmaxyx(local_win, height, width);
  /* Initialize the fields */
  int reserv_to_cadena=4;
  /* (numero de lineas, ancho o characters, altura, ) */
  //field[0] = new_field(2, width-1, reserv_to_cadena, 1, 8, 8);
  field[0] = new_field(1, 27, 1, 1, 8, 8);
  field[1] = new_field(reserv_to_cadena, width-4, 2, 1, 8, 8);
  field[2] = new_field(1, 9,  reserv_to_cadena+4, 1, 8, 8);
  field[3] = new_field(1, 20, reserv_to_cadena+6, 1, 8, 8);
  field[4] = new_field(1, 9, reserv_to_cadena+8, 1, 8, 8);
  field[5] = new_field(1, 20, reserv_to_cadena+10, 1, 8, 8);
  field[6] = new_field(1, 20, reserv_to_cadena+12, 1, 8, 8);
  field[7] = NULL;

  for (int i=0; field[i]; i++){
    /* Set field options */
  	set_field_back(field[i], A_UNDERLINE);
  	field_opts_off(field[i], O_AUTOSKIP); /* Don't go to next field when this */
  }

  /* SET of LABELS */
  set_field_buffer(field[0], 0,"Lista Simplemente Enlazada:");
  field_opts_off(field[0], O_ACTIVE); /* This field is a static label */
  set_field_just(field[0], JUSTIFY_CENTER); /* Center Justification */
  /* Field for show the lists */
  field_opts_off(field[1], O_ACTIVE); /* This field is a static label */
  //set_field_just(field[0], JUSTIFY_CENTER); /* Center Justification */
  /* Common LABELS */
  set_field_buffer(field[2], 0,"Insertar:");
  field_opts_off(field[2], O_ACTIVE);
  set_field_buffer(field[4], 0,"Eliminar:");
  field_opts_off(field[4], O_ACTIVE);
  /* Initialize Data Estructures */
  // Declaraci�n de una lista de cadenas:

  if(ListaCad.Vacia())
  {
      // Inserci�n de algunos valores, creando una lista ordenada:
      ListaCad.Insertar("beta");
      ListaCad.Insertar("delta");
      ListaCad.Insertar("delta");
      ListaCad.Insertar("gamma");
      ListaCad.Insertar("delta");
      ListaCad.Insertar("epsilon");
      ListaCad.Insertar("sigma");
      ListaCad.Insertar("alfa");
  }
  //lista_output = "";
  //ListaCad.ParaCada(Mostrar);
  //set_field_buffer(field[1], 0,&lista_output[0]);

  /* Create the form and post it */
  my_form = new_form(field);

  /* Calculate the area required for the form */
  scale_form(my_form, &rows, &cols);

  keypad(local_win, TRUE);

  /* Set main window and sub window */
  set_form_win(my_form, local_win);
  set_form_sub(my_form, derwin(local_win, rows, cols,3,1));

  //mvprintw(4, 10, "Value 1:");
  //post_form(my_form_cadena);
  post_form(my_form);
  wrefresh(local_win);

  mvprintw(LINES - 2, 0, "Use UP, DOWN arrow keys to switch between fields");
  refresh();

  /* Loop through to get user requests */
	while((ch = wgetch(local_win)) != KEY_F(1))
	{
    lista_output = "";
    ListaCad.ParaCada(Mostrar);
    //set_field_fore(field[1], COLOR_PAIR(2));
    set_field_buffer(field[1], 0, &lista_output[0]);
    printw("Debug: ch = %d",ch);
    switch(ch)
		{	case KEY_DOWN:
				/* Go to next field */
				form_driver(my_form, REQ_NEXT_FIELD);
				/* Go to the end of the present buffer */
				/* Leaves nicely at the last character */
				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_UP:
				/* Go to previous field */
				form_driver(my_form, REQ_PREV_FIELD);
				form_driver(my_form, REQ_END_LINE);
				break;
      case 10:
        // Or the current field buffer won't be sync with what is displayed
        form_driver(my_form, REQ_NEXT_FIELD);
        form_driver(my_form, REQ_PREV_FIELD);
        move(LINES-3, 2);

        //printw("%s", trim_whitespaces(field_buffer(field[3], 0)));
        if (strlen(trim_whitespaces(field_buffer(field[3], 0))) != 0 ){
          ListaCad.Insertar(trim_whitespaces(field_buffer(field[3], 0)));
          set_field_buffer(field[3], 0,"");
        }
        if (strlen(trim_whitespaces(field_buffer(field[5], 0))) != 0 ){
          ListaCad.BorrarPrimerValor(trim_whitespaces(field_buffer(field[5], 0)));
          set_field_buffer(field[5], 0,"");
        }
        lista_output = "";
        ListaCad.ParaCada(Mostrar);
        set_field_buffer(field[1], 0, &lista_output[0]);
      break;
			default:
				/* If this is a normal character, it gets */
				/* Printed				  */
				form_driver(my_form, ch);

			break;
		}
    lista_output = "";
    ListaCad.ParaCada(Mostrar);
    set_field_buffer(field[1], 0, &lista_output[0]);
    update_panels();
    doupdate();
	}

  return local_win;
}

WINDOW *charge_listad_win(WINDOW * local_win){
  int field_num = 8;
  FIELD *field[field_num];
	FORM  *my_form;
  int ch, rows, cols, height=0, width=0;

  getmaxyx(local_win, height, width);
  /* Initialize the fields */
  int reserv_to_cadena=2;
  /* (numero de lineas, ancho o characters, altura, ) */
  //field[0] = new_field(2, width-1, reserv_to_cadena, 1, 8, 8);
  field[0] = new_field(1, 27, 1, 1, 8, 8);
  field[1] = new_field(reserv_to_cadena, width-4, 2, 1, 8, 8);
  field[2] = new_field(1, 9,  reserv_to_cadena+4, 1, 8, 8);
  field[3] = new_field(1, 20, reserv_to_cadena+6, 1, 8, 8);
  field[4] = new_field(1, 9, reserv_to_cadena+8, 1, 8, 8);
  field[5] = new_field(1, 20, reserv_to_cadena+10, 1, 8, 8);
  field[6] = new_field(1, 20, reserv_to_cadena+12, 1, 8, 8);
  field[7] = NULL;

  for (int i=0; field[i]; i++){
    /* Set field options */
  	set_field_back(field[i], A_UNDERLINE);
  	field_opts_off(field[i], O_AUTOSKIP); /* Don't go to next field when this */
  }

  /* SET of LABELS */
  set_field_buffer(field[0], 0,"Lista Doblemente Enlazada:");
  field_opts_off(field[0], O_ACTIVE); /* This field is a static label */
  set_field_just(field[0], JUSTIFY_CENTER); /* Center Justification */
  /* Field for show the lists */
  field_opts_off(field[1], O_ACTIVE); /* This field is a static label */
  //set_field_just(field[0], JUSTIFY_CENTER); /* Center Justification */
  /* Common LABELS */
  set_field_buffer(field[2], 0,"Insertar:");
  field_opts_off(field[2], O_ACTIVE);
  set_field_buffer(field[4], 0,"Eliminar:");
  field_opts_off(field[4], O_ACTIVE);
  /* Initialize Data Estructures */
  // Declaraci�n de una lista de cadenas:

  if(iLista1.ListaVacia())
  {
      // Inserci�n de algunos valores, creando una lista ordenada:
      iLista1.Insertar("10");
      iLista1.Insertar("20");
      iLista1.Insertar("30");
      iLista1.Insertar("40");
      iLista1.Insertar("50");
      iLista1.Insertar("60");
      iLista1.Insertar("70");
      iLista1.Insertar("80");
  }
  //lista_output = "";
  //ListaCad.ParaCada(Mostrar);
  //set_field_buffer(field[1], 0,&lista_output[0]);

  /* Create the form and post it */
  my_form = new_form(field);

  /* Calculate the area required for the form */
  scale_form(my_form, &rows, &cols);

  keypad(local_win, TRUE);

  /* Set main window and sub window */
  set_form_win(my_form, local_win);
  set_form_sub(my_form, derwin(local_win, rows, cols,3,1));

  //mvprintw(4, 10, "Value 1:");
  //post_form(my_form_cadena);
  post_form(my_form);
  wrefresh(local_win);

  mvprintw(LINES - 2, 0, "Use UP, DOWN arrow keys to switch between fields");
  refresh();

  /*listad1_output = "";
  iLista1.ParaCada(Mostrard);
  //set_field_fore(field[1], COLOR_PAIR(2));
  set_field_buffer(field[1], 0, &listad1_output[0]);

  /* Loop through to get user requests */
	while((ch = wgetch(local_win)) != KEY_F(1))
	{
    listad1_output = "";
    iLista1.ParaCada(Mostrard1);
    //set_field_fore(field[1], COLOR_PAIR(2));
    set_field_buffer(field[1], 0, &listad1_output[0]);
    printw("Debug: ch = %d",ch);
    switch(ch)
		{	case KEY_DOWN:
				/* Go to next field */
				form_driver(my_form, REQ_NEXT_FIELD);
				/* Go to the end of the present buffer */
				/* Leaves nicely at the last character */
				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_UP:
				/* Go to previous field */
				form_driver(my_form, REQ_PREV_FIELD);
				form_driver(my_form, REQ_END_LINE);
				break;
      case 10:
        // Or the current field buffer won't be sync with what is displayed
        form_driver(my_form, REQ_NEXT_FIELD);
        form_driver(my_form, REQ_PREV_FIELD);
        move(LINES-3, 2);

        //printw("%s", trim_whitespaces(field_buffer(field[3], 0)));
        if (strlen(trim_whitespaces(field_buffer(field[3], 0))) != 0 ){
          iLista1.Insertar(trim_whitespaces(field_buffer(field[3], 0)));
          set_field_buffer(field[3], 0,"");
        }
        if (strlen(trim_whitespaces(field_buffer(field[5], 0))) != 0 ){
          iLista1.Borrar(trim_whitespaces(field_buffer(field[5], 0)));
          set_field_buffer(field[5], 0,"");
        }
        listad1_output = "";
        iLista1.ParaCada(Mostrard1);
        set_field_buffer(field[1], 0, &lista_output[0]);
      break;
			default:
				/* If this is a normal character, it gets */
				/* Printed				  */
				form_driver(my_form, ch);

			break;
		}
    listad1_output = "";
    iLista1.ParaCada(Mostrard1);
    set_field_buffer(field[1], 0, &listad1_output[0]);
    update_panels();
    doupdate();
	}

  return local_win;
}

/*
 * This is useful because ncurses fill fields blanks with spaces.
 */
static char* trim_whitespaces(char *str)
{
        char *end;

        // trim leading space
        while(isspace(*str))
                str++;

        if(*str == 0) // all spaces?
                return str;

        // trim trailing space
        end = str + strnlen(str, 128) - 1;

        while(end > str && isspace(*end))
                end--;

        // write new null terminator
        *(end+1) = '\0';

        return str;
}
