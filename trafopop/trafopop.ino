#include <SPI.h>

#define NUM sizeof(positions)/sizeof(Point)

typedef struct Point
{
  char x;
  char y;
}
Point;

#include "positions.h"

typedef struct color
{
  byte r, g, b;
}
color;

color pixels[NUM];

typedef struct CGPoint
{
  float x;
  float y;
}
CGPoint;

void show2(byte *bytes, int size)
{
  for (int index = 0; index < size; index++)
  {
    // nur 12.5% Helligkeit    
    byte c = bytes[index] >> 3;

    for (SPDR = c; !(SPSR & _BV(SPIF)););
  }
}

void setup()
{
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  // SPI.setClockDivider(SPI_CLOCK_DIV16);  // 1 MHz
  SPI.setClockDivider(SPI_CLOCK_DIV8);  // 2 MHz
  // SPI.setClockDivider(SPI_CLOCK_DIV4);  // 4 MHz 
}

long framecount = 0;

void loop()
{
  draw2(framecount++);
  show2((byte *)pixels, sizeof(pixels));

  delay(2);
}

