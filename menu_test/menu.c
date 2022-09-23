#include "menu.h"

// Variable to keep track on the selected option
static int option_selection = 0;
// Current menu page
static struct menu_page *current_page;

void print_menu()
{
    // Clear terminal
    system("clear");
    // Print title
    printf("** %s **\n", current_page->title);
    // Print all options
    for (int i = 0; i < MENU_LENGTH; i++) {
        if (i == option_selection) { // highlight selected option
            printf("> %s\n", current_page->options[i].name);
        }
        else {
            printf("  %s\n", current_page->options[i].name);
        }
    }
}

void change_menu(void *next_page)
{
    struct menu_page *page = next_page;
    if (page) {
        current_page = page;
    }
    option_selection = 0;
    print_menu();
}

void run_option()
{
    struct menu_option option = current_page->options[option_selection];
    if (option.callback) {
        (*option.callback)(option.callback_parameter);
    }
    else {
        print_menu();
    }
}

void select_down()
{
    option_selection--;
    if (option_selection < 0) {
        option_selection = MENU_LENGTH - 1;
    }
}

void select_up()
{
    option_selection++;
    if (option_selection > MENU_LENGTH - 1) {
        option_selection = 0;
    }
}