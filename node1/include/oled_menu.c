#include "oled_menu.h"
// Variable to keep track on the selected option
static int option_selection = 0;
// Current menu page
static struct menu_page *current_page;

void print_menu()
{
    //printf("print menu\n");
    // Clear terminal
    oled_clear();
    //printf("CLeared menu\n");
    // Set position 0
    oled_set_pos(0, 0);
    // Print title
    
    //printf("Print title\n");

    oled_putchar('*'); oled_putchar(' ');
    oled_print(current_page->title);
    oled_putchar(' '); oled_putchar('*');

    //oled_printf("** %s **\n", current_page->title);
    // Print all options
    for (uint8_t i = 0; i < MENU_LENGTH; i++) {
        //printf("print option %d\n", i);
        oled_set_pos(i+1, 0);
        char *name = current_page->options[i].name;
        if (i == option_selection) { // highlight selected option
            //printf("Option selected\n");
            oled_putchar('>');
            oled_print(name);
        }
        else {
            oled_putchar(' ');
            oled_print(name);
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

void select_up()
{
    option_selection--;
    if (option_selection < 0) {
        option_selection = MENU_LENGTH - 1;
    }
}

void select_down()
{
    option_selection++;
    if (option_selection > MENU_LENGTH - 1) {
        option_selection = 0;
    }
}