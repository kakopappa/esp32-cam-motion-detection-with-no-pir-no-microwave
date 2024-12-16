#include <Arduino.h>

#include <esp_camera.h>

// ===================
// Select camera model
// ===================
// #define CAMERA_MODEL_WROVER_KIT  // Has PSRAM
// #define CAMERA_MODEL_ESP_EYE  // Has PSRAM
// #define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
// #define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
// #define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
// #define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
// #define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
// #define CAMERA_MODEL_M5STACK_UNITCAM // No PSRAM
// #define CAMERA_MODEL_M5STACK_CAMS3_UNIT  // Has PSRAM
#define CAMERA_MODEL_AI_THINKER  // Has PSRAM
// #define CAMERA_MODEL_TTGO_T_JOURNAL  // No PSRAM
// #define CAMERA_MODEL_TTGO_T_V16_VERSION
// #define CAMERA_MODEL_TTGO_T_V05_VERSION
// #define CAMERA_MODEL_TTGO_T_PLUS_VERSION
// #define CAMERA_MODEL_TTGO_T_V162_VERSION
// #define CAMERA_MODEL_TTGO_T_MINI_VERSION
// #define CAMERA_MODEL_TTGO_T_V17_VERSION
// #define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
// ** Espressif Internal Boards **
// #define CAMERA_MODEL_ESP32_CAM_BOARD
// #define CAMERA_MODEL_ESP32S2_CAM_BOARD
// #define CAMERA_MODEL_ESP32S3_CAM_LCD
// #define CAMERA_MODEL_DFRobot_FireBeetle2_ESP32S3 // Has PSRAM
// #define CAMERA_MODEL_DFRobot_Romeo_ESP32S3 // Has PSRAM

#include "camera_pins.h"
#include "motion_detector.h"

// More sensitive to large moving objects
// #define BLOCK_THRESHOLD 40    // Require stronger pixel changes
// #define MOTION_THRESHOLD 15   // Require more blocks to change

// More sensitive to mid moving objects
#define BLOCK_THRESHOLD 30   // Accept subtler pixel changes
#define MOTION_THRESHOLD 10  // Accept mid changed blocks

// More sensitive to small moving objects
//  #define BLOCK_THRESHOLD 20    // Accept subtler pixel changes
//  #define MOTION_THRESHOLD 5    // Accept fewer changed blocks

MotionDetector* motionDetector = nullptr;
bool motionDetected = false;

void setupCamera() {
  Serial.printf("Setup camera..\r\n");

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA;  //  https://github.com/espressif/esp32-camera/blob/master/driver/include/sensor.h#L98C5-L98C18
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (psramFound()) {
    config.fb_count = 2;
    config.grab_mode = CAMERA_GRAB_LATEST;
  } else {
    Serial.println("Warnning! PSRAM not found!!!!");
    // Limit the frame size when PSRAM is not available
    config.frame_size = FRAMESIZE_SVGA;
    config.fb_location = CAMERA_FB_IN_DRAM;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  Serial.printf("Camera setup done!..\r\n");
}

void setupMotionDetection() {
  MotionDetector::Config mdConfig = {
    .width = 640,                         // VGA width
    .height = 480,                        // VGA height
    .blockSize = 16,                      // 16x16 pixel blocks
    .blockThreshold = BLOCK_THRESHOLD,    // Sensitivity to changes within blocks
    .motionThreshold = MOTION_THRESHOLD,  // Number of blocks that must change
    .jpegQuality = 12                     // JPEG quality setting
  };


  motionDetector = new MotionDetector(mdConfig);
  if (!motionDetector->init()) {
    Serial.println("Motion detector initialization failed!");
    return;
  }
}

void setup() {
  Serial.begin(115200);

  setupCamera();
  setupMotionDetection();
}

void loop() {
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  if (motionDetector->detectMotion(fb)) {
    if (!motionDetected) {
      Serial.println("Motion detected!");
      motionDetected = true;
    }
  } else {
    motionDetected = false;
  }

  esp_camera_fb_return(fb);
  delay(100);
}