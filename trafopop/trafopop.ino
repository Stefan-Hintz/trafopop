// Der Inhalt der Datei SPI.h wird beim Compilieren dieser Datei zu dieser Datei textuell hinzugefügt. Dadurch stehen die darin enthaltenen Definitionen auch hier zur Verfügung.
// http://arduino.cc/en/Reference/Include
// Konkret sind dort Dinge enthalten, die es ermöglichen die SPI-Schnittstelle des Arduinos nutzen zu können.
// http://arduino.cc/en/Reference/SPI
#include <SPI.h>

// Die Anzahl der Pixel dieser Jacke beträgt 102
// Überall wo im weiteren in dieser Datei NUM verwendetet wird, wird NUM beim Compilieren durch 102 ersetzt.
// Dadurch braucht man bei Änderung der Pixel-Anzahl nur eine Zeile ändern und nicht alle Stellen an denen dieser Wert verwendet wird.
// http://arduino.cc/en/Reference/Define
#define NUM 102

// Hier wird ein struktierter Typ mit dem Namen "color" deklariert.
// Konkret besteht dieser Typ aus drei Komponenten, die vom primitiven byte sind.
// Gedacht ist dieser Typ als ein Farbwert bestehend aus drei Kompanenten für Rot, Grün und Blau, die jeweils einen Heligkeitsbreich von minmal 0 und maximal 255 besitzen.
// http://arduino.cc/de/Reference/Byte
typedef struct color 
{
  byte r, g, b;
}
color;

// Hier wird eine Array-Variable definert die Anzahl NUM Elemente von dem soeben selbstdefinierten Typ color enthält.
// http://arduino.cc/en/Reference/VariableDeclaration
// http://arduino.cc/en/Reference/Array
color pixels[NUM];

/*
    Layout:
 zuerst 2x7 LEDs auf der Jackenfront, rechts (1) von oben (6) nach unten (-6), dann links (-1) von unten (-6) nach oben (6).
 
 Dann (später) die fünf Ringe siehe: setupPoints 
 */
// http://arduino.cc/en/Reference/Float
float pointX[NUM] =               // "float" = Typ einer Variable (hier: Fließkommazahlen...kann also Kommazahlen speichern) ... "pointx" = individueller Name für die Variable "float" ... "[NUM]" = Anzahl der Array-Elemente
{
  1, 1, 1, 1, 1, 1, 1,
  -1, -1, -1, -1, -1, -1, -1
};                               // im Koordinatensystem auf der X-Achse (-1 bis 1)

float pointY[NUM] =              // "pointy" = individueller Name für die Variable "float"
{
  6, 4, 2, 0, -2, -4, -6,
  -6, -4, -2, 0, 2, 4, 6
};                               // im Koordinatensystem auf der Y-Achse (-6 bis 6)

// bestimmt allein die Anzahl an Zahlen in einer Reihe, um wieviele Dioden es sich handelt?
// Nein. Die Anzahl wird hier durch NUM festgelegt. Die restlichen Werte werden weiter unter in der Funktion setupPoints() initialisiert.

// inline sorgt dafür, das intern keine Funktion aufgerufen wird, sondern der Code der Funkton überall dort hinein kopiert wird, wo sie aufgerufen.
// inline sorgt damit für schnelleren aber größeren Code, wenn es viele Aufrufe gibt. Bei sehr kurzen Funktionen oder sehr wenigen Aufrufern gibt es diesen Nachteil nicht. 
inline float normalizedX(byte index)  /* "inline" = inline-Funktion (C99-Standard) ... "Eine Funktion, die als inline definiert ist, soll gemäß dem C-Standard so schnell wie möglich aufgerufen werden. (wird aber von den meisten Compilern ignoriert)"
 siehe: http://de.wikibooks.org/wiki/C-Programmierung:_Funktionen#Inline-Funktionen */
// "float" deklariert hier die Variable "normalizedX" mit der Bedingung "byte index" ... "byte" ist ein Datentyp, der Zahlen von 0-255 speichern kann ... hier deklariert "byte" die Variable "index"

{
  // Der Wert an der Array-Position index wird von dieser Funktion zurück geliefert.
  return pointX[index];
}                                    /* "Die return-Anweisung beendet die Abarbeitung der aktuellen Funktion. Wenn eine return-Anweisung mit einem Ausdruck angegeben wird, 
 dann wird der Wert des Ausdrucks an die aufrufende Funktion als Rückgabewert geliefert. In einer Funktion können beliebig viele return-Anweisungen 
 angegeben werden. Jedoch muss dabei darauf geachtet werden, dass nachfolgender Programmcode durch den Programmfluss noch erreichbar bleibt.
 siehe (unten): http://de.wikibooks.org/wiki/C-Programmierung:_Sprachbeschreibung:_Anweisungen */

inline float normalizedY(byte index)
{
  return pointY[index];
}

// Hier wird ein struktierter Typ mit dem Namen "CGPoint" deklariert.
// Konkret besteht dieser Typ aus zwei Komponenten, die vom primitiven float sind.
// Gedacht ist dieser Typ für einen Punkt im 2D-Raum mit einer X- und einer Y-Koordinate.
typedef struct CGPoint         /* hier gebe ich dem Datentyp "struct" den Aliasnamen "CGPoint" ... CGPoint represents a point in a two-dimensional coordinate system 
 siehe: https://developer.apple.com/library/ios/documentation/graphicsimaging/reference/CGGeometry/Reference/reference.html */
{                              // innerhalb von "CGPoint" soll "float x" und "float y" gespeichert werden ... der Datentyp "float" deklariert hier x und y
  float x;
  float y;
}
CGPoint;

inline struct CGPoint CGPointMake(float x, float y)      /* mit "struct" lege ich hier den persönlichen Datentyp "CGPoint" an ... aber wozu dient "CGPointMake(float x, float y) ???
 "Eine Struktur (struct(ure)) ist ein Datentyp, der mehrere Variablen gleichen oder verschiedenen Typs zu einem neuen Datentyp zusammenfasst. Die Deklaration erfolgt mit dem Schlüsselwort struct."
 siehe: http://de.wikibooks.org/wiki/C%2B%2B-Programmierung:_Strukturen_und_Unions */

{                                                        // in { } wird dann angegeben, was im Datentyp "CGPoint" gespeichert werden soll
  struct CGPoint point;                                  // hier wird nun was gemacht ???

  point.x = x;                                           // Mit dem Operator . (Punkt) kann auf die einzelnen Daten zugegriffen werden.
  point.y = y;                                           // "x" und "y" wird jeweils ein Wert gegeben

  return point;
}

// Diee ist eine Funktion, die Ursprünglich den Anstand zweier Punkte in der Ebene nach Pythagora berechnete.
// http://de.wikipedia.org/wiki/Satz_des_Pythagoras
// Aus Geschwindigkeitsgründen wurde die Wurzelfunktion später weggelassen, wodurch eine etwas andere Animation entsteht.
inline float distance(CGPoint a, CGPoint b)              /* "float" deklariert die Variable "distance" mit der Bedingung "(CGPoint a, CGPoint b)" ... wie wirkt sich diese Bedingung aus ???
 Sollen hier die Variablen "CGPoint a" und "CGPoint b" auf etwas geprüft werden? Wenn ja, auf was ??? */
{
  float x = a.x - b.x;                                  // "float" deklariert "x" und gibt diesem ein Wert, der sich aus "a.x - b.x" berechnet ... "a" und "b" greifen auf den Wert "x" zu
  float y = a.y - b.y;                                  // "float" deklariert "y" und gibt diesem ein Wert, der sich aus "a.y - b.y" berechnet ... "a" und "b" greifen auf den Wert "y" zu

  //  return sqrt(x*x + y*y);
  return (x*x + y*y);                                    // "return" beendet die Funktion "inline float distance(CGPoint a, CGPoint b)" und gibt einen Wert zurück, der sich aus "x*x + y*y" berechnet ... An was gibt "return" den Wert zurück ??? Und welchen Einfluss hat dieser Wert?
}

// Dies ist eine Funktion, die einen 2D-Vektor als Eingabe bekommt und einen neuen Vektor berechnet.
// Der neue Vektor hat die gleiche Richtung, aber die Länge wurde auf 1 geändert.
inline CGPoint normalize(CGPoint p)                    // Wird hier etwas deklariert? (nichts ist orange in dieser Zeile) ... Was passiert hier ???
{
  float length = sqrt(p.x*p.x + p.y*p.y);              // der Datentyp "float" deklariert die Variable "length" mit dem Wert "sqrt(p.x*p.x + p.y*p.y)" ... sqrt = "Wurzel aus" ...
  // Wenn "p" auf den Wert x und y zugreift, wieso schreibt man dann nicht gleich einfach nur "(x*x + y*y) ??? p greif hier nicht auf einen Wet zu. x und y sind hier nicht definiert. p ist eine Variable. p.x ist die X-Komponente von p.

  return CGPointMake(p.x/length, p.y/length);          // "return" beendet diese Funktion und gibt einen Wert zurück ... an was ??? Rückgabewerte werden an den Aufrufer zurück gegegeben.
}

// Beginn der LED-Animation "draw"

// void bedeutet, dass diese Funktion keinen Rückgabewert hat. Stattdessen wird die globale Variable pixels verändert.
inline void draw(float frameCount)                                  // in "void" ist nichts enthalten ... "draw" nimmt also auch keinen Wert an, hat aber die Bedingung "float framecount" ... Warum ???
{
  // Der Variablen vom Typ float mit dem Namen s wird ein Wert zugewiesen 
  float s = 0.01 * (0.7 + 0.2 * sin(frameCount * 0.000827));
  // M_PI ist eine vordefinierter Wert und enthält das mathematisch Pi (3.141569...)
  float r = 2.0 * M_PI * sin(frameCount * 0.000742);      

  float time = frameCount * 0.02;   

  float sinr = sin(r);        
  float cosr = cos(r); 


  CGPoint center1 = CGPointMake(cos(time), cos(time*0.535));              // ???
  CGPoint center2 = CGPointMake(cos(time*0.259), cos(time*0.605));        // ???
  // CGPoint center3 = CGPointMake(cos(time*0.346), cos(time*0.263));
  // CGPoint center4 = CGPointMake(cos(time*0.1346), cos(time*0.1263));
  // float size = (sin(time*0.1)+1.2)*64.0;

  for (byte i = 0; i < NUM; i++)            // "for" ist eine Schleife ... "byte" deklariert die Variable "i", welches mit "0" initialisert wird ... "i < NUM" = Bedingung, damit etwas ausgeführt wird ... "i++" (auch: i + 1) = was passieren soll, wenn Bedingung erfüllt wird
  {
    float x0 = s * normalizedX(i);          // "float" deklariert "x0" mit dem Wert "s * normalizedX(i)" ???  (Wäre schön, das mal alles graphisch zu sehen...kann mir nichts darunter vorstellen)
    float y0 = s * normalizedY(i);          // "float" deklariert "y0" mit dem Wert "s * normalizedY(i)" ???
    float x = (x0*cosr - y0*sinr);          // "float" deklariert "x" mit dem Wert "(x0*cosr - y0*sinr)" ???
    float y = (x0*sinr + y0*cosr);          // "float" deklariert "y" mit dem Wert "(x0*sinr - y0*cosr)" ???

    // x++;
    y++;                                    // ???

    //  float r = sqrt(x*x+y*y);
    float r = (x*x+y*y);                    // "float" deklariert "r" mit dem Wert "(x*x+y*y)"
    float a = atan2(y, x);                  // "atan2() berechnet den Arcus Tangens des übergebenen Parameters." siehe: http://www.proggen.org/doku.php?id=c:lib:math:atan2 ... "y" und "x" = Punkte im Koordinatensystem
    float t = time + 100.0/(r+1.0);         // "float" deklariert "t" mit dem Wert "time + 100.0/(r+1.0)"
    float s = sin(r+t);                     // "float" deklariert "s" mit dem Wert "sin(r+t)"

    // float light = 3825.0*fabs(0.05*(sin(t)+sin(time+a*8.0)));
    byte light = 3200;                                              // "byte" deklariert "light" mit dem Wert 3200
    float u = -sin(r*2.5-a-time+s);                                 
    float v = sin(r*1.5+a+a-time+s);
    float w = cos(r+a*3+time)-s;
    // float l = sqrt(u*u + v*v + w*w);

    u *= light;                            // "*=" ???
    v *= light;
    w *= light;

    struct color color2 =
    {
      u,v,w 
    };

    pixels[i] = color2;
  }
}



// Beginn der LED-Animation "draw2"

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


// Beginn der LED-Animation "draw3"

inline void draw3(float frameCount)
{
  float s = 0.003 * (0.7 + 0.2 * sin(frameCount * 0.000827));
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
    float x = /*s **/ normalizedX(i);
    float y = /*s **/ normalizedY(i);
    // float x = (x0*cosr - y0*sinr);
    // float y = (x0*sinr + y0*cosr);

    /*
    CGPoint position =
     {
     x,y
     };
     */
    float radius = (sqrt(x*x+y*y) /*Satz des Phytagoras zur Bestimmung der LEDs im Koordinatensystem*/ + (sin(time) + 1) * 4.5)/9.0; // alle LEDs, die den gleichen Radius haben, haben dieselbe Farbe ... Wie das ???

    float red = radius;
    float green = radius * radius; //  * (i%3) hinter radius multiplizieren, um, wie in diesem Beispiel, jede 3. LED ein und dieselbe Farbe zu geben
    float blue = 1 - radius;
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

// bytes ist der Name des Parameters von Typ byte *
// Es wird hier kein byte übergeben, sondern die Adresse eines Speicherbereiches von Typ byte.
// Darauf kann man dann wie auf einen Array zugreifen.
void show2(byte *bytes, int size)              // Was bewirkt "*bytes" ???
{
  for (int index=0; index<size; index++)      // "for" = Schleife .. "int" deklariert die Variable "index", welches mit "0" initialisert wird ... "index < size" = Bedingung, damit etwas ausgeführt wird ... "index++" (auch: index + 1) wird nach jedem Schleifendurchlauf aufgerufen
  {
    // nur 25% Helligkeit 
    // 6,25% bei >> 4 
    byte c = bytes[index] >> 2;              // "[index]" = Array ???

    // Hier wird in einer Schleife nacheinander die 8 Bit eines Bytes über die SPI-Schnittstelle ausgegeben.
    for (SPDR = c; !(SPSR & _BV(SPIF)););   // ???
  }
}

// Hier werden nur die Koordinaten der LEDs in den Ringen berechnet
void setupPoints()
{
#define RING_COUNT 5              // "#define" definiert ein Symbol (hier das Symbol "RING_COUNT" mit dem Wert 5) ??? ... gibt an, um wieviele Ringe es sich handelt

  byte counts[RING_COUNT] =
  {   
    30, 24, 18, 12, 4            // number of pixels per ring = Array ???
  };

  byte offsets[RING_COUNT] =
  {   
    5, 4, 3, 3, 1               // number of pixels before the top direction in each ring
    // Abstand der "Sprung"-LED (wo von einem äußeren Ring in einen inneren Ring gewechselt wird)
    // zu der jeweils oberen LED in einem Ring
  };

  byte index = 14;              // Die ersten 14 Pixel wurden bereits oben definiert ... Wieso müssen sie hier nochmal erwähnt werden ???

  for (byte ring = 0; ring < RING_COUNT; ring++)      // "for" = Schleife
  {
    // 9, 7, 5, 3, 1
    byte radius = 9 - 2 * ring;        // von außen (9) nach innen (1) ... "9"/"1"/"2" was ???
    byte count = counts[ring];         // für "counts" siehe "#define RING_COUNT 5" ... "[ring]" = Array ???

    for (byte ringIndex = 0; ringIndex < count; ringIndex++, index++)      // ???
    {
      byte ringOffset = offsets[ring];                              // jeder Ring beginnt oben minus Offset und geht dann im Uhrzeigersinn weiter   ???
      float angle = 2 * M_PI * (ringIndex - ringOffset) / count;

      // sin und cos sind Kreisfunktionen
      // http://www.businessinsider.com/7-gifs-trigonometry-sine-cosine-2013-5
      pointX[index] = sin(angle) * radius;         // ???
      pointY[index] = cos(angle) * radius;         // ???
    }
  }
}

void setup()
{
  setupPoints();

  //SPI-Bibliothek zur Kommunikation als SPI-Master

  SPI.begin();                                         // startet SPI-Bibliothek
  SPI.setBitOrder(MSBFIRST);                           // setzt die Bit-Reihenfolge
  SPI.setDataMode(SPI_MODE0);                          // setzt den SPI-Modus
  // SPI.setClockDivider(SPI_CLOCK_DIV16);  // 1 MHz
  // Legt die Übertragungsgeschwindigkeit der Daten zu den LEDs fest
  SPI.setClockDivider(SPI_CLOCK_DIV4);  // 2 MHz       // stellt die Geschwindigkeit ein (von was???)
  // SPI.setClockDivider(SPI_CLOCK_DIV4);  // 4 MHz 
}
  
// Ist eine globale Zählervariable, die nach jedem berechneten Animation Frame erhöht wird, damit das nächste Frame anders aussieht.
// Dies ist sozusagen die Zeit-Dimension
long framecount = 0;  

void loop()
{
// Endlosschleife
// solange die Bedingung (1) wahr ist, wird die Schleife immer wieder ausgführt.
// 1 = wahr
// 0 = falsch
  while (1)                                        // Schleife .. Was bedeutet die "(1)" ???
  {
    // Aufruf der Funktion draw3 mit Parameter framecount, anschließend framecount um 1 erhöhen
    draw3(framecount++);
    
    // eigenlich ist pixels ein Array von Farben
    // hier wird er aber als Array von Bytes behandelt
    // sizeof liefert die Anzahl der Bytes, die die Variable enthält
    show2((byte *)pixels, sizeof(pixels)); 

  // 2 MIllisekunden warten
  // http://arduino.cc/en/Reference/delay
    delay(2); 
  }
}

