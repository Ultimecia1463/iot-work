#include "Arduino.h"
#include "WiFi.h"
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "driver/rtc_io.h"
#include <LittleFS.h>
#include <FS.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>

const char* ssid = "ultimecia";
const char* password = "ultimecia";

#define API_KEY "AIzaSyBmpU5gUjG5zPy7-oQd-VGKcfnAXtAp6hM"
#define USER_EMAIL "keshu1234harshu@gmail.com"
#define USER_PASSWORD "ultimecia69"
#define STORAGE_BUCKET_ID "project1-cc25e.appspot.com"

#define BUCKET_PHOTO "/photos/"

// Define the path to save photos on LittleFS
#define FILE_PHOTO_PATH "/photo.jpg"

// Define the Firebase Data object and Firebase Authentication object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig configF;
bool taskCompleted = false;

// Function to capture a photo and save it to LittleFS
void capturePhotoSaveLittleFS() {
  camera_fb_t* fb = NULL;

  for (int i = 0; i < 4; i++) {
    fb = esp_camera_fb_get();
    esp_camera_fb_return(fb);
    fb = NULL;
  }

  fb = esp_camera_fb_get();

  if (!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
  }

  // Get the current time
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  // Generate a filename with the current date and time
  String filename = "/photo_";
  filename += timeinfo.tm_year + 1900; // Year is since 1900
  filename += timeinfo.tm_mon + 1; // Month starts from 0
  filename += timeinfo.tm_mday;
  filename += "_";
  filename += timeinfo.tm_hour;
  filename += timeinfo.tm_min;
  filename += timeinfo.tm_sec;
  filename += ".jpg";

  Serial.printf("Picture file name: %s\n", filename);
  File file = LittleFS.open(filename, FILE_WRITE);

  if (!file) {
    Serial.println("Failed to open file in writing mode");
  } else {
    file.write(fb->buf, fb->len);
    Serial.print("The picture has been saved in ");
    Serial.print(filename);
    Serial.print(" - Size: ");
    Serial.print(fb->len);
    Serial.println(" bytes");
  }
  file.close();
  esp_camera_fb_return(fb);
}

// Initialize WiFi
void initWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

// Initialize LittleFS
void initLittleFS() {
  if (!LittleFS.begin(true)) {
    Serial.println("An Error has occurred while mounting LittleFS");
    ESP.restart();
  } else {
    delay(500);
    Serial.println("LittleFS mounted successfully");
  }
}

// Initialize the camera
void initCamera() {
  camera_config_t config;
  // Configure your camera pins here
  // ...

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  }
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  initLittleFS();
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  initCamera();

  configF.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  configF.token_status_callback = tokenStatusCallback;

  Firebase.begin(&configF, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Capture a new photo and save it to LittleFS
  capturePhotoSaveLittleFS();

  if (Firebase.ready() && !taskCompleted) {
    taskCompleted = true;
    Serial.print("Uploading picture... ");

    // Get the current time
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }

    String remotePath = BUCKET_PHOTO;
    remotePath += String(timeinfo.tm_year + 1900);
    remotePath += String(timeinfo.tm_mon + 1);
    remotePath += String(timeinfo.tm_mday);
    remotePath += "/";
    remotePath += String(timeinfo.tm_hour);
    remotePath += String(timeinfo.tm_min);
    remotePath += String(timeinfo.tm_sec);
    remotePath += ".jpg";


    if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID, remotePath, mem_storage_type_flash, remotePath, "image/jpeg", fcsUploadCallback)) {
      Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
    } else {
      Serial.println(fbdo.errorReason());
    }
  }
}

void fcsUploadCallback(FCS_UploadStatusInfo info) {
  if (info.status == firebase_fcs_upload_status_init) {
    Serial.printf("Uploading file %s (%d) to %s\n", info.localFileName.c_str(), info.fileSize, info.remoteFileName.c_str());
  } else if (info.status == firebase_fcs_upload_status_upload) {
    Serial.printf("Uploaded %d%s, Elapsed time %d ms\n", (int)info.progress, "%", info.elapsedTime);
  } else if (info.status == firebase_fcs_upload_status_complete) {
    Serial.println("Upload completed\n");
    Serial.printf("Download URL: %s\n", fbdo.downloadURL().c_str()); // Use fbdo to access the download URL
  }
}


void tokenStatusCallback(int httpCode, Stream& stream) {
  if (httpCode == -1000) {
    Serial.println("Token is unavailable or invalid");
    Serial.println("Restarting ESP32...");
    delay(1000);
    ESP.restart();
  }
}
