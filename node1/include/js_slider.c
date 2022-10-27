/*
 * js_slider.c
 *
 * Created: 19.09.2022 18:00:31
 *  Author: lineh
 */

#include "js_slider.h"
#define TRIGGER_POINT 70 // percent

static struct Slider_t slider;
static struct Joystick_t joystick;

bool new_joystick_direction() {
  static uint8_t old_direction = 0;
  if (old_direction != joystick.direction) {
    old_direction = joystick.direction;
    return true;
  } else {
    return false;
  }
}

uint8_t joystick_direction() { return joystick.direction; }

bool new_slider_left() {
  static uint8_t old_slider = 0;
  if (old_slider != slider.left_pos) {
    old_slider = slider.left_pos;
    return true;
  } else {
    return false;
  }
}

bool new_slider_right() {
  static uint8_t old_slider = 0;
  if (old_slider != slider.right_pos) {
    old_slider = slider.right_pos;
    return true;
  } else {
    return false;
  }
}

uint8_t left_slider_pos() { return slider.left_pos; }

uint8_t right_slider_pos() { return slider.right_pos; }

/* Converts adc-value of the joystick to a percentage*/
int8_t JS_adc_to_percent(uint8_t adc_val, uint8_t default_val) {
  if (adc_val < default_val) // LEFT or DOWN, Values: 0% to -100%
  {
    return -(100 - ((adc_val * 100) / default_val));
  } else if (adc_val > default_val) // RIGHT or UP, Values: 0% to 100%
  {
    return ((adc_val - default_val) * 100) / (255 - default_val);
  } else // DEFAULT
  {
    return 0;
  }
}

/* Percent-value of the joystick to a direction */
uint8_t JS_percent_to_direction(int8_t x_val, int8_t y_val) {
  if (x_val < -TRIGGER_POINT) {
    return LEFT;
  } else if (x_val > TRIGGER_POINT) {
    return RIGHT;
  } else if (y_val < -TRIGGER_POINT) {
    return DOWN;
  } else if (y_val > TRIGGER_POINT) {
    return UP;
  } else {
    return DEFAULT;
  }
}

/* Converts joystick value from ADC to a direction and returns it */
uint8_t get_JS_direction(void) {
  /*Converts adc-value to position in percent*/
  joystick.x_pos = JS_adc_to_percent(joystick.x_adc, joystick.x_default);
  joystick.y_pos = JS_adc_to_percent(joystick.y_adc, joystick.y_default);
  // printf("Joystick X:%d Y:%d \r\n", joystick.x_pos, joystick.y_pos);

  /* Returns which direction the joystick is pointing */
  return JS_percent_to_direction(joystick.x_pos, joystick.y_pos);
}

/* Silder ADC-value to percentage */
uint8_t get_slider_position(uint8_t val) { return (val * 100) / 255; }

void js_slider_update(void) {
  volatile char *adc = (char *)ADC_START_ADR;

  // Hardwire mode: sample all channels
  adc[0] = (uint8_t)0;
  _delay_ms(10);

  // Read result from all analog channels
  joystick.y_adc = adc[0];
  joystick.x_adc = adc[0];
  slider.left_adc = adc[0];
  slider.right_adc = adc[0];

  slider.left_pos = get_slider_position(slider.left_adc);
  slider.right_pos = get_slider_position(slider.right_pos);
  joystick.direction = get_JS_direction();
}

/* Averages 10 adc-measurements to set the default adc-values for the joystick
 * in default position */
void calibrate_joystick(void) {
  uint16_t x_sum_adc = 0;
  uint16_t y_sum_adc = 0;

  for (int x = 0; x < 10; x++) {
    js_slider_update();
    x_sum_adc += joystick.x_adc;
    y_sum_adc += joystick.y_adc;
  }

  joystick.x_default = (x_sum_adc / 10);
  joystick.y_default = (y_sum_adc / 10);
}

bool new_event(bool left_btn_pressed, bool right_btn_pressed) {
	// Return true if anye  value changed
	return (new_slider_left() || new_slider_right() || new_joystick_direction() ||
		left_btn_pressed || right_btn_pressed);
}

void write_event_data(uint8_t *buffer, bool left_btn_pressed, bool right_btn_pressed) {
	// Bitmanipulate joystick and button events to first byte
	uint8_t button_js_byte =
		(((uint8_t)left_btn_pressed & 0x01) << 4)  | 
		(((uint8_t)right_btn_pressed & 0x01) << 3) |
		(joystick_direction() & 0x07);
	buffer[0] = button_js_byte;
	buffer[1] = left_slider_pos();
	buffer[2] = right_slider_pos();
}
