#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>

// Define Fingerprint Sensor RX and TX pins
SoftwareSerial mySerial(2, 3); // RX, TX pins for the fingerprint sensor
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Relay pin for controlling the lock
const int relayPin = 7;

// Setup function
void setup() {
  // Start the serial communication
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(relayPin, OUTPUT);  // Set relay pin as an output

  // Initialize fingerprint sensor
  if (finger.begin()) {
    Serial.println("Fingerprint sensor initialized successfully.");
  } else {
    Serial.println("Fingerprint sensor not found. Please check the wiring.");
    while (1);  // Halt the program if sensor is not found
  }

  // Check if the sensor is ready
  finger.getTemplateCount();  
}

// Main loop
void loop() {
  int fingerprintID = getFingerprintID();
  if (fingerprintID >= 0) {
    Serial.print("Fingerprint matched with ID: ");
    Serial.println(fingerprintID);
    unlock();
    delay(3000); // Keep the lock open for 3 seconds
  } else {
    Serial.println("No match found.");
    delay(1000);  // Wait before trying again
  }
}

// Function to get the fingerprint ID
int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) {
    Serial.println("Failed to capture image.");
    return -1;  // Return -1 if fingerprint capture fails
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) {
    Serial.println("Failed to convert image.");
    return -1;
  }

  p = finger.fingerSearch();
  if (p != FINGERPRINT_OK) {
    Serial.println("No match found.");
    return -1;
  }

  return finger.fingerID;  // Return the matched fingerprint ID
}

// Function to unlock the system
void unlock() {
  // Activate the relay to unlock the lock
  digitalWrite(relayPin, HIGH);
  Serial.println("Lock unlocked!");

  // Wait for 3 seconds
  delay(3000);

  // Deactivate the relay to lock the system again
  digitalWrite(relayPin, LOW);
  Serial.println("Lock locked!");
}

