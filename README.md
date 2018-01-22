# 3dPrinterFanController

This quick code uses an Arduino Beetle Board, a DS18b20 Temperature Probe, a small switch, a 12V fan, and an Adafruit SSD1306 to control the temperature of a 3d Printer Enclosure.

There are two set temps, one for PLA and one for ABS. The idea is to keep the enclosure as close to room temperature when printing PLA and to let the enclosure warm up for printing ABS. The set point for ABS is high enough to provide some consistency, yet low enough to hopefully keep anything printed in PLA inside the enclosure from warping due to heat.

The code is rather quite simple. It grabs the temperature from the temp probe every iteration of the loop. After 10 milliseconds it computes the average temperature since the last loop. It then turns the fan on or off.

The display updates every 10 milliseconds after the new average is computed.

When first powering on, the display shows the previous buffer. I'm not going to bother to change this.

I have the output from the Beetle trigger a relay for sending the 12V to the fan. You could probably do this with transistors, but I had the relay laying around.

The fan I used is a 80mm Panaflow, but any 12V fan would be fine. You want the fan sucking hot air out of the top of the enclosure. You do not want it so powerful that it provides a strong breeze through the enclosure.

I've added a picture of my test board.

Pins Used:

* SDA
* SDL
* 5V
* GND
* D9 : Relay Output
* D10 : One wire bus (DS18B20)
* D11 : Switch input

I drive my switch with 5V.

I like to use PlatformIO for my development. If you grab the entire tree, then the first compile should pull down the necessary libraries for the build to complete.
