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

#define COMMAND_STATUS    '?'
#define COMMAND_CONFIG    'c'
#define COMMAND_RELOAD    'r'
#define COMMAND_SAVE      's'

// not including newline
#define PACKET_SIZE 12

// setting value addresses in EEPROM
#define ADDR_EXP_RATE   12
#define ADDR_EXP_TIME   16
#define ADDR_DUTY_CYCLE 20

unsigned long t_off = 900;  // in ms
unsigned long t_on  = 100;
unsigned long t_exp  = 16;


struct BUFFER {
  unsigned long expRate, expTime, dutyCycle;
};

BUFFER settings;

void setup() {
  Serial.begin(9600);

  // configure i/o
  setupPins();

  // update settings from EEPROM
  readConfigVals();

  generateTimingValues();

  Serial.println("Finished Setup");
  blinkOK();
}

void loop() {
  
  if ( Serial.available() ) handleSerial();

  unsigned long now = millis();

  digitalWrite( TO_PROJ, HIGH );

  while ( !Serial.available() && millis() - now < t_on);
  if (Serial.available()) handleSerial();

  digitalWrite( TO_PROJ, LOW );
  // projector sends back trigger pulses
  // after falling edge

  // timestamp to avoid time out if projector
  // never responds


  // wait until the projector sends the first
  // of the trigger pulses
  //while( !digitalRead( FROM_PROJ ) && millis()-now < t_off );

  now = millis();
  while ( !Serial.available() && millis() - now < t_on && !digitalRead( FROM_PROJ ));
  if (Serial.available()) handleSerial();

  // trigger pulse is wide enough we don't
  // have to worry about it falling before we check here
  if ( digitalRead( FROM_PROJ ) ) {

    // wait till we go low
    while ( digitalRead( FROM_PROJ ) );
    delayMicroseconds(t_exp - 1);

    // pass the pulse on to the camera
    // status blink too
    digitalWrite( TO_CAMERA, HIGH );
    digitalWrite(LED, HIGH);
    delay(t_exp);
    digitalWrite( TO_CAMERA, LOW );
    digitalWrite(LED, LOW);

    // idle and ignore the rest of the pulses
    // from the projector
    //while( millis()-now < t_off );
  }
  while ( millis() - now < t_off );
}

void handleSerial() {
  char c = COMMAND_STATUS;

  c = Serial.read();

  switch ( c ) {
    case COMMAND_STATUS:
      // to populate software fields
      sendConfig();
      blinkOK();
      break;

    case COMMAND_CONFIG:
      if( !parseConfig() )
        blinkError();
      else
        blinkOK();
      break;

    case COMMAND_RELOAD:
      // config stored in EEPROM
      // reset to prior config feature
      readConfigVals();
      blinkOK();
      break;

    case COMMAND_SAVE:
      // save current parameters to EEPROM
      saveConfigVals();
      blinkOK();
      break;
    default:
      blinkError();
  }

  generateTimingValues();

  // flush newline
  if( Serial.peek() == '\n' )
    Serial.read();
}

void generateTimingValues() {
  double t_on_frac = ((double)settings.dutyCycle)/((double)100.0);
  double t_off_frac = 1.0 - t_on_frac;
  
  t_on = (unsigned long)((double)settings.expRate*t_on_frac);
  t_off = (unsigned long)((double)settings.expRate*t_off_frac);
  t_exp = settings.expTime;
}

bool parseConfig() {
  settings.expRate = (unsigned long)Serial.parseInt();
  settings.expTime = (unsigned long)Serial.parseInt();
  settings.dutyCycle = (unsigned long)Serial.parseInt();

  if( settings.expRate != 0 && settings.expTime != 0 && settings.dutyCycle != 0 ){
    saveConfigVals();
    return true;      
  } else {
    return false;
  }
}

bool sendConfig() {
  String buf = String(ID_STRING) + " " + settings.expRate + String(" ") + settings.expTime + String(" ") + settings.dutyCycle;

  Serial.println(buf);

  return true;
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
  return ( EEPROM.read(a) << 24 |
           EEPROM.read(a + 1) << 16 |
           EEPROM.read(a + 2) << 8 |
           EEPROM.read(a + 3) );
}

void writeLongToEEPROM(uint8_t a, unsigned long d) {
  EEPROM.write(a, (uint8_t)(d >> 24));
  EEPROM.write(a + 1, (uint8_t)(d >> 16));
  EEPROM.write(a + 2, (uint8_t)(d >> 8));
  EEPROM.write(a + 3, (uint8_t)(d));
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
  for ( int i = 0; i < 5; i++ ) {
    digitalWrite(LED, HIGH);
    delay(150);
    digitalWrite(LED, LOW);
    delay(50);
  }
}

