#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "ansh"
#define WIFI_PASSWORD "12345678"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY ""

/* 3. Define the RTDB URL */
#define DATABASE_URL "" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app


//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;


String klop="";
String fullName="";
String timeStamp="";

int a, b, x, y;


void setup()
{
  Serial.begin(115200);
  delay(2000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Initialize the Firebase configuration
  FirebaseConfig config;
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Initialize the FirebaseESP32 library
  Firebase.begin(&config, &auth);

  // Rest of your setup code remains unchanged
  // ...
}


void loop() {
  // Set integer values and get the full name from the serial monitor
  if (Firebase.ready()) {
    // Generate random integers
    x = random(0, 9);
    y = random(10, 19);

    // Get the full name from the serial monitor
    if (Serial.available() > 0) {
      fullName = Serial.readStringUntil('\n');
    }
    delay(200);

    // Set integer values
    Firebase.setInt(fbdo, "/test/a", x);
    Firebase.setInt(fbdo, "/test/b", y);

    // Set string value
    if (Firebase.setString(fbdo, "/test/fullName", fullName)) {
      Serial.println("Full name has been successfully set in Firebase!");
    } else {
      Serial.println("Failed to set full name in Firebase. " + fbdo.errorReason());
    }

    delay(200);

    // Capture the timestamp using the millis() function
    String timestamp = String(millis());

    // Set the timestamp in the Firebase Realtime Database
    if (Firebase.setString(fbdo, "/test/timestamp", timestamp)) {
      Serial.println("Timestamp has been successfully set in Firebase!");
    } else {
      Serial.println("Failed to set timestamp in Firebase. " + fbdo.errorReason());
    }

    // Print the rest of the data as before
    // ...

    delay(2500); // Wait for 2.5 seconds before setting the values again
  }
}
