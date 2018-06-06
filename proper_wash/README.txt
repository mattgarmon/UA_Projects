(c) Matt Garmon - 2017-2018

Project Description:
Proper Wash is a temperature and time monitored handwashing device that consists of an 
LCD display screen, a sensor module mounted to the faucet, and two hydroelectric generators. 
Not only hand soap plays a role in a proper hand wash, water temperature and duration of 
washing also play a major role in it too. This device will improve current standard hand 
washing stations by guiding the user through the proper process of hand washing with 
intuitive status indicators and instructions. It will be powered on when the faucet is 
turned on, and will time the hand washing cycle only when the water is at the desired 
temperature and hands are detected in the sink. This device is also adaptable to almost 
any faucet, and features temperature display in Fahrenheit and Celsius to ensure usability. 
Adding these functionalities are important in the hand washing process for food and health 
service individuals alike.

Software Theory of Operation:

The software will interface with the sensor readings to provide relevant instructions and 
information to the user. It will guide a user through the proper hand washing cycle by 
instructing through a soap lathering cycle, temperature requirement checking, and hand 
rinse cycle. The software is responsible for the timing and information sent to the various 
user interfaces. The most important being an LCD display which will show temperature, timers, 
and instructions. As well as LED indicators for water temperature visualization and a buzzer 
to sound when the hand washing cycle is finished. Overall, the software will provide the 
logic for receiving sensor information and using them to implement a proper hand wash.

Compiling and running:
This code is intended to compiled with XC8 in MPLABX and ran on a pic microcontroller. 
The PIC16F1827 was chosen due to its low power consumption and various functionalities.