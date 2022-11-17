.. _node1_can:

CAN Node 1
############################

Node 1 CAN library communicates with a MCP2515 can transceiver.
The API to communicate over CAN includes functions for using this transceiver, 
SPI and basic CAN functions.

The Highes level usage for this library is via the `can_frame` struct, this
can be sent via the can API.

API Reference
**************

.. doxygengroup:: can

.. doxygengroup:: mcp2515

.. doxygengroup:: spi