#include "GPIOConfig.h"

#include <SmingCore.h>

const unsigned int GPIOConfig::safe_pins[] = {2,3,4,5,13,14,15,16};

/**
https://tttapa.github.io/ESP8266/Chap04%20-%20Microcontroller.html
Label	GPIO	Input	Output	Notes
A0	ADC0	Analog Input	X	
D3	GPIO0	pulled up	OK	connected to FLASH button, boot fails if pulled LOW
TX	GPIO1	TX pin	OK	HIGH at boot debug output at boot, boot fails if pulled LOW
D4	GPIO2	pulled up	OK	HIGH at boot connected to on-board LED, boot fails if pulled LOW
RX	GPIO3	OK	RX pin	HIGH at boot
D2	GPIO4	OK	OK	often used as SDA (I2C)
D1	GPIO5	OK	OK	often used as SCL (I2C)
D6	GPIO12	OK	OK	SPI (MISO)
D7	GPIO13	OK	OK	SPI (MOSI)
D5	GPIO14	OK	OK	SPI (SCLK)
D8	GPIO15	pulled to GND	OK	SPI (CS) Boot fails if pulled HIGH
D0	GPIO16	no interrupt	no PWM or I2C support	HIGH at boot used to wake up from deep sleep
*/
bool GPIOConfig::isSafeToUse(unsigned int pin) {
	return std::find(std::begin(GPIOConfig::safe_pins), std::end(GPIOConfig::safe_pins), pin) != std::end(GPIOConfig::safe_pins);
}