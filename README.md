# AVR microcontlorers
- [Access to microcontroller ports](https://github.com/akLeszek/Arduino_ATmega328/blob/main/Blink.ino)
  - Code with 3 functions:
    - **pinMode()** - takes one of the two values ***INPUT*** or ***OUTPUT***. Designates the direction of a specific port line.
    - **digitalWrite()** - takes one of the two values ***LOW*** or ***HIGH***. The function is to set the state of a specific port line.
    - **digitalRead()** - return the state of the port line ***0 - low***, ***1 - high***.
- [USART interface](https://github.com/akLeszek/Arduino_ATmega328/blob/main/usart.ino)
  - Assumptions:
    - Baud rate 9600
    - No parity, 1 stop bit, 8 data bits.
  - Initialization of the USART interface.
  - **serialWrite()** - function to send a char.
  - **serialPrintWrite()** - function to send a string.
  - **serialPrintInit()** - function for sending values in decimal mode.
  - Data byte receiving interrupt service, ***ISR(USART_RXC_vect)***
- [External interrupts](https://github.com/akLeszek/Arduino_ATmega328/blob/main/External_interrupt.ino)
  - Handling of interrupts coming from lines **INT0** and **INT1**:
    - **INT0** - interrupt on falling edge of the signal.
    - **INT1** - interrupt when the logical state is changed.
- [ADC](https://github.com/akLeszek/Arduino_ATmega328/blob/main/ADC.ino)
  - Assumptions:
    - AVcc set as reference voltage.
  - Cyclic readings on the selected ADC channel, with the possibility of any switching between ADC channels (0 ... 5).
  - Sending the measurement result using the USART interface and presenting it in millivolts (accuracy 10mV).
- [LCD display](https://github.com/akLeszek/Arduino_ATmega328/blob/main/LCD.ino)
  - Displaying the text on the display which is more than 16 characters and then using the sliding window mechanism will display all the text:
    - **lcdShiftLeft()** - shift the screen to the left,
    - **lcdShiftRight()** - shift the screen to the right. 
    - **lcdCreateChar()** - create and display a defined custom character (e.g. heart). 