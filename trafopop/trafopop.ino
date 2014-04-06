#include <SPI.h>
#include <avr/pgmspace.h>

#include "image0.h"
#include "image1.h"
#include "image2.h"
#include "image3.h"
#include "image4.h"
#include "image5.h"
#include "image6.h"
#include "image7.h"
#include "image8.h"
#include "image9.h"

#define NEXT_FRAME_BUTTON 5
#define ANIMATION_TIME 1000

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
  for (int index = 0; index < size; index++)
  {
    // nur 12.5% Helligkeit
    byte c = pgm_read_byte(bytes + index) >> 3;

    for (SPDR = c; !(SPSR & _BV(SPIF)););
  }
}

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

  digitalWrite(NEXT_FRAME_BUTTON, HIGH);
}

long framecount = 0;
boolean running = true;
byte status = 0;
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
  case 0:
    {
      show(image0, sizeof(image0));

      break;
    }

  case 1:
    {
      show(image1, sizeof(image1));

      break;
    }

  case 2:
    {
      show(image2, sizeof(image2));

      break;
    }

  case 3:
    {
      show(image3, sizeof(image3));

      break;
    }

  case 4:
    {
      show(image4, sizeof(image4));

      break;
    }

  case 5:
    {
      show(image5, sizeof(image5));

      break;
    }

  case 6:
    {
      show(image6, sizeof(image6));

      break;
    }

  case 7:
    {
      show(image7, sizeof(image7));

      break;
    }

  case 8:
    {
      show(image8, sizeof(image8));

      break;
    }

  case 9:
    {
      show(image9, sizeof(image9));

      break;
    }

  case 10:
    {
      draw2(framecount++);
      show2((byte *)pixels, sizeof(pixels));

      break;
    }

  case 11:
    {
      draw(framecount++);
      show2((byte *)pixels, sizeof(pixels));

      break;
    }

  default:
    {
      status = 0;

      break;
    }
  }

  delay(2);
}

