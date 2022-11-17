.. _byggern-home:

Byggern 2022 documentation
############################

Welcome to the documentation of TTK4155 Ping Pong source code.
This is the API documentation of the Ping Pong game developed in the course
TTK4155 (also known as "Byggern").

The Ping Pong game is built on two modules with each own source code that
independently controls part of the system and communicates over CAN.

Node 1 receives control data and sends these to Node 2.
Node 2 controls the Ping-Pong board.
The Ping-Pong board has a servo, a solenoid and a motor that you can play
ping-pong with.

The source code can be read at GitHub_

.. _GitHub:  https://github.com/schimen/ttk4155_ping_pong

.. raw:: latex

    \clearpage

.. figure:: ./images/node1.jpg
   :width: 50%
   :alt: node1

   Node 1

.. figure:: ./images/node2.jpg
   :width: 50%
   :alt: node2

   Node 2

.. figure:: ./images/ping_pong_board.jpg
   :width: 50%
   :alt: ping_pong_board

   Ping Pong board

.. raw:: latex

    \clearpage

Nodes
********

.. toctree::
   :maxdepth: 3
   :caption: Nodes

   ./node1/index.rst
   ./node2/index.rst
