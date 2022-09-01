#include <avr/io.h>
#include <util/delay.h>

const uint8_t LED_MASK = (1 << PD5);

int main()
{
    // set pin as output
    DDRD = LED_MASK;  
    while(1)
    {
        // toggle led
        PORTD ^= LED_MASK;
        _delay_ms(100);
    }
    return 0;
}