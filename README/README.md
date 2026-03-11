# rc-car-remote
My RC car remote control PCB design and future development 


Hey there! I'm Jonas, a 17-year old high school student from Germany, currently preparing for my Abitur next year.  

For my final school project, my friend and I decided to build a custom RC car but I wanted to go beyond the usual Arduino + breadboard setup my classmates use. So I designed a custom PCB based remote control. 

Remote Control Features: 

ESP32 microcontroller with custom 6 axis control (analog joystick + 6 buttons)  

0.91" OLED display for real time telemetry 

Clean 2-layer PCB (my first ever custom PCB design) 

Compact enclosure (designed, 3D printing pending PCB dimensions) 

Wireless System: 

ESP NOW protocol for low latency RC communication  

One ESP32 in remote and one ESP32 in car 

Structured 8-byte data packets (throttle, steering, 7 button states) 

 Current Status: The remote control hardware & firmware are fully functional  

PCB designed in KiCad, Gerber files ready for production 

Firmware reads all inputs, displays on OLED, ready for ESP-NOW transmission 

The RC car receiver + motors/servos are bought but not assembled yet  

Car ESP32 firmware is a work in progress skeleton  

Full car assembly after PCB arrives and remote is verified working 

Learning Goals: This is my first custom PCB project  I learned KiCad schematic/PCB layout, ESP32 programming, I2C OLED integration. I tried my best for the design but I'm open to feedback for my next iteration! 
