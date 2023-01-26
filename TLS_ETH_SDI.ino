#include <BMDSDIControl.h>
#include <EEPROM.h>
#include <Ethernet.h>  //ETH_DEBUG
#include <MemoryFree.h>
#include <SPI.h>

// developed from TLS_ETH_hybrid_RLS02_22.10.22

#define DEBUG
#define EEPROMe
#define DHCP
#define ATEM_enable

#define VERSION "TLS_ETH_SDI_26.01.23_VER02"

#define MAX_CHAN_NUM 8
#define TIMEOUT      2000  // connecting to switcher
// #define KEEPALIVE     2000               // comment to disable keepalive
#define TTK           1000               // time to kill - cas do ktoreho ak strizna neodpovie, restartuje sa spojenie
#define RESEND        100                // wireless hc12 resend time - default 100
#define DEFAULT_VALUE 0b001001001001001  // 4681
#define ACTIVE        3                  // 3
#define PREVIEW       2                  // 2
#define NOTHING       1                  // 1
#define CALL          4                  // 4

byte mac[] = {0x02, 0x54, 0x4C, 0x53, 0x00, 0x04};
uint8_t TLS_IPaddr[4] = {192, 168, 1, 118};

struct Switcher {
  uint8_t pos;                            // eeprom position
  uint8_t IPaddr[4] = {192, 168, 0, 10};  // mixer IP address
  char enable[8];                         // enable string
  const char *p_save;
  uint8_t status;                 // connected or disconnected
  uint8_t initialized;            // runned
  uint16_t code = DEFAULT_VALUE;  // for wireless
  uint8_t inputNumber[MAX_CHAN_NUM] = {0};
  uint8_t keepAliveFlag = 0;
  unsigned long cMillisKeepAlive;             // keepAlive
  uint8_t tallyValue[MAX_CHAN_NUM][2] = {0};  // for ATEM [channel][program=0 | preview=1]
};

// BMD_SDITallyControl_I2C sdiTallyControl(0x6E);
BMD_SDITallyControl_Serial sdiTallyControl;

EthernetClient vMixClient_1;
EthernetClient vMixClient_2;

EthernetServer server(80);

Switcher vMix_1;
Switcher vMix_2;
Switcher ATEM;

#include "website.h"

const char connected[] = "<p id=\"connected\">CONNECTED</p>";
const char disconnected[] = "<p id=\"disconnected\">DISCONNECTED</p>";
const char save[] = "<button class=\"button\" id=\"save\" name=\"save\" type=\"submit\">Save</button>";
const char *p_save;
unsigned long cMillisTimeout;  // timeout
unsigned long cMillisResend;   // resend
char message[300];

uint8_t flag = 0;

void setup() {
  Serial2.begin(115200);
  Serial1.begin(9600);
  Serial2.printf("\n\n\rversion: %s\n", VERSION);

  vMix_1.pos = 0;  // EEPROM position
  vMix_2.pos = 4 + MAX_CHAN_NUM + 1;
  ATEM.pos = 2 * (4 + MAX_CHAN_NUM + 1);
  Ethernet.init(10);
#ifdef ATEM_enable
  sdiTallyControl.begin();
  sdiTallyControl.setOverride(true);
#else
  Serial2.print("BMD_Shield disabled by program\r\n");
#endif

#ifdef EEPROMe
  eepromRead(vMix_1);
  eepromRead(vMix_2);
  eepromRead_ATEM();
#endif

#ifdef DHCP
  if (Ethernet.begin(mac) == 0) {
    Serial2.println("Using static IP address");
    Ethernet.begin(mac, TLS_IPaddr);
  } else {
    Serial2.println("Using DHCP IP address");
  }
#else
  Serial2.println("Using static IP address");
  Ethernet.begin(mac, TLS_IPaddr);
#endif

  server.begin();
#ifdef DEBUG
  Serial2.print("server BEGIN: ");
  Serial2.println(Ethernet.localIP());
#endif
  Serial2.print("freeMemory()=");
  Serial2.println(freeMemory());
}

void loop() {
  configClientConnect();

  vMixHandle(vMixClient_1, vMix_1);
  vMixHandle(vMixClient_2, vMix_2);
#ifdef ATEM_enable
  ATEM_handle();
#endif

  if (millis() > cMillisResend + RESEND) {
    sendCodeWireless();
    cMillisResend = millis();
  }
}

/*server*/
void configClientConnect() {
  char *p_message;
  p_message = message;
  EthernetClient conf = server.available();
  if (conf) {
    Serial2.printf("New HTTP client on socket: %d\n", conf.getSocketNumber());
    memset(message, '\0', sizeof(message));
    while (conf.connected()) {
      if (conf.available()) {
        char c = conf.read();
#ifdef DEBUG
        Serial2.write(c);
#endif
        *p_message = c;
        if ((message + sizeof(message)) > p_message) {
          p_message++;
        } else {
          Serial2.println("\n\rFATAL ERROR:pointer out of string");
          Serial2.print("start:");
          Serial2.println((int)&message);
          Serial2.print("current:");
          Serial2.println((int)p_message);
          break;
        }

        if (c == '\n') {
          if (strstr(message, "HTTP/1.1") != NULL) {
            processConfData("IPaddr1=", "en1", vMix_1);
            processConfData("IPaddr2=", "en2", vMix_2);
            processConfDataATEM();

            vMixHandle(vMixClient_1, vMix_1);
            vMixHandle(vMixClient_2, vMix_2);
#ifdef ATEM_enable
            setBMD_SDI_OUT();
#endif
            if (vMix_1.status) {
              if (vMix_2.status) {
                website(conf, vMix_1, vMix_2, ATEM, connected, connected, p_save, VERSION);
              } else {
                website(conf, vMix_1, vMix_2, ATEM, connected, disconnected, p_save, VERSION);
              }
            } else {
              if (vMix_2.status) {
                website(conf, vMix_1, vMix_2, ATEM, disconnected, connected, p_save, VERSION);
              } else {
                website(conf, vMix_1, vMix_2, ATEM, disconnected, disconnected, p_save, VERSION);
              }
            }
            p_message = NULL;
            break;
          }
        }
      }
    }
    delay(1);
    Serial2.printf("\nDisconnecting HTTP client, on socket:%d\n", conf.getSocketNumber());
    conf.stop();

    vMixClient_1.print("TALLY\r\n");
    vMixClient_2.print("TALLY\r\n");
    /*Serial.printf("0, 10<-SUBSCRIBE TALLY\n");
    vMixClient.print("SUBSCRIBE TALLY\r\n");*/
    vMix_1.cMillisKeepAlive = millis();
    vMix_2.cMillisKeepAlive = millis();
  }
}

void processConfData(char *ipAddr, char *enable, Switcher &sw) {
  char *p_data = strstr(message, ipAddr);
  if (p_data != NULL) {
    char *p_dataEN = strstr(message, enable);  // check if switcher is enable
    if (p_dataEN != NULL) {
      strcpy(sw.enable, "checked");
#ifdef EEPROMe
      EEPROM.write(sw.pos + 4 + MAX_CHAN_NUM, 1);
#endif
      p_dataEN = NULL;
    } else {
      memset(sw.enable, '\0', sizeof(sw.enable));
#ifdef EEPROMe
      EEPROM.write(sw.pos + 4 + MAX_CHAN_NUM, 0);
#endif
      p_dataEN = NULL;
    }

    p_data += 8;  // move to first data ("IPaddr1=")
    // Serial.print((*p_data) - 48);
    Serial2.printf("\nSwitcher %d IPadd:", sw.pos);
    for (uint8_t q = 0; q < 4; q++) {
      uint8_t count = 0;
      while (*p_data != '.' && *p_data != '&') {
        int digit = (*p_data) - 48;
        if (count != 0) {
          sw.IPaddr[q] *= 10;
          sw.IPaddr[q] += digit;
        } else {
          sw.IPaddr[q] = digit;
        }
        count++;
        p_data++;
      }
      p_data++;
#ifdef DEBUG
      Serial2.print(sw.IPaddr[q]);
      Serial2.print(".");
#endif
    }

    p_data += 4;  // move to inputsNumbers ("S11=")
    for (uint8_t q = 0; q < MAX_CHAN_NUM; q++) {
      uint8_t count = 0;
      while (*p_data != '&' && *p_data != ' ') {
        // Serial.print((*p_data) - 48);
        uint8_t digit = (*p_data) - 48;
        if (count != 0) {
          sw.inputNumber[q] *= 10;
          sw.inputNumber[q] += digit;
        } else {
          sw.inputNumber[q] = digit;
        }
        count++;
        p_data++;
      }
      p_data += 5;
    }

#ifdef EEPROMe
    for (uint8_t i = 0; i < 4; i++) {
      EEPROM.write(i + sw.pos, sw.IPaddr[i]);
    }

    if (strstr(message, "save")) {
      for (uint8_t i = 0; i < MAX_CHAN_NUM; i++) {
        EEPROM.write(sw.pos + 4 + i, sw.inputNumber[i]);
        // Serial.println("saving input numbers");
      }
      sw.p_save = NULL;
    } else {
      checkInputNumberProfile(sw);
    }

    if (vMix_1.p_save != NULL || vMix_2.p_save != NULL || ATEM.p_save != NULL) {  // check if at least one swithchers input profile has changed
      p_save = save;
    } else {
      p_save = NULL;
    }
#endif

#ifdef DEBUG
    Serial2.printf("\nSwitcher %d inputNmbr:", sw.pos);
    for (int i = 0; i < MAX_CHAN_NUM; i++) {
      Serial2.print(sw.inputNumber[i]);
      Serial2.print("-");
    }
    Serial2.printf("\nSwitcher %d enable: %s", sw.pos, sw.enable);
    Serial2.printf("\nSwitcher %d inputSave: %s", sw.pos, sw.p_save);
#endif
  }
}

void processConfDataATEM() {
  char *p_data = strstr(message, "ATEM");
  if (p_data != NULL) {                       // check if it contains any data
    char *p_dataEN = strstr(message, "en3");  // check if switcher is enable
    if (p_dataEN != NULL) {
      strcpy(ATEM.enable, "checked");
#ifdef EEPROMe
      EEPROM.write(ATEM.pos + 4 + MAX_CHAN_NUM, 1);
#endif
      p_dataEN = NULL;
    } else {
      memset(ATEM.enable, '\0', sizeof(ATEM.enable));
#ifdef EEPROMe
      EEPROM.write(ATEM.pos + 4 + MAX_CHAN_NUM, 0);
#endif
      p_dataEN = NULL;
    }

    p_data += 9;  // move to inputsNumbers ("ATEM&S31=")
    for (uint8_t q = 0; q < MAX_CHAN_NUM; q++) {
      uint8_t count = 0;
      while (*p_data != '&' && *p_data != ' ') {
        // Serial.print((*p_data) - 48);
        uint8_t digit = (*p_data) - 48;
        if (count != 0) {
          ATEM.inputNumber[q] *= 10;
          ATEM.inputNumber[q] += digit;
        } else {
          ATEM.inputNumber[q] = digit;
        }
        count++;
        p_data++;
      }
      p_data += 5;
    }

#ifdef EEPROMe
    if (strstr(message, "save")) {
      for (uint8_t i = 0; i < MAX_CHAN_NUM; i++) {
        EEPROM.write(ATEM.pos + 4 + i, ATEM.inputNumber[i]);
        // Serial.println("saving input numbers");
      }
      ATEM.p_save = NULL;
    } else {
      checkInputNumberProfile(ATEM);
    }

    if (vMix_1.p_save != NULL || vMix_2.p_save != NULL || ATEM.p_save != NULL) {  // check if at least one swithchers input profile has changed
      p_save = save;
    } else {
      p_save = NULL;
    }
#endif

#ifdef DEBUG
    Serial2.printf("\nSwitcher %d inputNmbr:", ATEM.pos);
    for (int i = 0; i < MAX_CHAN_NUM; i++) {
      Serial2.print(ATEM.inputNumber[i]);
      Serial2.print("-");
    }
    Serial2.printf("\nSwitcher %d enable: %s", ATEM.pos, ATEM.enable);
    Serial2.printf("\nSwitcher %d inputSave: %s", ATEM.pos, ATEM.p_save);
#endif
    Serial2.printf("\r\n");
  }
}

void checkInputNumberProfile(Switcher &sw) {
  uint8_t flag = 0;
  for (uint8_t i = 0; i < MAX_CHAN_NUM; i++) {
    if (EEPROM.read(sw.pos + 4 + i) != sw.inputNumber[i]) {
      flag++;
    }
  }
  if (flag == 0) {
    sw.p_save = NULL;
  } else {
    sw.p_save = save;
  }
}

/*vMix*/
void vMixHandle(EthernetClient &client, Switcher &sw) {
  if (memcmp(sw.enable, "checked", sizeof(sw.enable)) == 0) {
    int8_t len = client.available();
    if (len > 0) {
      sw.keepAliveFlag = 0;            // vynuluj keepAlive flag - prisli data, niet dovod na paniku
      sw.cMillisKeepAlive = millis();  // vynuluj keepAlive čas
      uint8_t buffer[len + 10];
      client.read(buffer, len);
      // Serial.println(len);
#ifdef DEBUG
      Serial2.printf("%d(%d)->", sw.pos, client.getSocketNumber());
      Serial2.write(buffer, len);  // show data from vmix
#endif
      processSwitcherData((char *)buffer, len, sw);
      sw.initialized = 1;
    }

    if (!client.connected()) {
      // Serial.println(client.status());
      if (millis() > cMillisTimeout + TIMEOUT) {
        cMillisTimeout = millis();
        if (connectTovMix(client, sw)) {
          return;
        }
      }

      if (sw.initialized) {
#ifdef DEBUG
        Serial2.println("client DISCONNECTED, stopping.");
#endif
        client.stop();
        sw.initialized = 0;
        setCodeDefaulte(sw);
      }
    } else {
      keepAlive(client, sw);
    }

  } else {
    if (sw.initialized == 1) {
      client.stop();
      sw.initialized = 0;
      sw.code = DEFAULT_VALUE;
      setCodeDefaulte(sw);
    }
  }
  if (client.connected()) {
    sw.status = 1;
  } else {
    sw.status = 0;
  }
}

uint8_t connectTovMix(EthernetClient &client, Switcher &sw) {
  /*Serial.print("freeMemory()=");
  Serial.println(freeMemory());*/
  IPAddress mixer(sw.IPaddr);
#ifdef DEBUG
  Serial2.print("connecting to ");
  Serial2.print(mixer);
  Serial2.print("...");
#endif
  uint8_t result = client.connect(mixer, 8099);
#ifdef DEBUG
  Serial2.printf("result:%d...on socket:%d...", result, client.getSocketNumber());
#endif
  sw.cMillisKeepAlive = millis();
  if (result == 1) {
#ifdef DEBUG
    Serial2.println("successful");
#endif
    client.print("SUBSCRIBE TALLY\r\n");
    return 1;
  } else {
#ifdef DEBUG
    Serial2.println("connection failed");
    return 0;
#endif
  }
}

void keepAlive(EthernetClient &client, Switcher &sw) {
#ifndef KEEPALIVE
  return;
#else
  if (millis() > sw.cMillisKeepAlive + KEEPALIVE) {
    client.print("TALLY\r\n");
#ifdef DEBUG
    Serial2.print(sw.pos);
    Serial2.println("<-TALLY");
#endif
    sw.cMillisKeepAlive = millis();
    sw.keepAliveFlag = 1;
  }

  if ((millis() > sw.cMillisKeepAlive + TTK) && sw.keepAliveFlag) {
#ifdef DEBUG
    Serial2.printf("Request timeout, restarting client %d. Socket status: %d\n", sw.pos, client.status());
#endif
    client.stop();
    sw.initialized = 0;
    sw.code = DEFAULT_VALUE;
    connectTovMix(client, sw);
    sw.keepAliveFlag = 0;
  }
#endif
}

void processSwitcherData(char *gdata, byte len, Switcher &sw) {
  char *p_data = strstr(gdata, "TALLY OK");
  if (p_data != NULL) {
    p_data = strrchr(gdata, ' ');  // find last space from end -  TALLY OK' '01200
    // checkInputNumber(p_data, sw);
    p_data += 1;  // point to first number
    char *default_p = p_data;
    // Serial.print("p_data:");
    // Serial.println(p_data);

    for (int q = 0; q < 5; q++) {
      if (sw.inputNumber[q] == 0) {
        sw.code = (~(0b111 << (q * 3)) & sw.code) | NOTHING << (q * 3);
        sw.tallyValue[q][0] = false;
        sw.tallyValue[q][1] = false;
      } else {
        p_data = default_p + (sw.inputNumber[q] - 1);
        switch (*p_data) {
          case '0':
            sw.code = (~(0b111 << (q * 3)) & sw.code) | NOTHING << (q * 3);
            sw.tallyValue[q][0] = false;
            sw.tallyValue[q][1] = false;
            break;
          case '2':
            sw.code = (~(0b111 << (q * 3)) & sw.code) | PREVIEW << (q * 3);
            sw.tallyValue[q][0] = false;
            sw.tallyValue[q][1] = true;
            break;
          case '1':
            sw.code = (~(0b111 << (q * 3)) & sw.code) | ACTIVE << (q * 3);
            sw.tallyValue[q][0] = true;
            sw.tallyValue[q][1] = false;
            break;
        }
      }
    }

    for (int q = 5; q < MAX_CHAN_NUM; q++) {
      if (sw.inputNumber[q] == 0) {
        sw.tallyValue[q][0] = false;
        sw.tallyValue[q][1] = false;
      } else {
        p_data = default_p + (sw.inputNumber[q] - 1);
        switch (*p_data) {
          case '0':
            sw.tallyValue[q][0] = false;
            sw.tallyValue[q][1] = false;
            break;
          case '2':
            sw.tallyValue[q][0] = false;
            sw.tallyValue[q][1] = true;
            break;
          case '1':
            sw.tallyValue[q][0] = true;
            sw.tallyValue[q][1] = false;
            break;
        }
      }
    }

    sendCodeWireless();
    setBMD_SDI_OUT();

    /*Serial2.printf("%d()--", sw.pos);
    Serial2.println(sw.code, BIN);*/
    /*
    Serial.print(sw.pos);
    Serial.print("FinalCode:");
    Serial.println(code, BIN);
    */
  }
}

/*ATEM*/
void ATEM_handle() {                                               // TODO generate ATEM.code
  if (memcmp(ATEM.enable, "checked", sizeof(ATEM.enable)) == 0) {  // check if BMDSDIControl is enabled
    uint8_t send = false;
    if (sdiTallyControl.available()) {
      
      uint8_t buffer[100];  // TODO shorten array lenght
      uint8_t len = sdiTallyControl.read(buffer, 16);

      Serial2.write(buffer, len);  // show received data
      // TODO parse incoming data

      /*for (int q = 0; q < MAX_CHAN_NUM; q++) {
        bool program;
        bool preview;
        sdiTallyControl.getCameraTally((q + 1), program, preview);
        if (ATEM.tallyValue[q][0] != program || ATEM.tallyValue[q][1] != preview) {
          ATEM.tallyValue[q][0] = program;
          ATEM.tallyValue[q][1] = preview;

          if (q < 5) {  // wireless works only for 5 channels
            if (ATEM.tallyValue[q][0] == true) {
              ATEM.code = (~(0b111 << (q * 3)) & ATEM.code) | ACTIVE << (q * 3);
            } else if (ATEM.tallyValue[q][1] == true) {
              ATEM.code = (~(0b111 << (q * 3)) & ATEM.code) | PREVIEW << (q * 3);
            } else {
              ATEM.code = (~(0b111 << (q * 3)) & ATEM.code) | NOTHING << (q * 3);
            }
          }
          send = true;
        }
      }
      if (send) {
        sendCodeWireless();
        setBMD_SDI_OUT();
      }*/
    }
  } else {
    setCodeDefaulte(ATEM);
  }
}

void sendCodeWireless() {
  uint16_t global_code = DEFAULT_VALUE;  // 4681;
  for (uint8_t q = 0; q < 5; q++) {
    if (((vMix_1.code >> (q * 3)) & 0b111) > ((vMix_2.code >> (q * 3)) & 0b111)) {
      global_code = (~(0b111 << (q * 3)) & global_code) | (((vMix_1.code >> (q * 3)) & 0b111) << (q * 3));
    } else {
      global_code = (~(0b111 << (q * 3)) & global_code) | (((vMix_2.code >> (q * 3)) & 0b111) << (q * 3));
    }
    if (((ATEM.code >> (q * 3)) & 0b111) > ((global_code >> (q * 3)) & 0b111)) {
      global_code = (~(0b111 << (q * 3)) & global_code) | (((ATEM.code >> (q * 3)) & 0b111) << (q * 3));
    }
  }

  Serial1.write(global_code);
  Serial1.write(global_code >> 8);
  Serial1.write(255);
}

void setBMD_SDI_OUT() {
#ifdef DEBUG
  Serial2.printf("SDI_OUT: ");
#endif
  uint8_t finalTallyValue[MAX_CHAN_NUM][2] = {0};
  for (uint8_t q = 0; q < MAX_CHAN_NUM; q++) {
    if (vMix_1.tallyValue[q][0] || vMix_2.tallyValue[q][0] || ATEM.tallyValue[q][0]) {
      finalTallyValue[q][0] = true;
    }
    if ((vMix_1.tallyValue[q][1] || vMix_2.tallyValue[q][1] || ATEM.tallyValue[q][1]) && finalTallyValue[q][0] != true) {
      finalTallyValue[q][1] = true;
    }
#ifdef DEBUG
    Serial2.printf("[%d, %d] ", finalTallyValue[q][0], finalTallyValue[q][1]);
#endif
#ifdef ATEM_enable
    sdiTallyControl.setCameraTally((q + 1), finalTallyValue[q][0], finalTallyValue[q][1]);
#endif
  }
#ifdef DEBUG
  Serial2.printf("\n");
#endif
}

void setCodeDefaulte(Switcher &sw) {
  sw.code = DEFAULT_VALUE;
  for (uint8_t q = 0; q < MAX_CHAN_NUM; q++) {
    sw.tallyValue[q][0] = 0;
  }
  for (uint8_t q = 0; q < MAX_CHAN_NUM; q++) {
    sw.tallyValue[q][1] = 0;
  }
}

/*SETUP function*/
void eepromRead(Switcher &sw) {
  for (uint8_t i = 0; i < 4; i++) {
    sw.IPaddr[i] = EEPROM.read(sw.pos + i);
  }
  for (uint8_t i = 0; i < MAX_CHAN_NUM; i++) {
    sw.inputNumber[i] = EEPROM.read(sw.pos + 4 + i);
  }
  if (EEPROM.read(sw.pos + 4 + MAX_CHAN_NUM) == 1) {
    strcpy(sw.enable, "checked");
  } else {
    memset(sw.enable, '\0', sizeof(sw.enable));
  }
}
void eepromRead_ATEM() {
  for (uint8_t i = 0; i < MAX_CHAN_NUM; i++) {
    ATEM.inputNumber[i] = EEPROM.read(ATEM.pos + 4 + i);
  }
  if (EEPROM.read(ATEM.pos + 4 + MAX_CHAN_NUM) == 1) {
    strcpy(ATEM.enable, "checked");
  } else {
    memset(ATEM.enable, '\0', sizeof(ATEM.enable));
  }
}

/*
  static const uint8_t CLOSED      = 0x00; 0
  static const uint8_t INIT        = 0x13; 19
  static const uint8_t LISTEN      = 0x14; 20 .
  static const uint8_t SYNSENT     = 0x15; 21
  static const uint8_t SYNRECV     = 0x16; 22
  static const uint8_t ESTABLISHED = 0x17; 23 .
  static const uint8_t FIN_WAIT    = 0x18; 24
  static const uint8_t CLOSING     = 0x1A; 26
  static const uint8_t TIME_WAIT   = 0x1B; 27
  static const uint8_t CLOSE_WAIT  = 0x1C; 28 .
  static const uint8_t LAST_ACK    = 0x1D; 29
  static const uint8_t UDP         = 0x22; 34
  static const uint8_t IPRAW       = 0x32; 50
  static const uint8_t MACRAW      = 0x42; 66
  static const uint8_t PPPOE       = 0x5F; 95
  */