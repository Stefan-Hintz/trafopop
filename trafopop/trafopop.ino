#include <SPI.h>

#define NUM 50

// 1 .. 255
#define BRIGHTNESS 15

typedef struct color
{
  byte r, g, b;
}
Color;

Color pixels[NUM];

inline char normalizedX(byte index)
{
  return index/10;
}

inline char normalizedY(byte index)
{
  return index%10;
}

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
  float s = 0.001 * (0.7 + 0.2 * sin(frameCount * 0.000827));
  float r = 2.0 * M_PI * sin(frameCount * 0.000742);

  float time = frameCount * 0.002;

  float sinr = sin(r);
  float cosr = cos(r);

  // CGPoint center1 = CGPointMake(cos(time), cos(time*0.535));
  // CGPoint center2 = CGPointMake(cos(time*0.259), cos(time*0.605));
  // CGPoint center3 = CGPointMake(cos(time*0.346), cos(time*0.263));
  // CGPoint center4 = CGPointMake(cos(time*0.1346), cos(time*0.1263));
  // float size = (sin(time*0.1)+1.2)*64.0;

  for (byte i = 0; i < NUM; i++)
  {
    float x0 = s * normalizedX(i);
    float y0 = s * normalizedY(i);
    float x = (x0*cosr - y0*sinr);
    float y = (x0*sinr + y0*cosr);

    // x++;
    y++;

    //  float r = sqrt(x*x+y*y);
    float r = (x*x+y*y);
    float a = atan2(y, x);
    float t = time + 100.0/(r+1.0);
    float s = sin(r+t);

    // float light = 3825.0*fabs(0.05*(sin(t)+sin(time+a*8.0)));
    int light = 3200;
    float u = -sin(r*2.5-a-time+s);
    float v = sin(r*1.5+a+a-time+s);
    float w = cos(r+a*3+time)-s;
    // float l = sqrt(u*u + v*v + w*w);

    u *= light;
    v *= light;
    w *= light;

    struct color color2 =
    {
      ((int)u & 255) * BRIGHTNESS / 255, ((int)v & 255) * BRIGHTNESS / 255, ((int)w & 255) * BRIGHTNESS / 255 
    };

    pixels[i] = color2;
  }
}

inline void draw2(float frameCount)
{
  float s = 0.01 * (0.7 + 0.2 * sin(frameCount * 0.000827));
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
    Color color2 =
    {
      max(0,red * BRIGHTNESS), max(0,green * BRIGHTNESS), max(0,blue * BRIGHTNESS)
      };

      pixels[i] = color2;
  }
}

inline void draw3(float frameCount)
{
  float s = 0.003 * (0.7 + 0.2 * sin(frameCount * 0.000827));
  float r = 2.0 * M_PI * sin(frameCount * 0.000742);

  float time = frameCount * 0.002;

  float sinr = sin(r);
  float cosr = cos(r);

  CGPoint center1 = CGPointMake(cos(time*0.1), cos(time*0.535));
  // CGPoint center2 = CGPointMake(cos(time*0.259), cos(time*0.605));

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
    CGPoint color = CGPointMake(cos(d+time),sin(d));

    CGPoint ncolor = normalize(color);

    float red = ncolor.x*ncolor.x;
    float green = ncolor.x*ncolor.y;
    float blue = ncolor.x-ncolor.y;

    Color color2 = 
    {
      max(0,red * BRIGHTNESS), max(0,green * BRIGHTNESS), max(0,blue * BRIGHTNESS)
      };

      pixels[i] = color2;
  }
}

inline void drawWalker(float frameCount)
{
  float time = frameCount * 0.03;

  for (byte i = 0; i < NUM; i++)
  {
    float c = i*0.1+30*cos(time*0.1)+time*0.11;
    Color color =
    {
      (max(0,BRIGHTNESS*sin(c))), (max(0,BRIGHTNESS*sin((c)*0.81))), (max(0,BRIGHTNESS*sin(c*0.69)))
      };

      pixels[i] = color;
  }
}

void show(byte *bytes, int size)
{/*
  for (int index = 0; index < size; index++)
 {
 // nur 12.5% Helligkeit    
 byte c = bytes[index] >> 3;
 
 //  for (SPDR = c; !(SPSR & _BV(SPIF)););
 SPI.transfer(c);
 }
 */
  // noInterrupts();
  SPI.transferBuffer(bytes, NULL, size);
  // interrupts();

  delay(1);
}

void setup()
{
  SPI.begin();
}

void loop()
{
  long framecount = 0;

  while (1)
  {
    for (int i = 0; i < 20000; i++)
    {
      drawWalker(0.1 * framecount++);
      show((byte *)pixels, sizeof(pixels));
    }

    for (int i = 0; i < 20000; i++)
    {
      draw3(0.1 * framecount++);
      show((byte *)pixels, sizeof(pixels));
    }

    for (int i = 0; i < 10000; i++)
    {
      draw2(0.1 * framecount++);
      show((byte *)pixels, sizeof(pixels));
    }

    for (int i = 0; i < 10000; i++)
    {
      draw(0.1 * framecount++);
      show((byte *)pixels, sizeof(pixels));
    }
  }
}

