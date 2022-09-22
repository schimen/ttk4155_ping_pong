#include "oled_lib.h"

void oled_setup(){
    oled_command_write(CMD_DISPLAY_OFF);
    oled_command_write(CMD_SEGMENT_REMAP_127); 
    oled_command_write(CMD_COM_PINS);
    oled_command_write(COM_PINS_RESET);

    //oled_command_write(CMD_COM_SCAN_DIRECTION_NORMAL); // scam from COM0 to COM63
    oled_command_write(CMD_COM_SCAN_DIRECTION_REMAP); // scan from COM63 to COM0
    oled_command_write(CMD_MULTIPLEX_RATIO); 
    oled_command_write(0x3f); // multiplex ratio 63

    oled_command_write(CMD_DISPLAY_CLOCK_DIVIDE_RATIO);
    oled_command_write(DISPLAY_CLOCK_DIVIDE_RESET); // divide ratio 1, osc. frq. 1000b

    oled_command_write(CMD_CONTRAST_CONTROL);
    //oled_command_write(0x50);
    oled_command_write(CONTRAST_CONTROL_RESET); // contrast 0x7F

    oled_command_write(CMD_PRE_CHARGE_PERIOD);
    oled_command_write(PRE_CHARGE_PERIOD_RESET); // phase 1 = 2, phase 2 = 2
    //oled_command_write(0x21);

    oled_command_write(CMD_MEMORY_ADDRESSING_MODE);
    oled_command_write(MEMORY_ADDRESSING_MODE_RESET); // page addressing mode

    oled_command_write(CMD_VCOMH_DESELECT_LEVEL);
    oled_command_write(VCOMH_DESELECT_LEVEL_RESET); // 0.77*Vcc
    //oled_command_write(0x30);

    oled_command_write(CMD_IREF_SELECTION);
    oled_command_write(IREF_EXTERNAL);

    oled_command_write(CMD_ENTIRE_DISPLAY_ON); // entire display on, follow RAM
    oled_command_write(CMD_NORMAL_DISPLAY);
    oled_command_write(CMD_DISPLAY_ON);

    oled_clear();
    oled_set_column(0);
    oled_set_line(0);
}


void oled_set_column(uint8_t column){
    uint8_t lower_column = column & 0xF;
    uint8_t upper_column = (column >> 4) | 0x10;

    oled_command_write(lower_column);
    oled_command_write(upper_column);
}

void oled_set_line(uint8_t line){
    uint8_t command = (line & 0x7F) | 0x40;
    oled_command_write(command);
}

void oled_set_page(uint8_t page){
    uint8_t command = (page & 0x07) | 0xB0;
    oled_command_write(command);
}

void oled_set_pos(uint8_t page, uint8_t column){
    oled_set_page(page);
    oled_set_column(column);
}

void oled_command_write(uint8_t command){
    volatile char* oled = (char*) OLED_COMMAND_ADDRESS;
    oled[0] = command;
}

void oled_data_write(uint8_t data){
    volatile char* oled = (char*) OLED_DATA_ADDRESS;
    oled[0] = data;
}

void oled_putchar(uint8_t c){
    if (c == '\n') {
        //oled_set_page(0);
    }
    else if (c == '\r') {
        oled_set_column(0);
    }
    else {
        uint8_t font_index = c - 32;
        for (uint8_t i = 0; i < 8; i++){
            oled_data_write(pgm_read_word(&font8[font_index][i]));
        }
    }
}

void oled_clear(){
    for (uint8_t i = 0; i < 127; i++){
        for (uint8_t j = 0; j < 8; j++){
            oled_set_pos(j, i);
            oled_data_write(0);
        }
    }
}

int oled_send_character(char data, FILE * file)
{
    oled_putchar((uint8_t)data);
    return 0;
}
