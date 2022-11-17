#ifndef OLED_MENU_H_
#define OLED_MENU_H_

/**
 * @brief OLED menu API for node 1
 * @defgroup oled_menu
 * @ingroup oled_menu
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include "util/delay.h"
#include "oled_lib.h"

#define MENU_LENGTH 7   // Number of options on a page
#define SCREEN_WIDTH 16 // Width of line in terms of characters

/**
 * @brief Struct representing option for menu.
 * - name: Array of characters representing option text.
 * - callback: Pointer to callback function that will be called when selected.
 * This function takes a void pointer that can be used to pass data.
 * - callback_parameter: void pointer that will be passed to callback.
*/
struct menu_option {
    // Name of option
    char name[SCREEN_WIDTH];
    // Pointer to function called upon selection
    void (*callback)(void*);
    // Void pointer passed to callback
    void *callback_parameter;
};

/**
 * @brief Struct representing a page in the menu.
 * - title: Array of characters representing title of page.
 * - options: Array of menu options that page will contain.
*/
struct menu_page {
    // Title of page
    char title[SCREEN_WIDTH];
    // Option array representing options in page
    struct menu_option options[MENU_LENGTH];
};


/**
 * @brief Print the menu saved to current_menu to the terminal. 
 * 
 */
void print_menu();


/**
 * @brief Save a new menu to current_menu and print it. 
 * 
 * @param next_page pointer to next page
 */
void change_menu(void *next_page);


/**
 * @brief Run the callback function in selected option. 
 * 
 */
void run_option();


/**
 * @brief Move the option selection one option up.
 * 
 */
void select_up();


/**
 * @brief Move the option selection one option down.
 * 
 */
void select_down();


/**
 * @brief Show game stats during game
 * 
 * @param score Current game score
 */
void game_menu(uint8_t score);


/**
 * @brief Show result of game after it is over 
 * 
 * @param score Final game score
 */
void game_over(uint8_t score);

/** @} */

#endif