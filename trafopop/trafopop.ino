#include <SPI.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>

#define NUM 100

typedef struct color
{
  byte r, g, b;
}
color;

color pixels[NUM];

byte pointX[NUM] =
{
-12,
-12,
-12,
-12,
-12,
-12,
-12,
-12,
-8,
-4,
-2,
2,
5,
9,
12,
12,
12,
12,
12,
12,
12,
12,
9,
5,
2,
-2,
-5,
-8,
-10,
-10,
-10,
-10,
-10,
-10,
-10,
-6,
-3,
0,
4,
7,
10,
10,
10,
10,
10,
10,
10,
7,
4,
0,
-4,
-6,
-6,
-8,
-8,
-7,
-4,
-1,
2,
5,
7,
8,
8,
6,
4,
2,
-1,
-4,
-4,
-4
-4,
-4,
-4,
-2,
0,
2,
4,
4,
4,
4,
4,
2,
0,
-2,
-2,
-2,
-2,
0,
2,
2,
2,
0,
0,
};

byte pointY[NUM] =
{
-12
,-9
,-5
,-2
,2
,5
,9
,12
,12
,12
,12
,12
,12
,12
,12
,8
,5
,2
,-2
,-5
,-9
,-12
,-12
,-12
,-12
,-12
,-12
,-12
,-10
,-7
,-4
,0
,3
,7
,10
,10
,10
,10
,10
,10
,10
,7
,3
,0
,-4
,-7
,-11
,-11
,-11
,-11
,-11
,-11
,-5
,-2
,-1
,4
,7
,8
,8
,7
,4
,-1
,-2
,-5
,-7
,-8
,-8
,-7
,-4
,-2
,0
,2
,4
,4
,4
,4
,4
,2
,0
,-2
,-4
,-4
,-4
,-4
,-2
,0
,2
,2
,2
,0
,-2
,-2
,0
};

#if 0
inline char normalizedX(byte index)
{
  byte x = index % 20;
  if (x>9)
  {
    x = 19-x;
  }

  return x-5;
}

inline char normalizedY(byte index)
{
  byte y = index / 10;

  return y-5;
}
#else
inline char normalizedX(byte index)
{
  return pointX[index]-5;
}

inline char normalizedY(byte index)
{
  return pointY[index]-5;
}
#endif

typedef struct CGPoint
{
  float x;
  float y;
}
CGPoint;

inline struct CGPoint CGPointMake(float x, float y)
{
  struct CGPoint point;

  point.x = x;
  point.y = y;

  return point;
}

inline float distance(CGPoint a, CGPoint b)
{
  float x = a.x - b.x;
  float y = a.y - b.y;

  //  return sqrt(x*x + y*y);
  return (x*x + y*y);
}

inline CGPoint normalize(CGPoint p)
{
  float length = sqrt(p.x*p.x + p.y*p.y);

  return CGPointMake(p.x/length, p.y/length);
}

inline void draw(float frameCount)
{
  float s = 0.01 * (0.7 + 0.2 * sin(frameCount * 0.000827));
  float r = 2.0 * M_PI * sin(frameCount * 0.000742);

  float time = frameCount * 0.02;

  float sinr = sin(r);
  float cosr = cos(r);

  CGPoint center1 = CGPointMake(cos(time), cos(time*0.535));
  CGPoint center2 = CGPointMake(cos(time*0.259), cos(time*0.605));
  // CGPoint center3 = CGPointMake(cos(time*0.346), cos(time*0.263));
  // CGPoint center4 = CGPointMake(cos(time*0.1346), cos(time*0.1263));
  // float size = (sin(time*0.1)+1.2)*64.0;

  for (byte i = 0; i < NUM; i++)
  {
    float x0 = s * normalizedX(i);
    float y0 = s * normalizedY(i);
    float x = (x0*cosr - y0*sinr);
    float y = (x0*sinr + y0*cosr);


    //   x++;
    y++;

    //  float r = sqrt(x*x+y*y);
    float r = (x*x+y*y);
    float a = atan2(y, x);
    float t = time + 100.0/(r+1.0);
    float s = sin(r+t);

    //float light = 3825.0*fabs(0.05*(sin(t)+sin(time+a*8.0)));
    byte light = 3200;
    float u = -sin(r*2.5-a-time+s);
    float v = sin(r*1.5+a+a-time+s);
    float w = cos(r+a*3+time)-s;
    //float l = sqrt(u*u + v*v + w*w);

    u *= light;
    v *= light;
    w *= light;

    struct color color2 =
    {
      u,v,w 
    };

    pixels[i] = color2;
  }
}

inline void draw2(float frameCount)
{
  float s = 0.003 * (0.7 + 0.2 * sin(frameCount * 0.000827));
  float r = 2.0 * M_PI * sin(frameCount * 0.000742);

  float time = frameCount * 0.002;

  float sinr = sin(r);
  float cosr = cos(r);

  CGPoint center1 = CGPointMake(cos(time), cos(time*0.535));
  CGPoint center2 = CGPointMake(cos(time*0.259), cos(time*0.605));
  // CGPoint center3 = CGPointMake(cos(time*0.346), cos(time*0.263));
  // CGPoint center4 = CGPointMake(cos(time*0.1346), cos(time*0.1263));
  // float size = (sin(time*0.1)+1.2)*64.0;

  for (byte i = 0; i < NUM; i++)
  {
    float x0 = s * normalizedX(i);
    float y0 = s * normalizedY(i);
    float x = (x0*cosr - y0*sinr);
    float y = (x0*sinr + y0*cosr);

    CGPoint position =
    {
      x,y
    };

    int size = 64;
    float d = distance(position, center1)*size;
    CGPoint color = CGPointMake(cos(d),sin(d));
    d = distance(position, center2)*size;
    color.x += cos(d);
    color.y += sin(d);
    /*
    d = distance(position, center3)*size;
     color.x += cos(d);
     color.y += sin(d);
     d = distance(position, center4)*size;
     color.x += cos(d);
     color.y += sin(d);
     */
    float c = sqrt(color.x*color.x+color.y*color.y)*0.25;
    // float c = (color.x*color.x+color.y*color.y);

    CGPoint ncolor = normalize(color);
    /*
    float red = c * ncolor.x;
     float green = c * ncolor.y;
     float blue = c * (-ncolor.x-ncolor.y);
     */
    float red = ncolor.x;
    float green = ncolor.y;
    float blue = c * (ncolor.x-ncolor.y);
    /*
  red = cos(red*3.0+0.5)+sin(red*2.0);
     green = cos(green*3.0+0.5)+sin(green*2.0);
     blue = cos(blue*3.0+0.5)+sin(blue*2.0);
     */
    struct color color2 =
    {
      max(0,red * 255), max(0,green * 255), max(0,blue * 255)
      };

      pixels[i] = color2;
  }
}

void show(byte *bytes, int size)
{
  for (int index=0; index<size; index++)
  {
    byte c = pgm_read_byte(bytes + index);

    // nur 25% Helligkeit
    for (SPDR = c>>2; !(SPSR & _BV(SPIF)););
  }
}

void show2(byte *bytes, int size)
{
  for (int index=0; index<size; index++)
  {
    // nur 25% Helligkeit    
    byte c = bytes[index] >> 2;

    for (SPDR = c; !(SPSR & _BV(SPIF)););
  }
}

#define NUM_STATUS 2
#define NUM_STATES NUM_STATUS

void setup()
{
  unsigned long last = 0;
  unsigned long now;

#if 0
  byte state = (EEPROM.read(0)+1) % (NUM_STATES+1);
  EEPROM.write(0, state);
  byte status = status;
#else
  byte state = EEPROM.read(0);
  Serial.begin(9600);
  byte status = state;
#endif

  status = 1;

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  // SPI.setClockDivider(SPI_CLOCK_DIV16);  // 1 MHz
  SPI.setClockDivider(SPI_CLOCK_DIV8);  // 2 MHz
  // SPI.setClockDivider(SPI_CLOCK_DIV4);  // 4 MHz 
  long framecount = 0;

  while (1)
  {
    if (Serial.available())
    {
      byte data = Serial.read();

      switch (data)
      {
      case '+':
      case '=':
      case '*':
        {
          state++;

          break;
        }

      case '-':
      case '_':
        {
          state += NUM_STATES - 1;

          break;
        }

      default:
        {
          state = data -'0';
        }
      }

      state %= NUM_STATES;

      Serial.write(data);
      EEPROM.write(0, state);

      if (state)
      {
        status = state;
      }
    }

    if (!state)
    {
      now = millis();
      if (now - last > 10000)
      {
        last = now;
        status = status % NUM_STATUS + 1;
      }
    }

    switch (status)
    {
    case 1:
      {
        draw2(framecount++);
        show2((byte *)pixels, sizeof(pixels));

        break;
      }

    case 2:
      {
        draw(framecount++);
        show2((byte *)pixels, sizeof(pixels));

        break;
      }
    }

    delay(2);
  }
}

void loop()
{
}

