/* P1P2_Config.h
 *
 * Copyright (c) 2019-2022 Arnold Niessen, arnold.niessen-at-gmail-dot-com - licensed under CC BY-NC-ND 4.0 with exceptions (see LICENSE.md)
 *
 * WARNING: P1P2-bridge-esp8266 is end-of-life, and will be replaced by P1P2MQTT
 *
 * Version history
 * 20220821 v0.9.17-fix1 corrected negative deviation temperature reporting, more temp settings reported
 * 20220817 v0.9.17 config file using COMBIBOARD, errors and scopemode-time info via P1P2/R/#
 * 20220808 v0.9.15 extended verbosity command, unique OTA hostname, minor fixes
 * 20220802 v0.9.14 AVRISP, wifimanager, mqtt settings, EEPROM, telnet, outputMode, outputFilter, ...
 * ..
 *
 */

#ifndef P1P2_Config
#define P1P2_Config

#define COMBIBOARD // define this for Arduino/ESP8266 combi-board and for P1P2-ESP-Interface, undefine for ESP-on-USB-debug

#ifdef COMBIBOARD
#define SERIALSPEED 250000
#define SERIAL_MAGICSTRING "1P2P" // Serial input line should start with SERIAL_MAGICSTRING, otherwise input line is ignored
#else
#define SERIALSPEED 115200
// do not #define SERIAL_MAGICSTRING
#endif

#define SAVEPARAMS
#define SAVEPACKETS
// to save memory to avoid ESP instability: don't #define SAVESCHEDULE // format of schedules will change to JSON format in P1P2MQTT

#define WELCOMESTRING "* [ESP] P1P2-bridge-esp8266 v0.9.17-fix1"
#define WELCOMESTRING_TELNET "P1P2-bridge-esp8266 v0.9.17-fix1"

#define AVRISP // enables flashing ATmega by ESP

//#define DEBUG_OVER_SERIAL // sends lots of information over serial. Uncomment this to avoid P1P2Monitor receiving this information. 
                          // Use only for serial over USB (or serial via 2nd ESP01) debugging

#define TELNET // define to allow telnet access (no password protection!) for monitoring and control
               // the telnet library provides no authentication for telnet
               // Unfortunately telnet output is synchronized, which may trigger some issues only when telnet is being used
               // undefine on open networks or if you experience problems

#define PSEUDO_PACKETS // define to have P1P2-bridge-esp8266 output additional P1P2-like packets with internal state information

// home assistent discovery
#define HA_PREFIX "homeassistant/sensor"      // homeassistant MQTT discovery prefix
#define HA_DEVICE_NAME "P1P2"                 // becomes device name in HA
#define HA_DEVICE_ID "P1P2ID12"               // uniq device_id. Currently all sensors in one device_ID
#define HA_DEVICE_MODEL "P1P2_ESP_Interface"  // shows up as Device Info in HA
#define HA_MF "NPC"
#define HA_SW "0.9.15"
#define HA_KEY_LEN 100
#define HA_VALUE_LEN 300
#define HA_POSTFIX "_12"

// MQTT topics
#define MQTT_KEY_PREFIXIP   7
char mqttHexdata[11]     = "P1P2/R/xxx";
char mqttBindata[11]     = "P1P2/X/xxx";
char mqttSignal[11]      = "P1P2/S/xxx";
char mqttCommands[11]    = "P1P2/W/xxx";
char mqttCommandsNoIP[7] = "P1P2/W";
char mqttJsondata[11]    = "P1P2/J/xxx";
char mqttKeyPrefix[16]   = "P1P2/P/xxx/M/0/";
#define MQTT_KEY_PREFIXCAT 11 // location in prefix for category identification
#define MQTT_KEY_PREFIXSRC 13 // location in prefix for source identification
#define MQTT_KEY_PREFIXLEN 15 // length of mqttKeyPrefix (excl '\0')

// To avoid Mqtt/CPU overload and message loss, throttle mqt traffic after reboot
#define THROTTLE_VALUE 5 // start with 20% parameter coverage, increasing in (THROTTLE_VALUE-1)*(THROTTLE_STEP) seconds to 100%
#define THROTTLE_STEP 4  // in seconds

#define MQTT_PUBLISH_NONHA 1 // 1 publishes all parameters (subject to outputFilter and outputMode)
                             // 0 (ugly, not entirely correct, implementation:) publishes only approximately subset of parameters which are defined to be discoverable by Home Assistant

#define MQTT_DISCONNECT_CONTINUE 0 // 0 pauses processing packets if mqtt is disconnected (to avoid that changes are lost)
                                   // Set to 1 to continue (in case you have no mqtt of want to see changes via telnet or so)
#define MQTT_DISCONNECT_RESTART 150 // Restart ESP if Mqtt disconnect time larger than this value in seconds (because after WiFi interruption, Mqtt may not reconnect reliably)

#define MQTT_RETAIN true // all messages (ha and parameter values) retained
//#define MQTT_RETAIN false // no messages (ha and parameter values) retained

#define INIT_OUTPUTFILTER 1    // outputfilter determines which parameters to report, can be changed run-time using 'S'/'s' command
                               // 0 all
                               // 1 only changed parameters
                               // 2 only changed parameters except measurements (temperature, flow)
                               // 3 only changed parameters except measurements (temperature, flow) and except date/time
                               // first-time parameters are always reported
#define INIT_OUTPUTMODE 0x3803 // outputmode determines output content and channels, can be changed run-time using 'J'/'j' command
                               // outputmode is sum of:
                               // 0x0001 to output raw packet data (including pseudo-packets) over mqtt P1P2/R/xxx
                               // 0x0002 to output mqtt individual parameter data over mqtt P1P2/P/xxx/#
                               // 0x0004 to output json data over mqtt P1P2/J/xxx
                               // 0x0008 to have mqtt/json include parameters even if functionality is unknown, warning: easily overloads ATmega/ESP (best to combine this with outputfilter >=1)
                               // 0x0010 ESP to output raw data over telnet
                               // 0x0020 to output mqtt individual parameter data over telnet
                               // 0x0040 to output json data over telnet
                               // 0x0080 (reserved)
                               // 0x0100 ESP to output raw data over serial
                               // 0x0200 to output mqtt individual parameter data over serial
                               // 0x0400 to output json data over serial
                               // 0x0800 to output raw bin data over P1P2/X/xxx
                               // 0x1000 to output timing data also over P1P2/R/xxx (prefix: C)
                               // 0x2000 to output error data also over P1P2/R/xxx (prefix: *)

// no need to change these:
#define RESET_PIN 5 // GPIO_5 on ESP-12F pin 20 connected to ATmega328P's reset line
#define RX_BUFFER_SIZE 2048 // to avoid serial buffer overruns (512 is too small)
#define MQTT_MIN_FREE_MEMORY 6000 // Must likely be more than 4kB, MQTT messages will not be transmitted if available memory is below this value
#define MQTT_QOS 0 // QOS = 1 is too slow
#define SERIAL_MAGICSTRING "1P2P" // Serial input of ATmega should start with SERIAL_MAGICSTRING, otherwise lines line is ignored by P1P2Monitor
#define CRC_GEN 0xD9    // Default generator/Feed for CRC check; these values work at least for the Daikin hybrid
#define CRC_FEED 0x00   // Define CRC_GEN to 0x00 means no CRC is checked when reading or added when writing
#define SPRINT_VALUE_LEN 400 // max message length for informational and debugging output over P1P2/S, telnet, or serial
#define MQTT_KEY_LEN 100
#ifdef SAVESCHEDULE
#define MQTT_VALUE_LEN 630 // max length: Program 6* (6+8)*6*7+3*7+2 = 611
#else
#define MQTT_VALUE_LEN 400
#endif

#endif /* P1P2_Config */
