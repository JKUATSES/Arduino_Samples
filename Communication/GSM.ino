/*

This code uses tinyGSM arduino library(https://github.com/vshymanskyy/TinyGSM) to initialize a gsm connection and send a text to a targed phone number
The GSM modem used in this example is SIM800. This should be changed to match the modem you are using, if not SIM800

Hardware Requirements: - Arduino or Genuino Board
                       - GSM/GPRS Shield - SIM800
                       - SIM card
Setup:
First, download and install the tinygm arduino library. Canbe found here: https://www.arduinolibraries.info/libraries/tiny-gsm

Written 12 November 2019
by Sackey Freshia
*/

//change this if you are using a different modem
#define TINY_GSM_MODEM_SIM800

#define TINY_GSM_DEBUG SerialMon


// Range to attempt to autobaud
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 38400

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
#define SerialAT Serial1
#define TINY_GSM_USE_GPRS true

#define TINY_GSM_USE_SMS true

//for pin unlock if your sim card is pin enabled
#define GSM_PIN ""

// Set phone number to send SMS to
#define SMS_TARGET  "+254XXXXXXXXX"

const char apn[]  = "YOUR APN";
const char gprs_User[] = "";
const char gprs_Pass[] = "";



#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif

void setup() {
  SerialMon.begin(115200);
  delay(10);

  DBG("Wait...");

  TinyGsmAutoBaud(SerialAT,GSM_AUTOBAUD_MIN,GSM_AUTOBAUD_MAX);
  delay(2500);
}

void loop() {

  initialize_GSM_and_send_SMS();
  delay(1000);
}
void initialize_GSM_and_send_SMS(){
  // Restart takes longer. To skip it, call init() instead of restart()
  DBG("Initializing modem...");
  if (!modem.restart()) {
    DBG("Failed to restart modem, delaying 10s and retrying");
    delay(3000);
    // restart autobaud in case GSM just rebooted
    TinyGsmAutoBaud(SerialAT,GSM_AUTOBAUD_MIN,GSM_AUTOBAUD_MAX);
    delay(10000);
    return;
  }

  String name = modem.getModemName();
  DBG("Modem Name:", name);

  String modemInfo = modem.getModemInfo();
  DBG("Modem Info:", modemInfo);

#if TINY_GSM_USE_GPRS
  // SIM unlock
  if ( GSM_PIN && modem.getSimStatus() != 3 ) {
    modem.simUnlock(GSM_PIN);
  }
#endif

  DBG("Waiting for network...");
  if (!modem.waitForNetwork()) {
    delay(10000);
    return;
  }

  if (modem.isNetworkConnected()) {
    DBG("Network connected");
  }

#if TINY_GSM_USE_GPRS
  DBG("Connecting to", apn);
  if (!modem.gprsConnect(apn,gprs_User,gprs_Pass)) {
    delay(10000);
    return;
  }

  bool res = modem.isGprsConnected();
  DBG("GPRS status:", res ? "connected" : "not connected");

  String cop = modem.getOperator();
  DBG("Operator:", cop);

#endif

#if defined(TINY_GSM_MODEM_HAS_GPS)
  modem.enableGPS();
  String gps_raw = modem.getGPSraw();
  modem.disableGPS();
  DBG("GPS raw data:", gps_raw);
  #endif

#if TINY_GSM_USE_SMS && defined(SMS_TARGET)
  res = send_SMS();

#endif

#if TINY_GSM_USE_GPRS
  modem.gprsDisconnect();
  if (!modem.isGprsConnected()) {
    DBG("GPRS disconnected");
  } else {
    DBG("GPRS disconnect: Failed.");
  }
#endif

  // Do nothing forevermore
  while (true) {
    modem.maintain();
  }
}

int send_SMS(){
    int result = 0;
    result = modem.sendSMS(SMS_TARGET, String("Hello from Sackey the most awesome"));
    DBG("SMS:", result ? "OK" : "fail");
    return result;
}