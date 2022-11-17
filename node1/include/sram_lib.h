#ifndef SRAM_LIB_H_
#define SRAM_LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>


#define EXRAM_START 0x1800

void sram_setup();

void sram_write(uint16_t address, uint8_t data);

uint8_t sram_read(uint16_t address);

void sram_test();

#endif