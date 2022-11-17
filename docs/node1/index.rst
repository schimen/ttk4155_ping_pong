.. _node1-home:

Documentation for Node 1
############################

Node 1 contains an AVR162 microcontroller that gathers sensor data for the
game and sends these to the game controller (Node 2).

Node 1 also contains a menu displayed on an oled screen. 
This menu is used to start the game or do hardware tests.

Modules
********

.. toctree::
   :maxdepth: 1
   :caption: Modules
   
   uart_node1.rst
   can.rst
   oled.rst
   js_slider.rst
   sram.rst

.. raw:: latex

    \clearpage