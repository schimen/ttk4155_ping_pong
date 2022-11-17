#include "oled_menu.h"
// Variable to keep track on the selected option
static int option_selection = 0;
// Current menu page
static struct menu_page *current_page;
// Current high-score
static uint8_t high_score = 0;

void print_menu()
{
    // Clear terminal
    oled_clear();
    // Set position 0
    oled_set_pos(0, 0);
    // Print title
    oled_putchar('*'); oled_putchar(' ');
    oled_print(current_page->title);
    oled_putchar(' '); oled_putchar('*');
    // Print all options
    for (uint8_t i = 0; i < MENU_LENGTH; i++) {
        oled_set_pos(i+1, 0);
        char *name = current_page->options[i].name;
        if (i == option_selection) { // highlight selected option
            oled_putchar('>');
            oled_print(name);
        }
        else {
            oled_putchar(' ');
            oled_print(name);
        }
    }
}

void game_menu(uint8_t score)
{
    // Clear terminal
    oled_clear();
    // Set position 0
    oled_set_pos(0, 0);
    // Print title
    oled_print("* Game on *");
    oled_set_pos(2, 0);
    // Print high-score
    oled_print("High score: ");
    oled_print_number(high_score);
    // Print current score
    oled_set_pos(4, 0);
    oled_print("Your score: ");
    oled_print_number(score);
    // Exit menu by pressing right button
    oled_set_pos(6, 0);
    oled_print(">Exit");
}

void game_over(uint8_t score) {
    // Clear terminal
    oled_clear();
    // Set position 0
    oled_set_pos(0, 0);
    // Print title
    oled_print("* Game over *");
    oled_set_pos(2, 0);
    // Print high-score
    oled_print("High score: ");
    oled_print_number(score);
    // Print current score
    oled_set_pos(4, 0);
    oled_print("Your score: ");
    oled_print_number(score);
    // Print message if high-score
    oled_set_pos(6, 0);
    if (high_score <= score) {
        oled_print("New high-score!");
        high_score = score;
        _delay_ms(3000);
        show_img_bird();
        _delay_ms(2000);
    }
    else {
        oled_print("No high-score");
        _delay_ms(3000);
        show_img_troll();
        _delay_ms(2000);
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