/* Rancio - tiny Textual Menu System library
   Copyright (C) 2013  asdrea <asdrea@behindthemind.org>

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
#include "rancio.h"

/* Menu item (mi) helpers */
#define mi_isroot(p) ((p) && !((p)->parent))
#define mi_isleaf(p) ((p) && !((p)->childs))
#define mi_parent(p) ((p)->parent)
#define mi_firstchild(p) ((p)->childs)
#define mi_islastsibling(p) (!(p) || !((p)->label))
#define mi_getid(root, item) ((item) - mi_firstsibling(root, item))

static rancio_item_t* mi_firstsibling(rancio_item_t *root, rancio_item_t *item)
{
    if(!mi_islastsibling(item))
        return (item->parent ? item->parent->childs : root);
    return 0;
}

static rancio_item_t* mi_nextsibling(rancio_item_t *root, rancio_item_t *item)
{
rancio_item_t *next_sibling = item + 1;

    if(!mi_islastsibling(item))
        return (next_sibling && next_sibling->label ? next_sibling : 0);
    return 0;
}

static rancio_item_t* mi_prevsibling(rancio_item_t *root, rancio_item_t *item)
{
rancio_item_t *prev_sibling = item - 1, *first_sibling = mi_firstsibling(root, item);

    if(first_sibling)
       return (prev_sibling >= first_sibling && prev_sibling->label ? prev_sibling : 0);
    return 0;
}

static void mi_init_r(rancio_item_t *root)
{
rancio_item_t *pchild;

    for(pchild = root->childs; !mi_islastsibling(pchild); pchild++) {
        pchild->parent = root;
        mi_init_r(pchild);
    }
}

/* APIs implementation */
rancio_menu_t* rancio_menu_init(rancio_menu_t *menu, rancio_item_t *items, rancio_printer_t printer)
{
rancio_item_t *p;

    for(p = items; !mi_islastsibling(p); p++)
        mi_init_r(p);
        
    menu->root = menu->curitem = items;
    menu->printer = printer;
    menu->userdata = 0;
    return menu;
}

void rancio_menu_print(rancio_menu_t *menu)
{
rancio_item_t *p;
unsigned i = 0, len = 0;

    for(p = mi_firstsibling(menu->root, menu->curitem); !mi_islastsibling(p); p++)
        len++;

    for(p = mi_firstsibling(menu->root, menu->curitem); !mi_islastsibling(p); p++)
        menu->printer(menu, p->label, i++, len);
}

int rancio_menu_action(rancio_menu_t *menu, rancio_action_t act)
{
rancio_item_t *p;

    switch(act) {
        case RANCIO_ACTION_SELECT:
            if(mi_isleaf(menu->curitem)) {
                if(menu->curitem->handler)
                    menu->curitem->handler(menu, menu->curitem);
            }
            else
                menu->curitem = mi_firstchild(menu->curitem);
            break;
        case RANCIO_ACTION_UP:
            if((p = mi_prevsibling(menu->root, menu->curitem)))
                menu->curitem = p;
            break;
        case RANCIO_ACTION_DOWN:
            if((p = mi_nextsibling(menu->root, menu->curitem)))
                menu->curitem = p;
            break;
        case RANCIO_ACTION_BACK:
            if(!mi_isroot(menu->curitem))
                menu->curitem = mi_parent(menu->curitem);
            break;
        case RANCIO_ACTION_CLOSE:
            return 0;
            break;
        case RANCIO_ACTION_NONE:
        default:
            /* nop */
            break;
    }
    return 1;
}

unsigned rancio_menu_curitem_id(rancio_menu_t *menu)
{
    return (menu->curitem - mi_firstsibling(menu->root, menu->curitem));
}

rancio_item_t* rancio_menu_getitem(rancio_menu_t *menu, const char *path)
{
unsigned len;
rancio_item_t *p;

    if(!path || !(*path))
        return 0;
    
    if(path[0] == '/') {
        p = menu->root;
        path++;
    }
    else
        p = menu->curitem;
    
    while(*path) {
        len = 0;
        while(path[len] && path[len] != '/' && !mi_islastsibling(p)) {
            while(path[len] && path[len] != '/' && p->id[len] && path[len] == p->id[len])
                len++;
            
            if((path[len] && path[len] != '/') || p->id[len]) {
                len = 0;
                p++;
            }
        }
        
        if(mi_islastsibling(p))
            return 0;

        path += len;            
        if(*path) {
            path++;
            if(*path)
                p = p->childs;
        }
    }
    
    return p;
}

