/* Rancio - tiny Textual Menu System library
   Copyright (C) 2013  Andrea Simonetto <self@andrea.simonetto.name>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef RANCIO_H
#define RANCIO_H

#define RANCIO_MAJOR 1  /* API breaking */
#define RANCIO_MINOR 0  /* Minor features or significant fixes */
#define RANCIO_REV 0    /* Minor bugs */

/* Test for RANCIO > 1.2.77 with:
#if RANCIO_VERSION > 10277
...
#endif
*/
#define RANCIO_VERSION (RANCIO_REV + RANCIO_MINOR * 100 + RANCIO_MAJOR * 10000)

/* Forward declarations */
typedef struct _rancio_item rancio_item_t;
typedef struct _rancio_menu rancio_menu_t;

/* Menu actions */
typedef enum {
    RANCIO_ACTION_NONE,   /* This does nothing */
    RANCIO_ACTION_SELECT, /* Go to submenu (if any) or call item handler */
    RANCIO_ACTION_UP,     /* Go to previous sibling item */
    RANCIO_ACTION_DOWN,   /* Go to next sibling item */
    RANCIO_ACTION_BACK,   /* Go to parent menu, if any, otherwise nop */
    RANCIO_ACTION_CLOSE   /* This also does nothing but can be used in future */
} rancio_action_t;

/* Functional parameters */
typedef void (*rancio_item_handler_t)(rancio_menu_t *menu, rancio_item_t *item);
typedef void (*rancio_printer_t)(rancio_menu_t *menu, const char *name, unsigned id, unsigned len);
/*typedef rancio_action_t (*rancio_reader_t)(rancio_menu_t *menu);*/

/* Menu item */
struct _rancio_item {
    const char *name;
    rancio_item_t *parent; 
    rancio_item_t *childs; /* This must be an array of items */
    
    /* Function to call when item is selected. The function is called only
    when the item have no childs (i.e. there's no submenu). The handler must
    be the name of a function like:
    
    void f(rancio_menu_t *m, rancio_item_t *selected_item)
    {
        // Things to do when this item is selected
    }
    */
    rancio_item_handler_t handler;
};

/* Menu */
struct _rancio_menu {
    rancio_item_t *root;
    rancio_item_t *curitem;
    rancio_printer_t printer;
    
    /* Set to NULL into rancio_menu_init(). This field is never access after
    the menu is initialized, and can be freely used for storing user data. */
    void *userdata;
};

/* Initialize menu with given items (an array of rancio_item_t) and printer. The
printer is a function like:
void f(rancio_menu_t *m, const char *n, unsigned i, unsigned len)
{
    // Print item at position i, having name n and having len siblings
    // Currently selected item can be retreived with rancio_menu_curitem_id(m)
}
*/
rancio_menu_t* rancio_menu_init(rancio_menu_t *menu, rancio_item_t *items, rancio_printer_t printer);

/* Inject an action into a menu. Printer is NOT called by this function.
*/
int rancio_menu_action(rancio_menu_t *menu, rancio_action_t act);

/* Print user menu using printer passed to rancio_menu_init().
*/
void rancio_menu_print(rancio_menu_t *menu);

/* Return the index of the currently selected item.
*/
unsigned rancio_menu_curitem_id(rancio_menu_t *menu);

#endif

