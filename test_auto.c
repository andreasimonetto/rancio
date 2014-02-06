#include <stdio.h>
#include "rancio.h"

void print_r(rancio_item_t *root, unsigned indent)
{
rancio_item_t *p;
unsigned i;

    for(p = root; p && p->label; p++) {
        for(i = 0; i < indent; i++)
            printf("   ");
        printf("%s (%s)\n", p->id, p->label);
        print_r(p->childs, indent + 1);
    }
}

/* Printer */
void print_item(rancio_menu_t *menu, const char *label, unsigned id, unsigned len)
{
    printf("%c %2d. %s\n", (rancio_menu_curitem_id(menu) == id ? '*' : ' '), id + 1, label);
}

/* Item handler */
void print_selected_item(rancio_menu_t *menu, rancio_item_t *item)
{
    printf("\nMENU HANDLER: %s\n", item->label);
}

int main()
{
rancio_menu_t menu;
rancio_item_t *it, menuitems[] = {
    { "status", "Status", 0, 0, print_selected_item },
    { "setup", "Setup", 0, (rancio_item_t[]) {
        { "type", "Type", 0, (rancio_item_t[]) {
            { "type1", "Type I", 0, 0, print_selected_item },
            { "type2", "Type II", 0, 0, print_selected_item },
            { "type3", "Type III", 0, 0, print_selected_item },
            { 0, 0, 0, 0, 0 }
        }, 0 },
        { "age", "Your age", 0, 0, print_selected_item },
        { "name", "Your name", 0, 0, print_selected_item },
        { "network", "Network", 0, (rancio_item_t[]) {
            { "addr", "Address", 0, 0, print_selected_item },
            { "mask", "Netmask", 0, 0, print_selected_item },
            { "back", "Backdoor", 0, 0, print_selected_item },
            { 0, 0, 0, 0, 0 }
        }, 0},
        { 0, 0, 0, 0, 0 }
    }},
    { "exit", "Exit", 0, 0, print_selected_item },
    { 0, 0, 0, 0, 0 }
};

    rancio_menu_init(&menu, menuitems, print_item);
    print_r(menu.root, 0);

    it = rancio_menu_getitem(&menu, "/setup/network/addr");
    if(it)
        printf("\nSelected: %s\n", it->id);
    else
        printf("\nItem not found\n");
    return 0;
}

