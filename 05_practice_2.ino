#define PIN_7
unsigned int count, toggle,cnt;

void setup() {
  pinMode(PIN7, OUTPUT);
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  Serial.println("Hello World!");
  count = toggle = 1;
  cnt = 0;
  digitalWrite(PIN7, toggle); 
}

void loop() {
  Serial.println(cnt);
  if(cnt<=10)
    toggle = toggle_state(toggle);
  else
  {
    while(1)
    {
      digitalWrite(PIN7, 1);
    }
  }
  digitalWrite(PIN7, toggle);
  if(cnt ==0)
  {
    cnt+=1;
    delay(1000);
  }
    
  else if(cnt<=10)
  {
    delay(100);
    cnt+=1;
  }
}

int toggle_state(int toggle) {
  if(toggle == 0)
  {
    toggle = 1;
    return toggle;
  }
  else
  {
    toggle = 0;
    return toggle;
  }
}
