// Each row is an instruction containing an LED configuration and the reset bit.
const unsigned char source[] = {
//  L1  L2  L3  L4  L5  L6  L7  L8  L9  L10 L11 L12 L13 L14 L15 L16 L17 L18 L19 L20 L21 L22 RST 
    1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,
    1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,
    1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,
    1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,
    1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,
    1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,
    1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  // Reset instruction, do not remove.
};

const unsigned char instructionSize = 23;

// Pin mapping.
const unsigned char led[22] = {26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47};
const unsigned char xclk = 22;
const unsigned char xrst = 23;
const unsigned char xwe  = 24;
const unsigned char rst  = 25;

void flash() {
  // Reset the program counter.
  digitalWrite(xrst, HIGH);
  delay(100);
  digitalWrite(xrst, LOW);
  delay(100);

  // Iterate through instructions and write them.
  for(int i = 0; i < sizeof(source); i += instructionSize) {
    // Load the LED configuration from the current instruction.
    for(int j = 0; j < sizeof(led); j++) {
      digitalWrite(led[j], source[i + j]);
    }

    // Load the reset bit from the current instruction.
    digitalWrite(rst, source[i + instructionSize - 1]);

    // Let the data stabilise.
    delay(100);

    // Write the data.
    digitalWrite(xwe, LOW);
    delay(100);
    digitalWrite(xwe, HIGH);
    delay(100);

    // Advance to the next memory address.
    for(int j = 0; j < 8; j++) { // Must be done 8 times becuase the program counter advances on the 8th pulse of the clock.
      digitalWrite(xclk, HIGH);
      delay(100);
      digitalWrite(xclk, LOW);
      delay(100);
    }
  }

  // Turn on the builtin LED to signal that user has 10 seconds to flip the top RUN/FLASH switch back to RUN.
  digitalWrite(LED_BUILTIN, HIGH);
  delay(10000);
  digitalWrite(LED_BUILTIN, LOW);

  // Reset program counter.
  digitalWrite(xrst, HIGH);
  delay(100);
  digitalWrite(xrst, LOW);
  delay(100);

  // Turn on built-in LED to signal that the user can flip the bottom RUN/FLASH switch back to RUN,
  // as well as disconnect the flash interface and unpause the clock.
  digitalWrite(LED_BUILTIN, HIGH);
}

void setup() {
  // Set all pins to output mode and give them an initial state.
  for(int i = 0; i < sizeof(led); i++) {
    pinMode(led[i], OUTPUT);
    digitalWrite(led[i], LOW);
  }

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(rst,         OUTPUT);
  pinMode(xrst,        OUTPUT);
  pinMode(xclk,        OUTPUT);
  pinMode(xwe,         OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(rst,         LOW);
  digitalWrite(xrst,        LOW);
  digitalWrite(xclk,        LOW);
  digitalWrite(xwe,         HIGH);

  // Let the outputs stabilise and run the flash function.
  delay(100);
  flash();
}

void loop() {

}
