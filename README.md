# IsolationForestTinyML
Master's thesis project


## Setup

Basically all that is needed is to install the required packages and libraries that are needed to compile for the Arduino Nano BLE sense 33.

However there will be a compile error regarding the defenition of the absolute value function. To fix this, simply go to the file that the output box says has the error, and comment out the defenition. The function is not used at all so there's no problem with this.

The error and file path should look something like this:
```
/users/your_user/Library/Arduino15/packages/arduino/hardware/mbed_nano/2.6.1/cores/arduino/Arduino.h
error: expected unqualified-id before '(' token
```
If you don't find the file then check hidden files and folders. This has been an issue on Mac OS, however not on Windows OS.
