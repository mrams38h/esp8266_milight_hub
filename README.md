# esp8266_milight_hub [![Build Status](https://travis-ci.org/sidoh/esp8266_milight_hub.svg?branch=master)](https://travis-ci.org/sidoh/esp8266_milight_hub) [![License][shield-license]][info-license]


### IN PROGRESS - DON'T USE YET

## Changes in this fork

This fork extends the original projekt. 
It's desigend to be used with a special built hardware, that contains 2 buttons and 8 leds, a Lolin Node MCU V3, an MCP23017 portexpander and the 2.4GHz module NRF24L01+PA. 
The target is to create a stand alone controller, that can control 8 groups and can store 6 scenes (limited by EEPROM), that will be automatically switched per time, pulled from an NTP server.
I also created a desktop program and an excel sheet&macro to take control from a pc and to program the scenes. 
The lib folder "MaxSeins" contains only code, that i wrote. 
All changes in the original code are written between ifdef statements.

Transitions and MQTT support are removed because not used.
Also only FUT89 is used (meaningless what you select in webinterface) because there was a bug, that UDP commands could only trigger FUT92 messages. 
It's also now possible to deactivate the Webinterface with a button in my PC program because of safty resons. 
This state is stored permanently in EEPROM an can only be changed with te PC program. 

## ORIGINAL

This is a replacement for a Milight/LimitlessLED remote/gateway hosted on an ESP8266. Leverages [Henryk Plötz's awesome reverse-engineering work](https://hackaday.io/project/5888-reverse-engineering-the-milight-on-air-protocol).

[Milight bulbs](https://www.amazon.com/Mi-light-Dimmable-RGBWW-Spotlight-Smart/dp/B01LPRQ4BK/r) are cheap smart bulbs that are controllable with an undocumented 2.4 GHz protocol. In order to control them, you either need a [remote](https://www.amazon.com/Mi-light-Dimmable-RGBWW-Spotlight-Smart/dp/B01LCSALV6/r?th=1) ($13), which allows you to control them directly, or a [WiFi gateway](http://futlight.com/productlist.aspx?typeid=125) ($30), which allows you to control them with a mobile app or a [UDP protocol](https://github.com/Fantasmos/LimitlessLED-DevAPI).

This project is a replacement for the wifi gateway.

[This guide](http://blog.christophermullins.com/2017/02/11/milight-wifi-gateway-emulator-on-an-esp8266/) on my blog details setting one of these up.

## Why this is useful

1. Both the remote and the WiFi gateway are limited to four groups. This means if you want to control more than four groups of bulbs, you need another remote or another gateway. This project allows you to control 262,144 groups (4*2^16, the limit imposed by the protocol).
2. This project exposes a nice REST API to control your bulbs.
3. You can secure the ESP8266 with a username/password, which is more than you can say for the Milight gateway! (The 2.4 GHz protocol is still totally insecure, so this doesn't accomplish much :).
4. Official hubs connect to remote servers to enable WAN access, and this behavior is not disableable.
5. This project is capable of passively listening for Milight packets sent from other devices (like remotes). It can publish data from intercepted packets to MQTT. This could, for example, allow the use of Milight remotes while keeping your home automation platform's state in sync. See the MQTT section for more detail.

## Supported remotes

The following remotes can be emulated:

Support has been added for the following [bulb types](http://futlight.com/productlist.aspx?typeid=101):

Model #|Name|Compatible Bulbs
-------|-----------|----------------
|FUT096|RGB/W|<ol><li>FUT014</li><li>FUT016</li><li>FUT103</li>|
|FUT005<br/>FUT006<br/>FUT007</li></ol>|CCT|<ol><li>FUT011</li><li>FUT017</li><li>FUT019</li></ol>|
|FUT098|RGB|Most RGB LED Strip Controlers|
|FUT020|RGB|Some other RGB LED strip controllers|
|FUT092|RGB/CCT|<ol><li>FUT012</li><li>FUT013</li><li>FUT014</li><li>FUT015</li><li>FUT103</li><li>FUT104</li><li>FUT105</li><li>Many RGB/CCT LED Strip Controllers</li></ol>|
|FUT091|CCT v2|Most newer dual white bulbs and controllers|
|FUT089|8-zone RGB/CCT|Most newer rgb + dual white bulbs and controllers|

Other remotes or bulbs, but have not been tested.

## What you'll need

1. An ESP8266. I used a NodeMCU.
2. A NRF24L01+ module (~$3 on ebay). Alternatively, you can use a LT8900.
3. Some way to connect the two (7 female/female dupont cables is probably easiest).

## Installing

#### Connect the NRF24L01+ / LT8900

This project is compatible with both NRF24L01 and LT8900 radios. LT8900 is the same model used in the official MiLight devices. NRF24s are a very common 2.4 GHz radio device, but require software emulation of the LT8900's packet structure. As such, the LT8900 is more performant.

Both modules are SPI devices and should be connected to the standard SPI pins on the ESP8266.

##### NRF24L01+


[This guide](https://www.mysensors.org/build/connect_radio#nrf24l01+-&-esp8266) details how to connect an NRF24 to an ESP8266. By default GPIO 4 for CE and GPIO 15 for CSN are used, but these can be configured late in the Web GUI under Settings -> Setup.

<img src="https://user-images.githubusercontent.com/40266/47967518-67556f00-e05e-11e8-857d-1173a9da955c.png" align="left" width="32%" />
<img src="https://user-images.githubusercontent.com/40266/47967520-691f3280-e05e-11e8-838a-83706df2edb0.png" align="left" width="22%" />

NodeMCU | Radio | Color
-- | -- | --
GND | GND | Black
3V3 | VCC | Red
D2 (GPIO4) | CE | Orange
D8 (GPIO15) | CSN/CS | Yellow
D5 (GPIO14) | SCK | Green
D7 (GPIO13) | MOSI | Blue
D6 (GPIO12) | MISO | Violet

_Image source: [MySensors.org](https://mysensors.org)_


##### LT8900

Connect SPI pins (CE, SCK, MOSI, MISO) to appropriate SPI pins on the ESP8266. With default settings, connect RST to GPIO 0, PKT to GPIO 16, CE to GPIO 4, and CSN to GPIO 15.  Make sure to properly configure these if using non-default pinouts.

#### Setting up the ESP

The goal here is to flash your ESP with the firmware. It's really easy to do this with [PlatformIO](http://platformio.org/):

```
export ESP_BOARD=nodemcuv2
platformio run -e $ESP_BOARD --target upload
```

Of course make sure to substitute `nodemcuv2` with the board that you're using.

You can find pre-compiled firmware images on the [releases](https://github.com/sidoh/esp8266_milight_hub/releases).

#### Configure WiFi

This project uses [WiFiManager](https://github.com/tzapu/WiFiManager) to avoid the need to hardcode AP credentials in the firmware.

When the ESP powers on, you should be able to see a network named "ESPXXXXX", with XXXXX being an identifier for your ESP. Connect to this AP and a window should pop up prompting you to enter WiFi credentials.  If your board has a built-in LED (or you wire up an LED), it will [flash to indicate the status](#led-status).

The network password is "**milightHub**".

#### Get IP Address

Both mDNS and SSDP are supported.

* OS X - you should be able to navigate to http://milight-hub.local.
* Windows - you should see a device called "ESP8266 MiLight Gateway" show up in your network explorer.
* Linux users can install [avahi](http://www.avahi.org/) (`sudo apt-get install avahi-daemon` on Ubuntu), and should then be able to navigate to http://milight-hub.local.

#### Use it!

The HTTP endpoints (shown below) will be fully functional at this point. You should also be able to navigate to `http://<ip_of_esp>`, or `http://milight-hub.local` if your client supports mDNS. The UI should look like this:

![Web UI](https://user-images.githubusercontent.com/589893/61682228-a8151700-acc5-11e9-8b86-1e21efa6cdbe.png)


If it does not work as expected see [Troubleshooting](https://github.com/sidoh/esp8266_milight_hub/wiki/Troubleshooting).

#### Pair Bulbs

If you need to pair some bulbs, how to do this is [described in the wiki](https://github.com/sidoh/esp8266_milight_hub/wiki/Pairing-new-bulbs).

## Device Aliases

You can configure aliases or labels for a given _(Device Type, Device ID, Group ID)_ tuple.  For example, you might want to call the RGB+CCT remote with the ID `0x1111` and the Group ID `1` to be called `living_room`.  Aliases are useful in a couple of different ways:

* **In the UI**: the aliases dropdown shows all previously set aliases.  When one is selected, the corresponding Device ID, Device Type, and Group ID are selected.  This allows you to not need to memorize the ID parameters for each lighting device if you're controlling them through the UI.
* **In the REST API**: standard CRUD verbs (`GET`, `PUT`, and `DELETE`) allow you to interact with aliases via the `/gateways/:device_alias` route.
* **MQTT**: you can configure topics to listen for commands and publish updates/state using aliases rather than IDs.

## REST API

The REST API is specified using the [OpenAPI v3](https://swagger.io/docs/specification/about/) specification.

[openapi.yaml](docs/openapi.yaml) contains the raw spec.

[You can view generated documentation for the master branch here.](https://sidoh.github.io/esp8266_milight_hub/branches/latest)

[Docs for other branches can be found here](https://sidoh.github.io/esp8266_milight_hub)



## UDP Gateways

You can add an arbitrary number of UDP gateways through the REST API or through the web UI. Each gateway server listens on a port and responds to the standard set of commands supported by the Milight protocol. This should allow you to use one of these with standard Milight integrations (SmartThings, Home Assistant, OpenHAB, etc.).

You can select between versions 5 and 6 of the UDP protocol (documented [here](https://github.com/BKrajancic/LimitlessLED-DevAPI/)). Version 6 has support for the newer RGB+CCT bulbs and also includes response packets, which can theoretically improve reliability. Version 5 has much smaller packets and is probably lower latency.


#### RESTful `/transitions` routes

These routes are fully documented in the [REST API documentation](https://sidoh.github.io/esp8266_milight_hub/branches/latest/#tag/Transitions).


## LED Status

Some ESP boards have a built-in LED, on pin #2.  This LED will flash to indicate the current status of the hub:

* Wifi not configured: Fast flash (on/off once per second).  See [Configure Wifi](#configure-wifi) to configure the hub.
* Wifi connected and ready: Occasional blips of light (a flicker of light every 1.5 seconds).
* Packets sending/receiving: Rapid blips of light for brief periods (three rapid flashes).
* Wifi failed to configure: Solid light.

In the setup UI, you can turn on "enable_solid_led" to change the LED behavior to:

* Wifi connected and ready: Solid LED light
* Wifi failed to configure: Light off

Note that you must restart the hub to affect the change in "enable_solid_led".

You can configure the LED pin from the web console.  Note that pin means the GPIO number, not the D number ... for example, D1 is actually GPIO5 and therefore its pin 5.  If you specify the pin as a negative number, it will invert the LED signal (the built-in LED on pin 2 (D4) is inverted, so the default is -2).

If you want to wire up your own LED you can connect it to D1/GPIO5. Put a wire from D1 to one side of a 220 ohm resistor. On the other side, connect it to the positive side (the longer wire) of a 3.3V LED.  Then connect the negative side of the LED (the shorter wire) to ground.  If you use a different voltage LED, or a high current LED, you will need to add a driver circuit.

Another option is to use an external LED parallel to the (inverted) internal one, this way it will mirror the internal LED without configuring a new LED pin in the UI. To do this connect the (short) GND pin of your LED to D4. The longer one to a 220 ohm resistor and finally the other side of the resistor to a 3V3 pin.

## Development

This project is developed and built using [PlatformIO](https://platformio.org/).

#### Running tests

On-board unit tests are available using PlatformIO.  Run unit tests with this command:

```
pio test -e d1_mini
```

substituting `d1_mini` for the environment of your choice.

#### Running integration tests

A remote integration test suite built using rspec is available under [`./test/remote`](test/remote).

## Ready-Made Hub

h4nc (h4nc.zigbee(a)gmail.com) created a PCB and 3D-printable case for espMH.  He's offering ready-made versions.  Please get in touch with him at the aforementioned email address for further information.

Find more information from the [espmh_pcb](https://github.com/sidoh/espmh_pcb) repository.

## Acknowledgements

* @WoodsterDK added support for LT8900 radios.
* @cmidgley contributed many substantial features to the 1.7 release.

[info-license]:   https://github.com/sidoh/esp8266_milight_hub/blob/master/LICENSE
[shield-license]: https://img.shields.io/badge/license-MIT-blue.svg

## Donating

If the project brings you happiness or utility, it's more than enough for me to hear those words.

If you're feeling especially generous, and are open to a charitable donation, that'd make me very happy.  Here are some whose mission I support (in no particular order):

* [Water.org](https://www.water.org)
* [Brain & Behavior Research Foundation](https://www.bbrfoundation.org/)
* [Electronic Frontier Foundation](https://www.eff.org/)
* [Girls Who Code](https://girlswhocode.com/)
* [San Francisco Animal Care & Control](http://www.sfanimalcare.org/make-a-donation/)
