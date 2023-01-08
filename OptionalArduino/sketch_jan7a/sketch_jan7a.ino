// #include <Mouse.h>

int buttonl = 7;
int buttonr = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buttonl, INPUT_PULLUP);
  pinMode(buttonr, INPUT_PULLUP);

  // Mouse.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  int statel = digitalRead(buttonl);
  if (statel==0){
    Serial.print("l0;");
    // Mouse.press(MOUSE_LEFT)
  }
  else{
    Serial.print("l1;");
    // Mouse.release(MOUSE_LEFT);
  }
  
  int stater = digitalRead(buttonr); 
  if (stater==0){
    Serial.print("r0;");
    // Mouse.press(MOUSE_RIGHT)
  }
  else{
    Serial.print("r1;");
    // Mouse.release(MOUSE_RIGHT);
  }

  delay(15);
}
