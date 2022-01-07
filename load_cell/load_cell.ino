#define MAX_BUFFER 128
#define NUMBER_OF_MACHINE 3

float start_time;
float current_time;
float scale_factor;

typedef union _Buffer {
	long readable;
	uint8_t buffer[4];
} Buffer;

typedef union _StringBuffer {
	float float_array[NUMBER_OF_MACHINE];
	uint8_t buffer[sizeof(float) * NUMBER_OF_MACHINE];
} StringBuffer;

void send (uint8_t *buffer, uint8_t size);

void init_hx711 (uint8_t SCK, uint8_t DT);

uint8_t read_byte (uint8_t SCK, uint8_t DT);

float read_total (uint8_t SCK, uint8_t DT);

void turn_off ();

void setup() {
	uint8_t pd_sck_list[NUMBER_OF_MACHINE];
	pd_sck_list[0] = 8;
	pd_sck_list[1] = 10;

	Serial.begin(9600);
	init_hx711(8, 9);
	init_hx711(10, 11);
	init_hx711(12, 13);

	scale_factor = 100.0;
	//turn_off (pd_sck_list, NUMBER_OF_MACHINE);
}

void loop() {
	StringBuffer string_buffer;
	start_time = millis();
	
	string_buffer.float_array[0] = read_total(10, 11);
	Serial.print("A");
	current_time = millis();
	Serial.println(current_time - start_time);	

	string_buffer.float_array[1] = read_total(8, 9);
	Serial.print("B");
	current_time = millis();
	Serial.println(current_time - start_time);	

	string_buffer.float_array[2] = read_total(12, 13);
	Serial.print("C");
	current_time = millis();
	Serial.println(current_time - start_time);	
	
	//send (string_buffer.buffer, sizeof(string_buffer.buffer));
}

void init_hx711(uint8_t SCK, uint8_t DT) {
	pinMode(SCK, OUTPUT);
	pinMode(DT, INPUT);
}

uint8_t read_byte(uint8_t SCK, uint8_t DT) {
	uint8_t value = 0;
  	uint8_t mask = 0x80;
  	while (mask > 0) {
    		digitalWrite(SCK, HIGH);
    		delayMicroseconds(1);     
    		if (digitalRead(DT) == HIGH) {
      			value |= mask;
    		}
    		digitalWrite(SCK, LOW);
    		delayMicroseconds(1);              
    		mask >>= 1;
  	}
  	return value;
}

float read_total (uint8_t PD_SCK, uint8_t DOUT) {
	while(digitalRead(DOUT) != LOW);
	Buffer v;

  	v.buffer[2] = read_byte(PD_SCK, DOUT);
 	v.buffer[1] = read_byte(PD_SCK, DOUT);
  	v.buffer[0] = read_byte(PD_SCK, DOUT);

    	digitalWrite(PD_SCK, HIGH);
    	digitalWrite(PD_SCK, LOW);

	// conversion to signed 32bit
  	if (v.buffer[2] & 0x80) v.buffer[3] = 0xFF;

  	return 1.0 * v.readable;
}

void send (uint8_t *buffer, uint8_t size) {
	StringBuffer st;
	for (uint8_t i = 0; i < size; i++) {
		st.buffer[i] = buffer[i];		
	}
	for (uint8_t i = 0; i < NUMBER_OF_MACHINE; i++) {
		Serial.println(st.float_array[i]);
	}
}

void turn_off (uint8_t *PD_SCK, uint8_t size) {
	for (uint8_t i = 0; i < size; i++) {
		digitalWrite(PD_SCK[i], HIGH);
	}
	delayMicroseconds(100);
	for (uint8_t i = 0; i < size; i++) {
		digitalWrite(PD_SCK[i], LOW);
	}
}
