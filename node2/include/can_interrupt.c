/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "can_interrupt.h"
#include <stdio.h>
#include "sam.h"
#include "printf_stdarg.h"
#include "can_controller.h"
#include "console_lib.h"
#include "pwm_lib.h"

#define DEBUG_INTERRUPT 1


