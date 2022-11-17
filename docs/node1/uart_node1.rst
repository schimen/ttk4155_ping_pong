.. _node1_uart:

UART Node 1
############################

The UART in Node 1 can be connected via a RS232 converter over serial 
to the desktop computers at the lab.

The UART is configured with 9600 baud.
The UART can be used via the `printf` function.
This example shows how to use `printf` with the UART API:
.. code-block:: c
    uart_setup();
    fdevopen(send_character, receive_character);
    printf("Hello world");


API Reference
**************

.. doxygengroup:: uart