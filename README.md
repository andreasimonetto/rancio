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
void printer(rancio_menu_t *m, const char *l, unsigned id, unsigned len)
{
    // Print item at position i, having label l and having len siblings
    // Currently selected item can be retreived with rancio_menu_curitem_id(m)
}
```

and the item handler(s) are like this:

```C
void handler(rancio_menu_t *m, rancio_item_t *selected_item)
{
    // Things to do when the item is selected
}
```

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
    { "item1", "Item 1", 0, 0, handler1 },
    { "item2", "Item 2", 0, (rancio_item_t[]) {
        { "item21", "Item 2.1", 0, (rancio_item_t[]) {
            { "item211", "Item 2.1.1", 0, 0, handler2 },
            { "item212", "Item 2.1.2", 0, 0, handler3 },
            { 0, 0, 0, 0, 0 }
        }, 0 },
        { "item22", "Item 2.2", 0, 0, handler4 },
        { 0, 0, 0, 0, 0 }
    }},
    { "item3", "Item 3", 0, 0, handler5 },
    { 0, 0, 0, 0, 0 }
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

There's also an automatable way of accessing the menu items through the 
function `rancio_menu_getitem()`, that takes paths built with items' IDs:

```C
rancio_menu_getitem(menu, "/item2/item21/item212");
```

Compile
-------

Use [GNU make](http://www.gnu.org/software/make/) in order to compile the
library. From the source directory, type:

```bash
make librancio.a
```

There are also other targets for testing and cleaning (see Makefile).

The library is easy to link. Suppose to have a source file `example.c`;
then you can compile and link with:

```bash
gcc -o example -L/path/to/rancio example.c -lrancio
```

