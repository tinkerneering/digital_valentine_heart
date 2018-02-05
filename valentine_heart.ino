/*
 * A Digital Valentines Heart
 * By Rob Clarke of Tinkerneering
 *
 * Based on an article provided by "Yet Another Arduino Blog"
 *
 */

int ANIMDELAY = 100;  // animation delay, default value is 100
int INTENSITYMIN = 0; // minimum brightness, valid range [0,15]
int INTENSITYMAX = 8; // maximum brightness, valid range [0,15]

int DIN_PIN = 2;      // data in pin
int CS_PIN = 3;       // load (CS) pin
int CLK_PIN = 4;      // clock pin

// MAX7219 registers
byte MAXREG_DECODEMODE = 0x09;
byte MAXREG_INTENSITY  = 0x0a;
byte MAXREG_SCANLIMIT  = 0x0b;
byte MAXREG_SHUTDOWN   = 0x0c;
byte MAXREG_DISPTEST   = 0x0f;

const unsigned char off[] =
{
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};

const unsigned char eye[] =
{
  B00000000,
  B00000000,
  B10000010,
  B11111110,
  B11111110,
  B10000010,
  B00000000,
  B00000000
};

const unsigned char heart[] =
{
  B01110000,
  B11111000,
  B11111100,
  B01111110,
  B01111110,
  B11111100,
  B11111000,
  B01110000
};

const unsigned char you[] =
{
  B00000000,
  B11111100,
  B11111110,
  B00000110,
  B00000110,
  B11111110,
  B11111100,
  B00000000
};



void setup ()
{
  pinMode(DIN_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);

  // initialization of the MAX7219
  setRegistry(MAXREG_SCANLIMIT, 0x07);
  setRegistry(MAXREG_DECODEMODE, 0x00);  // using an led matrix (not digits)
  setRegistry(MAXREG_SHUTDOWN, 0x01);    // not in shutdown mode
  setRegistry(MAXREG_DISPTEST, 0x00);    // no display test
  setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMIN);

  populateRegistry(off);
}


void loop ()
{

  // EYE
  populateRegistry(eye);
  setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMIN);
  delay(ANIMDELAY*6);

  // OFF
  populateRegistry(off);
  setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMIN);
  delay(ANIMDELAY*3);

  // first beat
  populateRegistry(heart);
  setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMAX);
  delay(ANIMDELAY);
  
  // switch down
  setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMIN);
  delay(ANIMDELAY);
  
  // second beat
  setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMAX);
  delay(ANIMDELAY);
  
  // switch down
  setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMIN);
  delay(ANIMDELAY*3);

  // OFF
  populateRegistry(off);
  setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMIN);
  delay(ANIMDELAY*3);

  // YOU
  populateRegistry(you);
  setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMIN);
  delay(ANIMDELAY*6);

  // OFF
  populateRegistry(off);
  setRegistry(MAXREG_INTENSITY, 0x0f & INTENSITYMIN);
  delay(ANIMDELAY*6);

}

void populateRegistry(const unsigned char* arr)
{
  // put up registry
  setRegistry(1, arr[0]);
  setRegistry(2, arr[1]);
  setRegistry(3, arr[2]);
  setRegistry(4, arr[3]);
  setRegistry(5, arr[4]);
  setRegistry(6, arr[5]);
  setRegistry(7, arr[6]);
  setRegistry(8, arr[7]);

}

void setRegistry(byte reg, byte value)
{
  digitalWrite(CS_PIN, LOW);

  putByte(reg);   // specify register
  putByte(value); // send data

  digitalWrite(CS_PIN, LOW);
  digitalWrite(CS_PIN, HIGH);
}

void putByte(byte data)
{
  byte i = 8;
  byte mask;
  while (i > 0)
  {
    mask = 0x01 << (i - 1);        // get bitmask
    digitalWrite( CLK_PIN, LOW);   // tick
    if (data & mask)               // choose bit
      digitalWrite(DIN_PIN, HIGH); // send 1
    else
      digitalWrite(DIN_PIN, LOW);  // send 0
    digitalWrite(CLK_PIN, HIGH);   // tock
    --i;                           // move to lesser bit
  }
}
