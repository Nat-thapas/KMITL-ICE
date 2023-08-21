// Including the required Arduino libraries
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// CS_PIN 5 (Must be PWN capable)
// DIN_PIN 11 (Does not seem to be configurable)
// CLK_PIN 13 (Does not seem to be configurable)

// Uncomment according to your hardware type
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
//#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

// Defining size, and output pins
#define MAX_DEVICES 4
#define CS_PIN 5

// Create a new instance of the MD_Parola class with hardware SPI connection
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup() {
	// Intialize the object
	myDisplay.begin();

	// Set the intensity (brightness) of the display (0-15)
	myDisplay.setIntensity(4);

	// Clear the display
	myDisplay.displayClear();
}

void loop() {
	myDisplay.setTextAlignment(PA_CENTER);
    myDisplay.print("Hello");
    delay(1000);
    myDisplay.print("World!");
    delay(1000);
}
