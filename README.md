# ESP32 Camera Motion Detection With No PIR or Microwave Sensor.

The code divides camera frames into blocks and analyzes pixel changes to detect motion. 

```cpp
// More sensitive to large moving objects
#define BLOCK_THRESHOLD 40    // Require stronger pixel changes
#define MOTION_THRESHOLD 15   // Require more blocks to change

// More sensitive to mid-moving objects
#define BLOCK_THRESHOLD 30   // Accept subtler pixel changes
#define MOTION_THRESHOLD 10  // Accept mid changed blocks

// More sensitive to small moving objects
#define BLOCK_THRESHOLD 20    // Accept subtler pixel changes
#define MOTION_THRESHOLD 5    // Accept fewer changed blocks
```

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
