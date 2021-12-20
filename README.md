Arduino nano firmware intended to control chicken house systems:
- Door motor (Open/close)
- Light
- Optional plug

Input outputs
=============

Switchs
-------
Each switch is connected to an input. Each also got a light set connected to an output
- In Door down: 11
- In Door up: 9
- In Light on/off: 8
- In Plug on/off: 10


- Out Door down: 7
- Out Door up: 6
- Out Light: A5
- Out Plug: 12

Relays
------

- Motor up: A3
- Motor down: A4
- Light: 5
- Plug: 4

Serial port
===========

The serial port allows to execute commands and send back event

Commands:
- 'U': Door up
- 'D': Door down
- 'S': Door motor stop
- 'L': Light on
- 'l': Light off
- 'P': Plug on
- 'p': Plug off

Event sent from the board:
- 'U': Door is motor is moving up
- 'D': Door is motor is moving down
- 'S': Door is motor is stopped
- 'L': Light is swiched on
- 'l': Light is swiched off
- 'P': Plug is swiched on
- 'p': Plug is swiched off

