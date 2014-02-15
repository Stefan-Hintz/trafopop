#include <SPI.h>

#define NUM sizeof(positions)/sizeof(Point)

typedef struct Point
{
  char x;
  char y;
}
Point;

Point positions[100] =
{
5,-1,
5,0,
5,1,
5,2,
5,3,
5,4,
5,5,
5,6,
5,7,
4,6,
4,5,
4,4,
4,3,
4,2,
4,1,
4,0,
4,-1,
4,-2,
4,-3,
3,-3,
3,-2,
3,-1,
3,0,
3,1,
3,2,
3,3,
3,4,
3,5,
2,4,
2,3,
2,2,
2,1,
2,0,
2,-1,
2,-2,
2,-3,
1,-4,
1,-3,
1,-2,
1,-1,
1,0,
1,1,
1,2,
1,3,
1,4,
0,4,
0,3,
0,2,
0,1,
0,0,
0,-1,
0,-2,
0,-3,
0,-4,
0,-5,
-1,-4,
-1,-3,
-1,-2,
-1,-1,
-1,0,
-1,1,
-1,2,
-1,3,
-1,4,
-2,4,
-2,3,
-2,2,
-2,1,
-2,0,
-2,-1,
-2,-2,
-2,-3,
-3,-3,
-3,-2,
-3,-1,
-3,0,
-3,1,
-3,2,
-3,3,
-3,4,
-3,5,
-4,6,
-4,5,
-4,4,
-4,3,
-4,2,
-4,1,
-4,0,
-4,-1,
-4,-2,
-4,-3,
-5,-1,
-5,0,
-5,1,
-5,2,
-5,3,
-5,4,
-5,5,
-5,6,
-5,7,
};

typedef struct color
{
  byte r, g, b;
}
color;

color pixels[NUM];

inline char normalizedX(byte index)
{
  return positions[index].x;
}

inline char normalizedY(byte index)
{
  return positions[index].y;
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

    // x++;
    y++;

    //  float r = sqrt(x*x+y*y);
    float r = (x*x+y*y);
    float a = atan2(y, x);
    float t = time + 100.0/(r+1.0);
    float s = sin(r+t);

    // float light = 3825.0*fabs(0.05*(sin(t)+sin(time+a*8.0)));
    byte light = 3200;
    float u = -sin(r*2.5-a-time+s);
    float v = sin(r*1.5+a+a-time+s);
    float w = cos(r+a*3+time)-s;
    // float l = sqrt(u*u + v*v + w*w);

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
    struct color color2 =
    {
      max(0,red * 255), max(0,green * 255), max(0,blue * 255)
      };

      pixels[i] = color2;
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
}

long framecount = 0;

void loop()
{
  draw2(framecount++);
  show2((byte *)pixels, sizeof(pixels));

  delay(2);
}

