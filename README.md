Rancio - tiny Textual Menu System
=================================

Summary
-------

Rancio is a library that provide a general interface for textual menu building
and accessing. The library is written with portability in mind (ANSI C) and is 
kept as small and simple as possible.

Usage
-----

In order to use the library, you must provide a function for displaying menu
items, and optionally one or more handlers for menu items. The printer is
something like:

```C
void printer(rancio_menu_t *m, const char *name, unsigned id, unsigned len)
{
    // Print item at position i, having name n and having len siblings
    // Currently selected item can be retreived with rancio_menu_curitem_id(m)
}
```

and the item handler(s) are like this:

void handler(rancio_menu_t *m, rancio_item_t *selected_item)
{
    // Things to do when the item is selected
}

Initialization
--------------

Suppose to have printer and handlers functions, the menu initialization is
something like:

```C
#include "rancio.h"

/* Other includes + printer and handlers definition */

int main()
{
rancio_menu_t menu;
rancio_item_t menuitems[] = {
    { "Item1", 0, 0, handler1 },
    { "Item2", 0, (rancio_item_t[]) {
        { "Item2.1", 0, (rancio_item_t[]) {
            { "Item2.1.1", 0, 0, handler2 },
            { "Item2.1.2", 0, 0, handler3 },
            { 0, 0, 0, 0 }
        }, 0 },
        { "Item2.2", 0, 0, handler4 },
        { 0, 0, 0, 0 }
    }},
    { "Item3", 0, 0, handler5 },
    { 0, 0, 0, 0 }
};

    rancio_menu_init(&menu, menuitems, printer);
    /* ... */
    return 0;
}
```

I/O
---

Once the menu is initalized, the management is very simple through the 
functions `rancio_menu_action()` (for input) and `rancio_menu_print()` for
menu printing.

Available actions are:

* `RANCIO_ACTION_NONE`: empty action, does nothing at all;
* `RANCIO_ACTION_SELECT`: go to submenu (if any) or call item handler;
* `RANCIO_ACTION_UP`: go to previous sibling item;
* `RANCIO_ACTION_DOWN`: go to next sibling item;
* `RANCIO_ACTION_BACK`: go to parent menu, if any, otherwise nop;
