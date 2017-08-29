# nodemcu-ito-transition
Nodemcu/ESP8266/ESP12 with MQTT, IR transmitter diode, 433Mhz RC Transmit and receive module. This is a project, as a present for my long lost fiend.... Why not to turn on an air condition before he comes home...

I have no doubt that most of devices around us will have some kind of IOT functionality so this is just like.. transitional phase and the device which tends to IOTize existing devices which we have.


Hardware:

1. Nodemcu (With ESP12E/F)
2. Small peace of breadboard
3. Big enough plastic chase
4. IR diode
5. RC transmitter
6. RC receiver
7. DHT22 temperature and humidity sensor
8. Adafruit NeoPixel ring with 12 leds 5050

Software dependencies:
0. ESP8266/Arduino https://github.com/esp8266/Arduino
1. IRremoteESP8266 lib for arduino => https://github.com/markszabo/IRremoteESP8266
2. RCSwitch lib for arduino => https://github.com/sui77/rc-switch
3. Adafruit DHT Humidity & Temperature Unified Sensor Library => https://github.com/adafruit/DHT-sensor-library
4. Adafruit NeoPixel Library => https://github.com/adafruit/Adafruit_NeoPixel
5. WiFiManager => https://github.com/tzapu/WiFiManager
6. ArduinoJson Library => https://github.com/bblanchon/ArduinoJson
....ESP wifi/pubsub Libraries...etc...


You also need mqtt server, i prefer using mosquitto or some public, free servers

You can control IR devices like TV, Air condition etc...
Also, you can buy some of RC ready sockets and nstantly start controlling some of the devices in your home. (keep in mind that such sockets have max current limitations lower than regular sockets, don't try to power up heating etc...)

### Some Images:
<img src="https://raw.githubusercontent.com/nardev/nodemcu-ito-transition/master/images/hepek-for-edin-out.jpg" width="264"  align="left">&nbsp;&nbsp;
<img src="https://raw.githubusercontent.com/nardev/nodemcu-ito-transition/master/images/hepek-for-edin-in.jpg" width="222"  align="left">&nbsp;&nbsp;
<img src="https://raw.githubusercontent.com/nardev/nodemcu-ito-transition/master/images/hepek-for-edin-light.jpg" width="305"  align="left">&nbsp;&nbsp;
