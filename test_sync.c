#include <stdio.h>
#include "rancio.h"

/*
void print_r(rancio_item_t *root, unsigned indent)
{
rancio_item_t *p;
unsigned i;

    for(p = root; p && p->label; p++) {
        for(i = 0; i < indent; i++)
            printf("   ");
        puts(p->label);
        print_r(p->childs, indent + 1);
    }
}
*/

/* Blocking input request */
rancio_action_t get_action()
{
char command;
rancio_action_t act;

    do {
        printf("Command (s=select, u=up, d=down, b=back, q=quit)? ");
        fflush(stdout);
        command = getchar();
        while(command != '\n' && getchar() != '\n');

        switch(command) {
            case 's':
                act = RANCIO_ACTION_SELECT;
                break;
            case 'u':
                act = RANCIO_ACTION_UP;
                break;
            case 'd':
                act = RANCIO_ACTION_DOWN;
                break;
            case 'b':
                act = RANCIO_ACTION_BACK;
                break;
            case 'q':
                act = RANCIO_ACTION_CLOSE;
                break;
            default:
                act = RANCIO_ACTION_NONE;
                printf("\nWhat?!\n");
                break;
        }
        printf("\n");
    } while(act == RANCIO_ACTION_NONE);
    return act;
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
rancio_item_t menuitems[] = {
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
    /*print_r(menu.root, 0);*/
    
    do {
        rancio_menu_print(&menu);
    } while(rancio_menu_action(&menu, get_action()));

    return 0;
}

