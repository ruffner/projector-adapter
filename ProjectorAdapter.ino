// Trigger interface between camera 
// and projector
// Matt Ruffner 2016

#include <EEPROM.h>

// pin definitions
#define TO_PROJ   15
#define FROM_PROJ 14
#define TO_CAMERA 16
#define LED       13

#define ID_LEN 11
#define ID_STRING "MR2016v001"

#define TIMEOUT 5000

#define STATUS    '?'
#define CONFIG    'c'
#define RELOAD    'r'
#define SAVE      's'
#define START     't'

// not including newline
#define PACKET_SIZE 12

// setting value addresses in EEPROM
#define ADDR_EXP_RATE   0
#define ADDR_EXP_TIME   4
#define ADDR_DUTY_CYCLE 8

unsigned long t_off = 900;  // in ms 
unsigned long t_on  = 100;
unsigned long t_exp  = 16;


typedef struct BUFFER {
  unsigned long expRate, expTime, dutyCycle;
};

BUFFER settings;

void setup() {
  Serial.begin(9600);

  // configure i/o
  setupPins();

  // update settings from EEPROM
  readConfigVals();

  // cofig timeout on boot
  unsigned long n = millis();
  while( !Serial.available() && millis() - n < TIMEOUT );

  // didn't timeout
  if( Serial.available() ){
    String cd = "";
    char c = STATUS;

    // allows for multiple configs
    while ( c != START ){
      c = Serial.read();
      
      switch( c ){
        case STATUS:
          // to populate software fields
          if( sendConfig() )
            blinkOK();
          else
            blinkError();
          break;
          
        case CONFIG:
          // incoming config vals after command byte
          while( Serial.available() < PACKET_SIZE );
          if( parseConfig() )
            blinkOK();
          else 
            blinkError();  
          break;
          
        case RELOAD:
          // config stored in EEPROM
          // reset to prior config feature
          readConfigVals();
          blinkOK();
          break;
          
        case SAVE:
          // save current parameters to EEPROM
          saveConfigVals();
          blinkOK();
          break;
        default:
          blinkError();
      }
      
      // flush newline
      Serial.read();
    }
    
  }

  t_on = 100;
  t_off = 900;
  t_exp = 16;

  blinkOK();
}

void loop() {
  
  digitalWrite( TO_PROJ, HIGH );
  delay(t_on);
  digitalWrite( TO_PROJ, LOW );
  // projector sends back trigger pulses
  // after falling edge

  // timestamp to avoid time out if projector
  // never responds
  unsigned long now = millis();

  // wait until the projector sends the first
  // of the trigger pulses
  while( !digitalRead( FROM_PROJ ) && millis()-now < t_off );

  // trigger pulse is wide enough we don't
  // have to worry about it falling before we check here
  if( digitalRead( FROM_PROJ ) ){

    // wait till we go low
    while( digitalRead( FROM_PROJ ) );
    delayMicroseconds(t_exp-1);
      
    // pass the pulse on to the camera
    // status blink too
    digitalWrite( TO_CAMERA, HIGH );
    digitalWrite(LED, HIGH);
    delay(t_exp);
    digitalWrite( TO_CAMERA, LOW );
    digitalWrite(LED, LOW);

    // idle and ignore the rest of the pulses
    // from the projector
    while( millis()-now < t_off );
  }
  
}

bool parseConfig() {
  uint16_t cka, ckb, ckck;

  settings.expRate = readLongFromSerial();
  settings.expTime = readLongFromSerial();
  settings.dutyCycle = readLongFromSerial();

  cka = Serial.read();
  ckb = Serial.read();

  ckck = checksum();

  if( ckck == ((cka << 8) | ckb) )
    return true;
  else
    return false;
}

bool sendConfig() {
  uint16_t ckck = checksum();

  String buf = ID_STRING;

  uint8_t *p = (uint8_t *)settings.expRate;
  for( int i=0; i<ID_LEN; i++,p++ )
    buf[i] = *p;

  for( int i=0; i<4; i++ )
    buf[ID_LEN+i] = (unsigned char)(settings.expTime >> 8*i);
  

  
  buf[ID_LEN+PACKET_SIZE] = (uint8_t)(ckck >> 8);
  buf[ID_LEN+PACKET_SIZE+1] =  (uint8_t)(ckck);
  
  return true;
}

uint16_t checksum(){
  uint8_t *p = (uint8_t *)settings.expRate, cka, ckb;
  
  for( int i=0; i<PACKET_SIZE; i++,p++ ){
    cka += *p;
    ckb += cka;
  }

  return (uint16_t) ((cka << 8) | ckb);
}

void readConfigVals() {
  settings.expRate = readLongFromEEPROM(ADDR_EXP_RATE);
  settings.expTime = readLongFromEEPROM(ADDR_EXP_TIME);
  settings.dutyCycle = readLongFromEEPROM(ADDR_DUTY_CYCLE);
}

void saveConfigVals() {
  writeLongToEEPROM( ADDR_EXP_RATE, settings.expRate );
  writeLongToEEPROM( ADDR_EXP_TIME, settings.expTime );
  writeLongToEEPROM( ADDR_DUTY_CYCLE, settings.dutyCycle );
}

unsigned long readLongFromSerial() {
  return (Serial.read() << 24 |
          Serial.read() << 16 | 
          Serial.read() << 8 | 
          Serial.read());
}

unsigned long readLongFromEEPROM(uint8_t a) {
  return( EEPROM.read(a) << 24 | 
          EEPROM.read(a+1) << 16 |
          EEPROM.read(a+2) << 8 |
          EEPROM.read(a+3) );
}

void writeLongToEEPROM(uint8_t a, unsigned long d) {
  EEPROM.write(a, (uint8_t)(d >> 24));
  EEPROM.write(a, (uint8_t)(d >> 16));
  EEPROM.write(a, (uint8_t)(d >> 8));
  EEPROM.write(a, (uint8_t)(d));
}

void setupPins() {
  // trigger signal to projector
  pinMode( TO_PROJ, OUTPUT );

  // camera trigger signal from projector
  // we debounce this and pass pulse to camera
  pinMode( FROM_PROJ, INPUT );

  // trigger line to camera
  pinMode( TO_CAMERA, OUTPUT );

  // onboard status LED
  pinMode( LED, OUTPUT) ;
}

void blinkOK() {
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
}

void blinkError() {
  for( int i=0; i<5; i++ ){
    digitalWrite(LED, HIGH);
    delay(150);
    digitalWrite(LED, LOW);
    delay(50);
  }
}

