# ESP32 Alexa jarolift-control

[Jarolift remote control](https://www.jarolift.de/files/jarolift/manuals/handsender/en/Jarolift_TDRC_08_16_EN.pdf) is ready to drive 2 motors for blinds. My intention is to manage (alexa and browser) and centralize all the control of electric blinds.

The service should be able to manage [jarolift electric shutters](https://www.jarolift.de/rollladenmotoren.html) from any browser within the same network.

## 1. Set your Credentials

Add your wifi and [Sinric Pro](https://sinric.pro/es-index.html) credentials modifying [include/config.h](include/config.h) file 

## 2. Uploading Filesystem Image

1. Click the PIO icon at the left side bar. The project tasks should open.
2. Select env:esp32doit-devkit-v1 (it may be slightly different depending on the board you’re using).
3. Expand the Platform menu.
4. Select Build Filesystem Image.
5. Finally, click Upload Filesystem Image.

![Uploading Filesystem Image](https://i2.wp.com/randomnerdtutorials.com/wp-content/uploads/2020/12/Upload-filesystem-image-ESP32-VS-Code-PlatformIO-SPIFFS.jpg)

## 3. Build and upload 

The service is exposed on [http://192.168.1.200](http://192.168.1.200)