#include <Adafruit_NeoPixel.h>

#define NUM 124

#define PIN 16

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM, PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
  strip.begin();
  strip.setBrightness(32);
  strip.show(); // Initialize all pixels to 'off'
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    strip.setPixelColor(i, c);
    strip.show();

    delay(wait);
  }
}

void rainbow(uint8_t wait) 
{
  uint16_t i, j;

  for(j=0; j<256; j++) 
  {
    for(i=0; i<strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }

    strip.show();

    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) 
{
  uint16_t i, j;

  for(j=0; j<256*5; j++)
  { 
    // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }

    strip.show();

    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85)
  {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }

  if (WheelPos < 170)
  {
    WheelPos -= 85;

    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }

  WheelPos -= 170;

  return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
}

inline char normalizedX(byte index)
{
  byte x = index % 25;
  byte y = index / 25;

  return y % 2 == 1 ? x : 26 - x;
}

typedef struct CGPoint
{
  float x;
  float y;
}
CGPoint;

inline struct CGPoint normalizedPoint(byte index)
{
  CGPoint point;

  if (index < 24)
  {
    point.x = -2;
    point.y = index;
  }
  else if (index < 49)
  {
    point.x = -1;
    point.y = 48 - index;
  }
  else if (index < 75)
  {
    point.x = 0;
    point.y = index - 49;
  }
  else if (index < 100)
  {
    point.x = 1;
    point.y = 99 - index;
  }
  else
  {
    point.x = 2;
    point.y = index - 100;
  }

  return point;
}

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

  CGPoint center1 = CGPointMake(cos(time), cos(time*0.535));
  CGPoint center2 = CGPointMake(cos(time*0.259), cos(time*0.605));
  // CGPoint center3 = CGPointMake(cos(time*0.346), cos(time*0.263));
  // CGPoint center4 = CGPointMake(cos(time*0.1346), cos(time*0.1263));
  // float size = (sin(time*0.1)+1.2)*64.0;

  for (byte i = 0; i < NUM; i++)
  {
    CGPoint point = normalizedPoint(i);
    float x0 = s * point.x;
    float y0 = s * point.y;
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

    strip.setPixelColor(i, u, v, w);
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
    CGPoint point = normalizedPoint(i);
    float x0 = s * point.x;
    float y0 = s * point.y;
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
    strip.setPixelColor(i, max(0,red * 255), max(0,green * 255), max(0,blue * 255));
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
  CGPoint center2 = CGPointMake(cos(time*0.259), cos(time*0.605));

  for (byte i = 0; i < NUM; i++)
  {
    CGPoint point = normalizedPoint(i);
    float x0 = s * point.x;
    float y0 = s * point.y;
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

    strip.setPixelColor(i, max(0,red * 255), max(0,green * 255), max(0,blue * 255));
  }
}

inline void drawWalker(long frameCount)
{
  float time = frameCount * 0.03;

  for (byte i = 0; i < NUM; i++)
  {
    float c = i*0.1+30*cos(time*0.1)+time*0.11;

    strip.setPixelColor(i, (max(0,255*sin(c))), (max(0,255*sin((c)*0.81))), (max(0,255*sin(c*0.69))));
  }
}

void loop()
{
  long framecount = 0;

  // Some example procedures showing how to display to the pixels:
  //  colorWipe(strip.Color(255, 0, 0), 50); // Red
  //  colorWipe(strip.Color(0, 255, 0), 50); // Green
  //  colorWipe(strip.Color(0, 0, 255), 50); // Blue

  while (1)
  {
    for (int i = 0; i < 2000; i++)
    {
      drawWalker(framecount++);
      strip.show();
    }

    for (int i = 0; i < 2000; i++)
    {
      draw3(framecount++);
      strip.show();
    }

    for (int i = 0; i < 1000; i++)
    {
      draw2(framecount++);
      strip.show();
    }

    for (int i = 0; i < 10; i++)
    {
      rainbow(20);
    }

    for (int i = 0; i < 1000; i++)
    {
      draw(framecount++);
      strip.show();
    }

    rainbowCycle(20);
  }
}

