// Trigger interface between camera 
// and projector
// Matt Ruffner 2016

#define TO_PROJ   15
#define FROM_PROJ 14
#define TO_CAMERA 16

void setup() {
  // put your setup code here, to run once:

  // 1Hz trigger signal to projector
  pinMode( TO_PROJ, OUTPUT );

  // camera trigger signal from projector
  // we debounce this and pass pulse to camera
  pinMode( FROM_PROJ, INPUT );

  // trigger line to camera
  pinMode( TO_CAMERA, OUTPUT );

  // onboard status LED
  pinMode(13, OUTPUT);
}

void loop() {
  // 20% duty cycle @ 1Hz
  // beggining edge
  digitalWrite( TO_PROJ, HIGH );
  delay(100);
  digitalWrite( TO_PROJ, LOW );
  // projector sends back trigger pulses
  // after falling edge

  // timestamp to avoid time out if projector
  // never responds
  unsigned long now = millis();

  // wait until the projector sends the first
  // of the trigger pulses
  while( !digitalRead( FROM_PROJ ) && millis()-now < 900 );

  // trigger pulse is wide enough we don't
  // have to worry about it falling before we check here
  if( digitalRead( FROM_PROJ ) ){

    // wait till we go low
    while( digitalRead( FROM_PROJ ) );
    delayMicroseconds(3000);
      
    // pass the pulse on to the camera
    // status blink too
    digitalWrite( TO_CAMERA, HIGH );
    digitalWrite(13, HIGH);
    delay(18);
    digitalWrite( TO_CAMERA, LOW );
    digitalWrite(13, LOW);

    // idle and ignore the rest of the pulses
    // from the projector
    while( millis()-now < 900 );
  }
  
}
