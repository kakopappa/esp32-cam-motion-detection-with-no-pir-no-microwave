# ESP32 Camera Motion Detection With No PIR or Microwave Sensor.

A robust motion detection system using ESP32-CAM that captures the motion when block changes are detected. This project uses efficient memory management and RGB565 color space for optimal performance on ESP32 hardware.

## Features

- Real-time motion detection using block comparison algorithm
- JPEG image capture when motion is detected
- Configurable sensitivity and detection parameters
- Memory-efficient implementation using RGB565 color space
- Support for ESP32-CAM module
 
## Hardware Requirements

- ESP32-CAM module (AI-Thinker ESP32-CAM recommended)
- USB-to-TTL converter for programming (if your board doesn't have USB)
- Power supply (5V)
- (Optional) SD card for storing more images

## Configuration

The motion detector can be configured using the following parameters in the `Config` struct:

```cpp
MotionDetector::Config config = {
    .width = 320,           // Camera resolution width
    .height = 240,          // Camera resolution height
    .blockSize = 16,        // Size of comparison blocks
    .blockThreshold = 30,   // Sensitivity threshold for blocks
    .motionThreshold = 10,  // Number of blocks that must change
    .jpegQuality = 12      // JPEG quality (0-63, lower is better)
};
``` 