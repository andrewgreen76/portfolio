#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

void setAddress(int address, bool oe){
  /* Do not copy the same 8b-addr into 15-8 (really 10-8) and 7-0 bits. 
  Capture the top byte (really 3 MSB) with >>8. 
  Upon printing the lower byte the 3 MSB will get shifted out to bits 10-8. 
  */
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (oe ? 0x00 : 0x80) ); 
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);
  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

byte readEEPROM(int addr){
  for(int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1){
    pinMode(pin, INPUT);
  }

  setAddress(addr, true /*oe-*/ );
  byte data=0;
  for(int pin = EEPROM_D7; pin >= EEPROM_D0; pin--){
    data = (data << 1) + digitalRead(pin);
  }

  return data;
}

void writeEEPROM(int address, byte data) {

  setAddress(address, /*oe-*/ false);

  for(int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1){
    pinMode(pin, OUTPUT);
  }

  for(int pin=EEPROM_D0; pin <= EEPROM_D7; pin +=1){
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }

  digitalWrite(WRITE_EN, LOW);
  
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(5);
}

void printContents(){    
  for(int base=0; base <= 255; base += 16){
    byte data[16];
    for(int offset=0; offset <= 15; offset += 1){
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:   %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x", 
      base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], 
      data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]
      );
    Serial.println(buf);
  }
}

// 4-bit hex decoder for common cathode 7-segment display
//byte data[] = { 0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b, 0x77, 0x1f, 0x4e, 0x3d, 0x4f, 0x47 };

void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT); 
  Serial.begin(4800);



  // Program data bytes
  Serial.println("Programming EEPROM");

  byte digits[] = { 0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b};

  Serial.println("Programming ones place");
  for(int value = 0; value <= 255; value += 1){
    writeEEPROM(value, digits[value%10]); 
  }

  Serial.println("Programming tens place");
  for(int value = 0; value <= 255; value += 1){
    writeEEPROM(value+256, digits[(value/10)%10] ); 
  }

  Serial.println("Programming hundreds place");
  for(int value = 0; value <= 255; value += 1){
    writeEEPROM(value+512, digits[(value/100)%10]); 
  }

  Serial.println("Programming sign");
  for(int value = 0; value <= 255; value += 1){
    writeEEPROM(value+768, 0); 
  }
  /* ================ Negative half: ================ */
  Serial.println("Programming ones place, 2's complement");
  for(int value = -128; value <= 127; value += 1){
    writeEEPROM( (byte)value + 1024, digits[abs(value)%10] );
  }

  Serial.println("Programming tens place, 2's complement");
  for(int value = -128; value <= 127; value += 1){
    writeEEPROM( (byte)value + 1280, digits[abs(value/10)%10] );
  }

  Serial.println("Programming hundreds place, 2's complement");
  for(int value = -128; value <= 127; value += 1){
    writeEEPROM( (byte)value + 1536, digits[abs(value/100)%10] );
  }

  Serial.println("Programming sign, 2's complement");
  for(int value = -128; value <= 127; value += 1){
    if(value<0) {
      writeEEPROM( (byte)value + 1792, 0x01 );
    } else {
      writeEEPROM( (byte)value + 1792, 0 );
    }
  }

  // Read and print out the contents of the EERPROM
  Serial.println("Reading EEPROM");
  printContents();

  //Serial.println((char)13);

  //Serial.println(readEEPROM(1));
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
  
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, 0x55);
  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);*/
}
