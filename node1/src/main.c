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

static bool game_on = false;

void start_game();
void handle_can_msg(struct can_frame *msg);

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
	// Do stuff based on flag
	
	if (flag == MCP_RX0IF || flag == MCP_RX1IF) // Message received on RXB0 or RXB1
	{
		struct can_frame can_rx_msg;
		while (can_receive(&can_rx_msg)) { handle_can_msg(&can_rx_msg); };
	}
	// Clear current interrupt flag
	mcp_bit_modify(MCP_CANINTF,(0xFF & flag), ~(flag));
}

struct menu_page main_page = {
    .title = "Menu",
    .options = {
		{ .name = "Start game", .callback = &start_game },
        { .name = "SRAM test", .callback = &sram_test },
        { .name = "CAN test", .callback = &can_test },
    }
};

void start_game() {
	game_on = true;
	struct can_frame msg;
	msg.id = 2;
	msg.len = 2;
	msg.data[0] = 1;
	msg.data[1] = 0;
	can_write(&msg);
	game_menu(0);
}

void stop_game() {
	struct can_frame msg;
	msg.id = 2;
	msg.len = 2;
	msg.data[0] = 0;
	msg.data[1] = 0;
	can_write(&msg);
	game_over(0);
	game_on = false;
	change_menu(&main_page);
}

void start_menu() {
    change_menu(&main_page);
}

void menu_service(uint8_t direction, bool run_option_bool){
	if (game_on) {
		return;
	}
	switch (direction)
	{
		case UP:
			select_up();
			print_menu();
			_delay_ms(100);
			break;
		case DOWN:
			select_down();
			print_menu();
			_delay_ms(100);
			break;
	}
	if (run_option_bool) {
		run_option();
	}
}

void handle_can_msg(struct can_frame *msg) {
	// Game message
	if (msg->id == 2) {
		if (msg->data[0] == 0) {
			game_over(msg->data[1]);
			game_on = false;
			change_menu(&main_page);
		}
		// Game is on, update score
		else {
			game_menu(msg->data[1]);
		}
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
			// Send event data to node 2 when game is on
			if (game_on) {
				write_event_data(
					event_msg.data, left_btn_pressed, right_btn_pressed
				);
				can_write(&event_msg);
				
				if (left_btn_pressed) {
					stop_game();
				}

			}
			// React to event in menu when game is not on
			else {
				menu_service(joystick_direction(), left_btn_pressed);
			}
			left_btn_pressed = false;
			right_btn_pressed = false;
		}
		_delay_ms(50);
	}
}

