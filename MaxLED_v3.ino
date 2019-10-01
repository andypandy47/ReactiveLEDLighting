#include <Adafruit_NeoPixel.h>

#define PIN6 6
#define PIN7 7
#define PIN8 8
#define PIN9 9
#define stripCount 4
#define amountOfColors 6
#define amountOfPatterns 3
#define amountOfLightGroups 12
#define debug 1

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

enum patternType {TYPE1,TYPE2,TYPE3,TYPE4,TYPE5};
struct RGBcolor
{
  public: String colorName;
  public: uint8_t redValue;
  public: uint8_t greenValue;
  public: uint8_t blueValue;
};
RGBcolor colors[] = {
                     {"DarkCyan",0,139,139},{"BlueViolet",138,43,226},{"Crimson",220,20,60},
                     {"DeepPink",255,20,147},{"Gold",255,215,0},{"LimeGreen",50,205,50}
                    };

struct LEDStrip
{
  public: Adafruit_NeoPixel strip;
  public: uint8_t ampValue;
  public: uint8_t oldAmpValue;
  public: patternType pattern;
  public: RGBcolor currentColor;
  public: bool switching;
  public: uint8_t lightGroupIndex;
  public: uint8_t pat3MaxBrightness;
  public: uint8_t pat3CurrentBrightness;
  public: bool pat4LightOn;
  public: bool pat5LightOn;
};
LEDStrip strips[] = {
                     {Adafruit_NeoPixel(60,PIN6,NEO_GRB + NEO_KHZ800),0,0,TYPE1,colors[0],false,random(0, amountOfLightGroups-1),255,0,false,false},
                     {Adafruit_NeoPixel(60,PIN7,NEO_GRB + NEO_KHZ800),0,0,TYPE1,colors[1],false,random(0, amountOfLightGroups-1),255,0,false,false},
                     {Adafruit_NeoPixel(60,PIN8,NEO_GRB + NEO_KHZ800),0,0,TYPE1,colors[2],false,random(0, amountOfLightGroups-1),255,0,false,false},
                     {Adafruit_NeoPixel(60,PIN9,NEO_GRB + NEO_KHZ800),0,0,TYPE1,colors[3],false,random(0, amountOfLightGroups-1),255,0,false,false}
                    };

struct LEDGrouping
{
  public: uint8_t minValue;
  public: uint8_t maxValue;
};
LEDGrouping lightGroups[] = {
                              {0,4},{5,9},{10,14},{15,19},{20,24},{25,29},{30,34},
                              {35,39},{40,44},{45,49},{50,54},{55,59}
                            };
char temp[] = {0,0,0,0};
int d=0;
bool canInterrupt = true;

void setup()
{
  Serial.begin(9600);

 for(int i=0;i<stripCount;i++)
 {
   strips[i].strip.begin();
   strips[i].strip.show();
   strips[i].currentColor = colors[0];
 }
}

void loop()
{
  while (Serial.available())
  { 
      temp[d] = Serial.read();
      d++;

      if (d == 2)
      {
        int identifier = temp[0];//read a byte of serial data
        int val = temp[1];         

        if (identifier!=-1)
        {
          if (debug==1)
          {
            Serial.print(identifier);
            Serial.print(" ");
            Serial.println(val);
          }

          switch (identifier)
          {
            case 0:
              strips[0].ampValue=val;
              break;
            case 1:
              strips[1].ampValue=val;
              break;
            case 2:
              strips[2].ampValue=val;
              break;
            case 3:
              strips[3].ampValue=val;
              break;
            case 4:
              ChangeStrip(strips[0], val);
              break;
            case 5:
              ChangeStrip(strips[1], val);
              break;
            case 6:
              ChangeStrip(strips[2], val);
              break;
            case 7:
              ChangeStrip(strips[3], val);
              break;
            case 8:
              switch(val)
              {
                case 0:
                  for(int i=0;i<stripCount;i++)
                  {
                    if (strips[i].pattern==2)
                    {
                      strips[i].pat3CurrentBrightness=strips[i].pat3MaxBrightness;
                      Pattern3(strips[i]); 
                    }
                  }
                  break;
                case 1:
                  for(int i=0;i<stripCount;i++)
                  {
                    if (strips[i].pattern==3)
                    {
                      strips[i].pat4LightOn = true;
                      Pattern4(strips[i]);
                    }
                  }
                  break;
                case 2:
                  for(int i=0;i<stripCount;i++)
                  {
                    if (strips[i].pattern==4)
                    {
                      strips[i].pat5LightOn = true;
                      Pattern5(strips[i]);
                    }
                  }
                  break;
                
              }
          }
        }
        d = 0;
      }

    for(int i=0; i<stripCount;i++)
    {     
      if (strips[i].pattern == 0)
      {
        Pattern1(strips[i]);
      }
      else if (strips[i].pattern == 1)
      {
        Pattern2(strips[i]);
      }
      
      strips[i].strip.show();
    }   
  }
}

void ChangeStrip(LEDStrip& stripToEffect, int value)
{
  switch (value)
  {
    case 43:
      stripToEffect.currentColor = colors[0];
      break;
    case 44:
      stripToEffect.currentColor = colors[1];
      break;
    case 45:
      stripToEffect.currentColor = colors[2];
      break;
    case 46:
      stripToEffect.currentColor = colors[3];
      break;
    case 47:
      stripToEffect.currentColor = colors[4];
      break;
    case 48:
      stripToEffect.currentColor = colors[5];
      break; 
    case 52:
      stripToEffect.pattern = TYPE1;
      break;
    case 53:
      stripToEffect.pattern = TYPE2;
      break;
    case 54:
      stripToEffect.pattern = TYPE3;
      break;
    case 55:
      stripToEffect.pattern = TYPE4;
      break;
    case 56:
      stripToEffect.pattern = TYPE5;
      break;  
  }

  for(int i=0;i<stripToEffect.strip.numPixels();i++)
  {
    stripToEffect.strip.setPixelColor(i, stripToEffect.strip.Color(0,0,0));
  }
}

void Pattern1(LEDStrip& stripToEffect)
{
  if (stripToEffect.oldAmpValue > stripToEffect.ampValue)
  {
    for (uint16_t j=stripToEffect.oldAmpValue;j>stripToEffect.ampValue;j--)
    {
      stripToEffect.strip.setPixelColor(j, stripToEffect.strip.Color(0,0,0));
    }
  }
  else
  {
    for (uint16_t j=0; j<stripToEffect.ampValue;j++)
    {
      stripToEffect.strip.setPixelColor(j, stripToEffect.strip.Color
      (stripToEffect.currentColor.redValue,stripToEffect.currentColor.blueValue,stripToEffect.currentColor.greenValue));
    }
  } 
  stripToEffect.oldAmpValue = stripToEffect.ampValue;
}

void Pattern2(LEDStrip& stripToEffect)
{
  if (stripToEffect.ampValue > stripToEffect.oldAmpValue)
  {
    for(uint16_t j=29;j<29+stripToEffect.ampValue;j++)
    {
      stripToEffect.strip.setPixelColor(j, stripToEffect.strip.Color
      (stripToEffect.currentColor.redValue,stripToEffect.currentColor.blueValue,stripToEffect.currentColor.greenValue));
    }

    for(uint16_t j=29;j>29-stripToEffect.ampValue;j--)
    {
      stripToEffect.strip.setPixelColor(j, stripToEffect.strip.Color
      (stripToEffect.currentColor.redValue,stripToEffect.currentColor.blueValue,stripToEffect.currentColor.greenValue));
    }
  }
  else
  {
    for (uint16_t j=29+stripToEffect.oldAmpValue;j>29+stripToEffect.ampValue;j--)
    {
      stripToEffect.strip.setPixelColor(j,stripToEffect.strip.Color(0,0,0));
    }

    for (uint16_t j=29-stripToEffect.oldAmpValue;j<29-stripToEffect.ampValue;j++)
    {
      stripToEffect.strip.setPixelColor(j,stripToEffect.strip.Color(0,0,0));
    }
  }
  stripToEffect.oldAmpValue = stripToEffect.ampValue;
}

void Pattern3(LEDStrip& stripToEffect)
{
  while (stripToEffect.pat3CurrentBrightness>0)
    {
      for(uint16_t i=lightGroups[stripToEffect.lightGroupIndex].minValue;
          i<lightGroups[stripToEffect.lightGroupIndex].maxValue;i++)
      {
        stripToEffect.strip.setPixelColor(i,stripToEffect.strip.Color(stripToEffect.pat3CurrentBrightness,0,0));
      }
      stripToEffect.pat3CurrentBrightness-=5;
      stripToEffect.strip.show();
    }
    for(uint16_t i=lightGroups[stripToEffect.lightGroupIndex].minValue;
          i<lightGroups[stripToEffect.lightGroupIndex].maxValue;i++)
      {
        stripToEffect.strip.setPixelColor(i,stripToEffect.strip.Color(0,0,0));
      }
    stripToEffect.lightGroupIndex = random(0, amountOfLightGroups-1);
}

void Pattern4(LEDStrip& stripToEffect) 
{      
  int firstLight = 1;
  int lastLight;
  
  while(stripToEffect.pat4LightOn)
  {
    lastLight = firstLight-5;
    if (lastLight >= 60)
    {
      stripToEffect.pat4LightOn = false;
    }
    
    for(int i= lastLight > 0 ? lastLight:0;i<firstLight;i++)
    {
      stripToEffect.strip.setPixelColor(i, stripToEffect.strip.Color
      (stripToEffect.currentColor.redValue,stripToEffect.currentColor.blueValue,stripToEffect.currentColor.greenValue));
    }

    if (lastLight > 0)
    {
      stripToEffect.strip.setPixelColor(lastLight-1,stripToEffect.strip.Color(0,0,0));
    }
    stripToEffect.strip.show();

    firstLight++;
    Serial.println(lastLight);
    delay(5);
  }
}

void Pattern5(LEDStrip& stripToEffect)
{
  int firstLight = 59;
  int lastLight;
  while(stripToEffect.pat5LightOn)
  {
    lastLight = firstLight+10;
    if (lastLight < 0)
    {
      stripToEffect.pat5LightOn = false;
    }
    
    for(int i= lastLight < 60 ? lastLight:60;i>firstLight;i--)
    {
      stripToEffect.strip.setPixelColor(i, stripToEffect.strip.Color
      (stripToEffect.currentColor.redValue,stripToEffect.currentColor.blueValue,stripToEffect.currentColor.greenValue));
    }
    
    if (lastLight < 60)
    {
      stripToEffect.strip.setPixelColor(lastLight+1,stripToEffect.strip.Color(0,0,0));
    }
    stripToEffect.strip.show();

    firstLight--;
    
    delay(5);
  }
}
