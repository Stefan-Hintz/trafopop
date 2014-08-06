#include <SPI.h>
#include <avr/pgmspace.h>

#include "images.h"

#define BRIGHTNESS 3
// #define BRIGHTNESS map(analogRead(A0), 0, 1023, 7, 2)

// delay between images in milliseconds
#define ANIMATION_TIME 1000
// #define ANIMATION_TIME map(analogRead(A1), 0, 1023, 50, 2000)

// true, false
#define ANIMATION_RUNNING_AT_START true 

#define NEXT_FRAME_BUTTON 5

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

void show(byte *bytes, int size)
{
  int inputValue = BRIGHTNESS;

  for (int index = 0; index < size; index++)
  {
    // nur 12.5% Helligkeit
    byte c = pgm_read_byte(bytes + index) >> inputValue;

    for (SPDR = c; !(SPSR & _BV(SPIF)););
  }
}

void show2(byte *bytes, int size)
{
  int inputValue = BRIGHTNESS;

  for (int index = 0; index < size; index++)
  {
    // nur 12.5% Helligkeit    
    byte c = bytes[index] >> inputValue;

    for (SPDR = c; !(SPSR & _BV(SPIF)););
  }
}

void setup()
{
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  // 2 MHz clock frequency
  SPI.setClockDivider(SPI_CLOCK_DIV8);

  digitalWrite(NEXT_FRAME_BUTTON, HIGH);
}

long framecount = 0;
boolean running = ANIMATION_RUNNING_AT_START;
int startStatus = ANIMATION_RUNNING_AT_START ? 0 : -2;
int status = startStatus;
boolean button = false;
long buttonTime = 0;

void loop()
{
  boolean oldButton = button;
  button = !digitalRead(NEXT_FRAME_BUTTON);

  boolean touchUp = (oldButton && !button);

  if (running)
  {
    if (touchUp && buttonTime < 50)
    {
      running = false;
    }
    else
    {
      status++;
      delay(ANIMATION_TIME);
    }
  }
  else
  {
    if (touchUp)
    {
      status++;
    }
  }

  if (button)
  {
    if (buttonTime > 50)
    {
      running = true;
    }
    else
    {
      buttonTime++;
      delay(10);
    }
  }
  else
  {
    buttonTime = 0;
  }

  switch (status)
  {
  default:
    {
      status = startStatus;

      // no break;
    }

#if !ANIMATION_RUNNING_AT_START
  case -2:
    {
      draw2(framecount++);
      show2((byte *)pixels, sizeof(pixels));

      break;
    }

  case -1:
    {
      draw(framecount++);
      show2((byte *)pixels, sizeof(pixels));

      break;
    }
#endif

#include "cases.h"
  }

  delay(2);
}

