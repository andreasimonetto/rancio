#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include "rancio.h"

/* Global menu and termination flag */
rancio_menu_t menu;
int done = 0;

/* Asynchronous event handler */
void input_handler(int signum)
{
rancio_action_t act = RANCIO_ACTION_NONE;

    switch(getchar()) {
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
            done = 1;
            break;
        default:
            break;
    }
    
    if(act != RANCIO_ACTION_CLOSE && act != RANCIO_ACTION_NONE) {
        rancio_menu_action(&menu, act);
        rancio_menu_print(&menu);
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
const char *msg[] = { "Doing my stuff", "Lot of stuff to do", "Working hard", "Doing some jobs" };
int oflags, t = 0;
struct termios oldattr, attr;

    srand(time(0));
    rancio_menu_init(&menu, menuitems, print_item);
    rancio_menu_print(&menu);
    printf("Commands: s=select, u=up, d=down, b=back, q=quit\n");
    
    /* Set async input with noecho */
    tcgetattr(STDIN_FILENO, &oldattr);
    attr = oldattr;
    attr.c_lflag &= ~(ECHO | ICANON);
    attr.c_cc[VMIN] = 1;
    attr.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &attr);
    signal(SIGIO, input_handler);
    oflags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, oflags | O_NONBLOCK | O_ASYNC);
    fcntl(STDIN_FILENO, F_SETOWN, getpid());

    /* Main loop */
    while(!done) {
        if(t <= 0) {
            t = 2 + (rand() % 4);
            fprintf(stderr, "\n%s, then snooze %ds...\n", msg[rand() % (sizeof(msg) / sizeof(char*))], t);
        }
        t = sleep(t);
    }

    /* Reset terminal attributes */
    tcsetattr(0, TCSAFLUSH, &oldattr);
    return 0;
}

