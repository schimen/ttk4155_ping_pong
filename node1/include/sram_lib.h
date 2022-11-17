#ifndef SRAM_LIB_H_
#define SRAM_LIB_H_

/**
 * @brief SRAM API for node 1
 * @defgroup sram
 * @ingroup sram
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

/** Start address for SRAM in ATMega162 external memory */
#define EXRAM_START 0x1800

/**
 * @brief Setup of external SRAM for ATMega162 
 * 
 */
void sram_setup();


/**
 * @brief Write byte to SRAM address
 * 
 * @param address SRAM address
 * @param data Byte to write
 */
void sram_write(uint16_t address, uint8_t data);


/**
 * @brief Read byte from SRAM address
 * 
 * @param address SRAM address
 * @return uint8_t received byte
 */
uint8_t sram_read(uint16_t address);


/**
 * @brief Stress test of the SRAM
 * 
 */
void sram_test();

/** @} */

#endif