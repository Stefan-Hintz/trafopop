#include <Adafruit_NeoPixel.h>

#define PIN 11
#define NUM 256

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM, PIN, NEO_GRB + NEO_KHZ800);

inline float normalizedX(byte index)
{
  return (index >> 4) - 7.5;
}

inline float normalizedY(byte index)
{
  char y = index & 0xf;

  return (index & 0x10 ? y : 0xf - y) - 7.5;
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

  for (int i = 0; i < NUM; i++)
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
 // CGPoint center2 = CGPointMake(cos(time*0.259), cos(time*0.605));
  // CGPoint center3 = CGPointMake(cos(time*0.346), cos(time*0.263));
  // CGPoint center4 = CGPointMake(cos(time*0.1346), cos(time*0.1263));
  // float size = (sin(time*0.1)+1.2)*64.0;

  for (int i = 0; i < NUM; i++)
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
  //  d = distance(position, center2)*size;
  //  color.x += cos(d);
   // color.y += sin(d);
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
  float s = 0.1 * (0.7 + 0.2 * sin(frameCount * 0.000827));
  // float r = 2.0 * M_PI * sin(frameCount * 0.000742);

  float time = frameCount * 0.1;

  CGPoint center1 = CGPointMake(2*sin(time), 2*sin(time*0.535));
 
  for (int i = 0; i < NUM; i++)
  {
    float x = s * normalizedX(i);
    float y = s * normalizedY(i);
 
    CGPoint position =
    {
      x, y
    };

    float d = distance(position, center1);
 
    float red = cos(d+time);
    float green = sin(d);
    float blue = sin(d-time)*0.81;
    
     strip.setPixelColor(i, max(0, red * 255), max(0, green * 255), max(0, blue * 255));
  }
}

void setup()
{
  strip.begin();
  strip.setBrightness(32);
  strip.show(); // Initialize all pixels to 'off'

  long framecount = 0;

  while (1)
  {
    draw3(framecount++);
    strip.show();
  }
}

void loop()
{
}

