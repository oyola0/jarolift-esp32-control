# jarolift-esp32-control


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