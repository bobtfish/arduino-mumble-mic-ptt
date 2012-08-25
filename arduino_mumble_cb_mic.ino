void setup();
void loop();

#define KEY_LEFT_CTRL	0x01
#define KEY_LEFT_SHIFT	0x02
#define KEY_RIGHT_CTRL	0x10
#define KEY_RIGHT_SHIFT	0x20

#define LED 13       // Digital output for LED display of PTT function. 13 is the board built in LED.
#define PIN_PTT 7    // The IO pin for the PTT button, floats high, so wire to ground for PTT.
#define PTT_KEY "g"  // A specific ASCII key - note that + and - are mapped to volume up and volume down
#define PTT_MOD 0x10 // Note this is one of the above KEY_(LEFT|RIGHT)_(CTRL|SHIFT) or 0x00 for none.

void setup() {
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
    pinMode(PIN_PTT, INPUT);
    delay(5000);
}

void loop() {
    int state = 1;
    state = digitalRead(PIN_PTT);
    if (state != 1) {
        digitalWrite(LED, HIGH);
        sendKey(PTT_KEY, PTT_MOD);
        while (state != 1) {
            state = digitalRead(PIN_PTT);
        }
        digitalWrite(LED, LOW);
        releaseKey();
    }
}

void sendKey (char *ch, int mod) {
      uint8_t buf[8] = { 0 };	/* Keyboard report buffer */
  
	if ((*ch >= 'a') && (*ch <= 'z')) {
	    buf[2] = *ch - 'a' + 4;
	} else if ((*ch >= 'A') && (*ch <= 'Z')) {
	    buf[0] = KEY_LEFT_SHIFT;	/* Caps */
	    buf[2] = *ch - 'A' + 4;
	} else {
	    switch (*ch) {
	    case ' ':
	    	buf[2] = 0x2c;	// Space
		break;
	    case '+':
	       buf[2] = 128;	// Volume up
	       break;
	    case '-':
	       buf[2] = 129;	// Volume down
	       break;
	    default:
	        /* Character not handled. To do: add rest of chars from HUT1_11.pdf */
		buf[2] = 0x37;	// Period
		break;
	    }
	}
    buf[0] = buf[0] | mod;

    Serial.write(buf, 8); // Send keypress
    delay(125);
}

void releaseKey() {
  uint8_t mybuf[8] = { 0 };
  mybuf[0] = 0;
  mybuf[2] = 0;
  Serial.write(mybuf, 8); // Release key
  delay(125);
}

