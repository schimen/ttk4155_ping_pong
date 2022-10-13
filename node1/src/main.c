#define F_CPU 4915200

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "util/delay.h"
#include "uart_lib.h"
#include "sram_lib.h"
#include "adc_lib.h"
#include "js_slider.h"
#include "oled_lib.h"
#include "oled_menu.h"
#include "mcp2515_lib.h"

/* External interrupts for buttons*/
ISR (INT0_vect) //LEFT USB-button
{
	left_btn_pressed = true;
}

ISR (INT1_vect) //RIGHT USB-button
{
	run_option(); //select menu-item
}

ISR (INT2_vect)
{
	//Interrupt handler for MCP2515 goes here
}

struct menu_page main_page = {
    .title = "Main menu",
    .options = {
        { .name = "First sub-menu", .callback = &change_menu },
        { .name = "Second sub-menu", .callback = &change_menu },
		{ .name = "Exit", .callback = &oled_clear }
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

void start_menu() {
	// Pointers to callback parameters. TODO: fix this
    main_page.options[0].callback_parameter = &sub_page_1;
    main_page.options[1].callback_parameter = &sub_page_2;
    sub_page_1.options[0].callback_parameter = &main_page;
    sub_page_1.options[1].callback_parameter = &sub_page_2;
    sub_page_2.options[0].callback_parameter = &main_page;
    sub_page_2.options[1].callback_parameter = &sub_page_1;
    change_menu(&main_page);
}

void menu_service(uint8_t direction){
	
	switch (direction)
	{
		case DEFAULT:
			//printf("DEFAULT\r\n");
			break;
		case RIGHT:
			//printf("RIGHT\r\n");
			break;
		case LEFT:
			//printf("LEFT\r\n");
			break;
		case UP:
			//printf("UP\r\n");
			select_up();
			print_menu();
			_delay_ms(100);
			break;
		case DOWN:
			//printf("DOWN\r\n");
			select_down();
			print_menu();
			_delay_ms(100);
			break;
	}
}

int main(void)
{
	uart_setup();
	/*Enable printf */
	fdevopen(send_character, receive_character);
	sram_setup();
	button_setup();
	oled_setup();
	calibrate_joystick();
	spi_setup();
	//mcp2515_setup_loopback();
	sei(); // Enable global interrupts
	printf("Setup done\r\n");
	
	start_menu();
	uint8_t status;
	while(1) {
// 		status = mcp2515_read_status();
// 		printf("Status: %d\n", status);
		//mcp2515_setup_loopback();
		_delay_ms(250);

   		//adc_read(); // Update ADC-values
   		//uint8_t JS_pos = get_JS_direction();
		//menu_service(JS_pos);

//   		silder_service();
//  		button_service();
//   		_delay_ms(500);'
	}
}

