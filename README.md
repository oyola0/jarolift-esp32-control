# jarolift-esp32-control

## Uploading Filesystem Image
1. Click the PIO icon at the left side bar. The project tasks should open.
2. Select env:esp32doit-devkit-v1 (it may be slightly different depending on the board you’re using).
3. Expand the Platform menu.
4. Select Build Filesystem Image.
5. Finally, click Upload Filesystem Image.

![Uploading Filesystem Image](https://i2.wp.com/randomnerdtutorials.com/wp-content/uploads/2020/12/Upload-filesystem-image-ESP32-VS-Code-PlatformIO-SPIFFS.jpg)

### Path variable

With path variable you can create a custom regex rule for a specific parameter in a route. 
For example we want a `sensorId` parameter in a route rule to match only a integer.

```cpp
    server.on("^\\/api\\/middle\\/channel\\/([0-9])$", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String channel = request->pathArg(0); 
        addRequest("middle", channel);
        request->send(200, "text/plain", "{}");
    });
```
*NOTE*: All regex patterns starts with `^` and ends with `$`

To enable the `Path variable` support, you have to define the buildflag `-DASYNCWEBSERVER_REGEX`.


For Arduino IDE create/update `platform.local.txt`:

`Windows`: C:\Users\(username)\AppData\Local\Arduino15\packages\\`{espxxxx}`\hardware\\`espxxxx`\\`{version}`\platform.local.txt

`Linux`: ~/.arduino15/packages/`{espxxxx}`/hardware/`{espxxxx}`/`{version}`/platform.local.txt

Add/Update the following line:
```
  compiler.cpp.extra_flags=-DASYNCWEBSERVER_REGEX
```

For platformio modify `platformio.ini`:
```ini
[env:myboard]
build_flags = 
  -DASYNCWEBSERVER_REGEX
```
*NOTE*: By enabling `ASYNCWEBSERVER_REGEX`, `<regex>` will be included. This will add an 100k to your binary.