#define F_CPU 4915200

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "util/delay.h"
#include "uart_lib.h"
#include "sram_lib.h"
#include "js_slider.h"
#include "oled_lib.h"
#include "oled_menu.h"
#include "mcp2515_lib.h"
#include "can_lib.h"
#include "interrupt.h"

volatile bool left_btn_pressed = false;
volatile bool right_btn_pressed = false;

/* External interrupts for buttons*/
ISR (INT0_vect) //LEFT USB-button
{
	left_btn_pressed = true;
}

ISR (INT1_vect) //RIGHT USB-button
{
	right_btn_pressed = true;
}

ISR (INT2_vect)
{
	uint8_t flag = 0b00000011 & mcp_read_byte(MCP_CANINTF); //Holds current interrupt flag
	printf("MCP Interrupt on flag: %02x \r\n", flag);
	// Do stuff based on flag
	
	if (flag == MCP_RX0IF)
	{
		printf("Message received on RXB0");
		
		struct can_frame can_rx_msg;
		while (can_receive(&can_rx_msg))
		{
			printf("message received (%d length): ", can_rx_msg.len);
			for (uint8_t i = 0; i < can_rx_msg.len; i++) {
				printf("%d, ", can_rx_msg.data[i]);
			}
			printf(" from id %d\n", can_rx_msg.id);
		}
		printf("\n");
	}
	if (flag == MCP_RX1IF)
	{
		printf("Message received on RXB1");
	}
	
	// Clear current interrupt flag
	mcp_bit_modify(MCP_CANINTF,(0xFF & flag), ~(flag));
}

struct menu_page main_page = {
    .title = "Menu",
    .options = {
        { .name = "SRAM test", .callback = &sram_test },
        { .name = "CAN test", .callback = &can_test },
    }
};

void start_menu() {
    change_menu(&main_page);
}

void menu_service(uint8_t direction, bool run_option_bool){
	
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
	if (run_option_bool) {
		run_option();
	}
}

int main(void)
{
	uart_setup();
	/*Enable printf */
	fdevopen(send_character, receive_character);
	sram_setup();
	interrupt_setup();
	oled_setup();
	calibrate_joystick();
	spi_setup();
	can_setup();
	sei(); // Enable global interrupts
	start_menu();
	printf("Setup done\r\n");

	struct can_frame event_msg;
	event_msg.len = 3;
	event_msg.id = 1;
	while(1) {
		// Update ADC-values
   		js_slider_update();
		// Act on new input at new event
		if (new_event(left_btn_pressed, right_btn_pressed)) {
			// React to event in menu
			menu_service(joystick_direction(), left_btn_pressed);
			// Send event data to node 2
			write_event_data(
				event_msg.data, left_btn_pressed, right_btn_pressed
			);
			left_btn_pressed = false;
			right_btn_pressed = false;
			can_write(&event_msg);
		}
		_delay_ms(100);
	}
}

