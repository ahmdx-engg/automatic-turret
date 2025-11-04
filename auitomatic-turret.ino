#include <ESP32Servo.h>
#include <Wire.h>

Servo myservo;   // create servo object to control a servo
int flag=0;
char State;
int directions[2] = {40,40};
bool isMoving = false;

struct eyes {
  int trigPin;
  int echoPin;
  float distance;
  float arr[50];
};
float defaultDistance = 25;

eyes A = {4, 16, 0};
eyes B = {26, 27, 0};

// Function prototypes
void ultrasonicTask(void *pvParameters);

void setup() {
  Serial.begin(9600);
  myservo.setPeriodHertz(50);
  myservo.attach(2, 1000, 2000);

  pinMode(A.trigPin, OUTPUT);
  pinMode(A.echoPin, INPUT);
  pinMode(B.trigPin, OUTPUT);
  pinMode(B.echoPin, INPUT);

  // Create a task that will be executed in the ultrasonicTask function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
    ultrasonicTask, /* Task function. */
    "UltrasonicTask", /* name of task. */
    10000, /* Stack size of task */
    NULL, /* parameter of the task */
    1, /* priority of the task */
    NULL, /* Task handle to keep track of created task */
    1); /* pin task to core 0 */  
  delay(500); // Short delay before starting the loop
}

void loop() {


  Serial.print("A=");  Serial.println(A.distance);
  Serial.print("B=");  Serial.println(B.distance);Serial.println("\n");
  switch (State) {
    case 'A':
      myservo.write(90);
      Serial.println("stay to a");
      break;
    case 'B':
      myservo.write(180);delay(500);
      Serial.println("go to b");
      break;
    case 'C':
      if (flag<100) {if(flag%5) {setSpeed(directions[0]);}} 
      else {if(flag%5) {setSpeed(directions[1]);}}

      if (flag==200) {flag=0;}
      flag++;
  };


  
}

// Function to read ultrasonic sensor values
void ultrasonicTask(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    // Read ultrasonic sensor A
    digitalWrite(A.trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(A.trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(A.trigPin, LOW);
    A.distance = pulseIn(A.echoPin, HIGH) / 58.2;
    

    // Read ultrasonic sensor B
    digitalWrite(B.trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(B.trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(B.trigPin, LOW);
    B.distance = pulseIn(B.echoPin, HIGH) / 58.2;

    if (A.distance < defaultDistance && A.distance < B.distance && A.distance > 2) {
      State = 'A';
    } else if (B.distance < defaultDistance && B.distance > 2) {
      State = 'B';
    } else {
      State = 'C';
    }
    

    // Add a non-blocking delay between readings
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}




void setSpeed(int x) {

  isMoving = !isMoving;
  if (isMoving) {
    myservo.write(x);
  } else {
    myservo.write(90);
  }

}