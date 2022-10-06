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

volatile bool exit_feature = true;

/* External interrupts for buttons*/
ISR (INT0_vect)
{
	left_btn_pressed = true;
	select_up();
	print_menu();
}

ISR (INT1_vect)
{
	right_btn_pressed = true;
	select_down();
	print_menu();
}

ISR (INT2_vect)
{
	if(!exit_feature)
	{
		exit_feature = true;
	}
	else {
		run_option();
	}
}

void start_ADC_print(){
	exit_feature = false;
}

struct menu_page main_page = {
    .title = "Main menu",
    .options = {
        { .name = "First sub-menu", .callback = &change_menu },
        { .name = "Second sub-menu", .callback = &change_menu },
		{ .name = "ADC Measurement", .callback = &change_menu },
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

struct menu_page adc_page = {
	.title = "ADC",
	.options = {
		{ .name = "Main menu",  .callback = &change_menu },
		{ .name = "Print ADC",  .callback = &start_ADC_print }
		
	}
};

void start_menu() {
	// Pointers to callback parameters. TODO: fix this
    main_page.options[0].callback_parameter = &sub_page_1;
    main_page.options[1].callback_parameter = &sub_page_2;
    main_page.options[2].callback_parameter = &adc_page;
    sub_page_1.options[0].callback_parameter = &main_page;
    sub_page_1.options[1].callback_parameter = &sub_page_2;
    sub_page_2.options[0].callback_parameter = &main_page;
    sub_page_2.options[1].callback_parameter = &sub_page_1;
	adc_page.options[0].callback_parameter = &main_page;
    change_menu(&main_page);
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
	sei(); // Enable global interrupts
	printf("Setup done\r\n");
	start_menu();
	
	while(1) {
//   		adc_read(); // Update ADC-values
//   		JS_service();
//   		silder_service();
//  		button_service();
//   		_delay_ms(500);'
	if(!exit_feature){
		adc_read();
		JS_service();
		_delay_ms(500);
	}
	}
}

