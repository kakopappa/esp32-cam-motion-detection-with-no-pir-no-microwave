# ESP32 Camera Motion Detection With No PIR or Microwave Sensor.

A robust motion detection system using ESP32-CAM to capture. The code divides camera frames into blocks and analyzes pixel changes to detect motion. 

## Features

- Real-time motion detection using block comparison algorithm
- JPEG image capture when motion is detected
- Configurable sensitivity and detection parameters
- Memory-efficient implementation using RGB565 color space
- Support for ESP32-CAM module

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
