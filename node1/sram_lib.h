#ifndef SRAM_LIB_H_
#define SRAM_LIB_H_

#define EXRAM_START 0x1800

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

void sram_setup(void);

void sram_write(uint16_t address, uint8_t data);

uint8_t sram_read(uint16_t address);

void sram_test(void);

#endif