#include "sram_lib.h"

void sram_setup(void)
{
    // Enable external memory
    MCUCR |= (1 << SRE);
    // Mask unused bits
    SFIOR |= (1 << XMM2);
}

void sram_write(uint16_t address, uint8_t data)
{
    volatile char* ext_ram = (char*)EXRAM_START;
    ext_ram[address] = data;
}

uint8_t sram_read(uint16_t address)
{
    volatile char* ext_ram = (char*)EXRAM_START;
    uint8_t ret_val = ext_ram [address];
    return ret_val;
}

void sram_test(void)
{
    volatile char *ext_ram = (char *)EXRAM_START; // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;//0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
    printf("Starting SRAM test...\n");
    // rand() stores some internal state, so calling this function in a loop will
    // yield different seeds each time (unless srand() is called before this function)
    uint16_t seed = rand();
    // Write phase: Immediately check that the correct value was stored
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
            write_errors++;
        }
    }
    // Retrieval phase: Check that no values were changed during or after the write phase
    srand(seed);
    // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
            retrieval_errors++;
        }
    }
    printf("Completed %4d SRAM tests with \n%4d errors in write phase (%4d%%) and \n%4d errors in retrieval phase (%4d%%)\n\n", ext_ram_size, write_errors, (write_errors*100)/ext_ram_size, retrieval_errors, (retrieval_errors*100)/ext_ram_size);
}