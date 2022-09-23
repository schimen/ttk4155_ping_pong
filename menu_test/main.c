/*
    Test of menu library in terminal
*/
#include <stdio.h>
#include <stdlib.h>
#include "menu.h"

void stop_program(void* _)
{
    abort();
}

struct menu_page main_page = {
    .title = "Main menu",
    .options = {
        { .name = "First sub-menu",  .callback = &change_menu  },
        { .name = "Second sub-menu", .callback = &change_menu  },
        { .name = "Exit",            .callback = &stop_program }
    }
};

struct menu_page sub_page_1 = {
    .title = "Sub menu 1",
    .options = {
        { .name = "Main menu",  .callback = &change_menu },
        { .name = "Sub-menu 2", .callback = &change_menu }
    }
};

struct menu_page sub_page_2 = {
    .title = "Sub menu 2",
    .options = {
        { .name = "Main menu",  .callback = &change_menu },
        { .name = "Sub-menu 1", .callback = &change_menu }
    }
};

// Process incoming characters from the terminal
void handle_character()
{
    int c = getchar();
    switch (c) {
        case 'A': // Arrow up
            select_down();
            print_menu();
            break;
        case 'B': // Arrow down
            select_up();
            print_menu();
            break;
        case ' ': // Space (selection)
            run_option();
            break;
    }
}

int main()
{
    // Pointers to callback parameters. TODO: fix this
    main_page.options[0].callback_parameter = &sub_page_1;
    main_page.options[1].callback_parameter = &sub_page_2;
    sub_page_1.options[0].callback_parameter = &main_page;
    sub_page_1.options[1].callback_parameter = &sub_page_2;
    sub_page_2.options[0].callback_parameter = &main_page;
    sub_page_2.options[1].callback_parameter = &sub_page_1;

    change_menu(&main_page);
    while (1) {
        handle_character();
    }
}