# ardu-pic-628
A simple Arduino programmer for PIC16F628A based on jaromir-sukuba's a-p-prog.

# Programmer for PIC16F628A devices built upon AVR (or Arduino)

I needed a way to program a PIC16F628A. I used [jaromir-sukuba's a-p-prog](https://github.com/jaromir-sukuba/a-p-prog) to program some other PICs in the past. I therefore took the code and modifed it in order to allow me to program my PIC16F628A. The resulting software is capable of programming **ONLY** the PIC16F628A. Please visit jaromir's repo for any other PIC.

I did not keep any structure in this project. It is the minimum required to make programming the PIC possible.

## Hardware
There are two options fo hardware for this project:

### Hardware option 1 - Arduino hardware
Take any Arduino with ATmega328P, like Uno or most of cheap chinese knock-off boards. Considering the target has its own power supply, connect GND, MCLR, PGC and PGD lines to respective pins on arduino as follows:
The PIC16F628 needs some extra connections. **ONLY HIGH VOLTAGE PROGRAMMING IS SUPPORTED!!**
| Arduino pin | AVR pin | Target PIC pin | Comment                                                                     |
|-------------|---------|----------------|-----------------------------------------------------------------------------|
| GND         | GND     | GND            | common ground                                                               |
| GND         | GND     | PGM            | PGM might need to be connected to GND to enter HV programming correctly     |
| A3          | PC3     | MCLR*          | reset line of PIC MCU                                                       |
| A2          | PC2     | VDD            |    power supply for PIC MCU, you may **NOT** power target from other source |
| A1          | PC1     | PGD            | programming data line                                                       |
| A0          | PC0     | PGC            | programming clock line                                                      |

* In order for HV programming to work, when Arduino pulls A3 LOW, 12V must reach MCLR on the PIC. A possible schematic is shown below.
* 
![image](https://github.com/vlad-plavat/ardu-pic-628/assets/101560676/261495df-0e70-4cf1-ba33-9e5b2e44facb)

### Firmware

The firmware (Arduino code) is also taken from jaromir. It is heavily modified to support strictly what is necessary. It communicates serially using 57600 BAUD. The commands are the following:
* in order to perform a simple READ operation, send a single 'r' character. The Arduino will read the entire memory contents and dump it via serial(16 words per line). In the end the user ID and configuration bytes will be dumped.
* in order to perform a write, first write a single 'w' character. Then the Arduino expects the first word (2 bytes) to be programmed, then sends a response byte with the value 0. Then it will wait for the next word and so on. This will repeat 2048 times (The PIC16F628A has 2048 words of FLASH). Afterwards, the FLASH memory is read and dumped to serial. Then the configuration word must be sent. In the end, the configuration word is read back and sent back over serial.
* After either operation, the PIC is left in a running state, and the Arduino can accept another command. If an error occurs, the Arduino must be reset as there is no timeout implemented.

## Software

I heavily modified the software (pp3) from jaromir's project. I followed absolutely no structure. The code is in a working state, but it is not anymore compatible with the original project.

I uploaded the C cource and a compiled executable. It works similar to jaromir's.

Running the executable with no parameters should only bring banner "pp programmer". The tested options/parameters are as follows:

test -c COM7 -t 16f628 filename -n -p
*-c specifies the serial port
*-t specifies the part. It **MUST** be 16f628, or else the program won't work
*filename is the file to be programmed
*-n disables verifying
*-p disables programming

If you only want to read the program you should use -n and -p together.



## Supported devices

This project had the only goal to program a PIC16F628A. I posted it in case somebody might need it. Currently, only PIC16F628A is supported. However, it should not be too complicated to add support for other devices.
