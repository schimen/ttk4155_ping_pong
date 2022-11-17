#ifndef OLED_LIB_H_
#define OLED_LIB_H_

#define OLED_COMMAND_ADDRESS 0x1200
#define OLED_DATA_ADDRESS    0x1300

#include <stdio.h>
#include <avr/io.h>
#include <string.h>
#include "fonts.h"
#include "images.h"
#include <avr/pgmspace.h>

/**
 * @brief  OLED API for Node 1
 * @defgroup oled
 * @ingroup oled
 * @{
 * 
*/

/**
 * @brief Set up oled
 * 
 */
void oled_setup(void);

/**
 * @brief Clear oled screen
 * 
 */
void oled_clear();

/**
 * @brief Set selected column
 * 
 * @param column Column between 0 and 7
 */
void oled_set_column(uint8_t column);

/**
 * @brief Set selected line
 * 
 * @param line Line between 0 and 127
 */
void oled_set_line(uint8_t line);

/**
 * @brief Set selected page
 * 
 * @param page Page between 0 and 7
 */
void oled_set_page(uint8_t page);

/**
 * @brief Set selected position
 * 
 * @param page Page between 0 and 7
 * @param column Column between 0 and 127
 */
void oled_set_pos(uint8_t page, uint8_t column);

/**
 * @brief Write command to oled
 * 
 * @param command Oled command
 */
void oled_command_write(uint8_t command);

/**
 * @brief Write data to oled
 * 
 * @param data Oled data
 */
void oled_data_write(uint8_t data);

/**
 * @brief Put character to oled screen
 * 
 * @param c Char character to write
 */
void oled_putchar(uint8_t c);

/**
 * @brief Send character (wrapper for putchar)
 * 
 * @param data Character to write
 * @param file C stream
 * @return int Always returns 0
 */
int oled_send_character(char data, FILE *file);

/**
 * @brief Print string to oled
 * 
 * @param text Array of characters
 */
void oled_print(char *text);

/**
 * @brief Print longer number to oled
 * 
 * @param number Number between 0 and 255
 */
void oled_print_number(uint8_t number);

/**
 * @brief Show image of a troll on OLED
 * 
 */
void show_img_troll();

/**
 * @brief Show image of bird on OLED
 * 
 */
void show_img_bird();

/**
 * @brief Show image of a skyline on OLED
 * 
 */
void show_img_skyline();

/** @} */

/* OLED command defines */
#define CMD_SEGMENT_REMAP_0              0xA0 
#define CMD_SEGMENT_REMAP_127            0xA1 
#define CMD_COM_PINS                     0xDA 
#define COM_PINS_RESET                   0x12 
#define CMD_COM_SCAN_DIRECTION_NORMAL    0xC0 
#define CMD_COM_SCAN_DIRECTION_REMAP     0xC8 
#define CMD_MULTIPLEX_RATIO              0xA8 
#define CMD_DISPLAY_CLOCK_DIVIDE_RATIO   0xD5 
#define DISPLAY_CLOCK_DIVIDE_RESET       0x80 
#define CMD_CONTRAST_CONTROL             0x81
#define CONTRAST_CONTROL_RESET           0x7F
#define CMD_PRE_CHARGE_PERIOD            0xD9
#define PRE_CHARGE_PERIOD_RESET          0x22
#define CMD_MEMORY_ADDRESSING_MODE       0x20
#define MEMORY_ADDRESSING_MODE_RESET     0x02
#define CMD_VCOMH_DESELECT_LEVEL         0xDB
#define VCOMH_DESELECT_LEVEL_RESET       0x20
#define CMD_NOP                          0xE3
#define CMD_IREF_SELECTION               0xAD
#define IREF_EXTERNAL                    0x00
#define IREF_INTERNAL                    0x10
#define CMD_ENTIRE_DISPLAY_ON            0xA4
#define CMD_ENTIRE_DISPLAY_ON_IGNORE_RAM 0xA5
#define CMD_NORMAL_DISPLAY               0xA6
#define CMD_INVERSE_DISPLAY              0xA7
#define CMD_DISPLAY_OFF                  0xAE
#define CMD_DISPLAY_ON                   0xAF


#endif