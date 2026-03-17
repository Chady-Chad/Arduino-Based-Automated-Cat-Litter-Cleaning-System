#include <Servo.h>

const int trigPin = 9;        // Trigger pin for ultrasonic sensor
const int echoPin = 10;       // Echo pin for ultrasonic sensor
const int ledPin1 = 13;       // First LED pin indicate as green.
const int ledPin2 = 7;        // Secondary LED pin indicate as red.
const int in1 = 2;            // Motor Driver Input in1
const int in2 = 3;            // Motor Driver Input in2
const int in3 = 4;            // Motor Driver Input in3
const int in4 = 5;            // Motor Driver Input in4
const int servoPin = 6;       // Pin for servo motor
const int switchPin1 = 8;     // Pin for spdt switch for counterclockwise
const int switchPin2 = 11;    // Pin for spdt switch for clockwise

long duration;                 // Duration of the ultrasonic pulse
int distance;                  // This is the distance for sensing parameter of sonar.
bool isObjectDetected = false; // This indicates if an object is detected.
unsigned long lastObjectDetectedTime = 0; // Time when the last object was detected. This is crucial for loop process. Don't change it except for special function.
const unsigned long delayTime = 8000; // Delay time in milliseconds (8 seconds)

Servo servoMotor;            

void setup() {
  Serial.begin(9600); // this is only for serial monitor baud rate data communication. Don't alter this.
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin1, OUTPUT);     // Initialize the first green LED
  pinMode(ledPin2, OUTPUT);    // Initialize the secondary red LED
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(servoPin, OUTPUT);
  pinMode(switchPin1, INPUT_PULLUP); // SPDT switch counterclockwise
  pinMode(switchPin2, INPUT_PULLUP); // SPDT switch counterclockwise

  servoMotor.attach(servoPin); // Attach the servo signal to its pin 6. (Color coded: yellow or white wire)
  
  digitalWrite(ledPin1, LOW);  // Initial turn off the first LED. Don't change it to HIGH logic.
  digitalWrite(ledPin2, LOW);  // Initially turn off the secondary LED

  // Set initial position of servo to 60 degrees
  servoMotor.write(60);
  delay(1000); // Allow time for the servo to reach its initial position
}

void loop() {
  // Ultrasonic sensor routine/functionality
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; //Don't change the value except for modification of distance and the type of a sensor or sonar to be used.

  Serial.print("The distance of an echo: "); //You can check the distance in serial monitor
  Serial.print(distance);
  Serial.println("In cm"); // same with cm.


  // Obstacle detection routine
  if (distance < 33) { // You can change the value. the limit of distance in cm is 2cm to 400 cm.
    digitalWrite(ledPin1, HIGH); // Turn on first green LED
    isObjectDetected = true;
    lastObjectDetectedTime = millis(); // Update the time when the last object was detected
  } 
  
  
  else {
    digitalWrite(ledPin1, LOW); // Turn off first green LED
    unsigned long currentTime = millis();
    if (isObjectDetected && (currentTime - lastObjectDetectedTime >= delayTime)) {
      // If no object has been detected for 8 seconds then:

      // Move servo counterclockwise
      digitalWrite(ledPin2, HIGH); // Turn on secondary LED
      servoMotor.write(60); // Rotate servo clockwise to 60 degrees
      
      // Start motor clockwise until it reaches the switch to be triggered and stop.
      if (digitalRead(switchPin2) == HIGH){
        startMotorClockwise();
        while (digitalRead(switchPin2) == HIGH) {}
        stopMotor();
      }

      delay(1000); // Delay before changing direction. You can change the value according to your desired time preference of waiting before moving to counterclockwise

      // Move servo clockwise
      servoMotor.write(0); // Rotate servo counterclockwise to 0 degrees


      // Start motor counterclockwise until it reaches the switch to be triggered and stop.
      if (digitalRead(switchPin1) == HIGH) {
        startMotorCounterclockwise();
        while (digitalRead(switchPin1) == HIGH) {} // Wait until the switch is pressed
        stopMotor(); // Stop the motor when the switch is pressed
      }


      digitalWrite(ledPin2, LOW); // Turn off secondary LED
      delay(3000); // Delay before going back to the first LED
      
      // Wait before returning the servo to its initial position
      delay(3000); // Adjust this delay as needed.

      // Move servo back to initial position
      servoMotor.write(60); // Rotate servo back to 60 degrees
      isObjectDetected = false;
    }
  }
  
  delay(1000); // Wait before next sensor reading
}

// Motor control functions
void startMotorClockwise() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void startMotorCounterclockwise() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void stopMotor() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
