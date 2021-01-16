/**************************************************************
   WiFiManager is a library for the ESP8266/Arduino platform
   Built by AlexT https://github.com/tzapu

   Rewrite by Bui Van Huy
   
   Licensed under MIT license
 **************************************************************/

#ifndef WiFiManager_h
#define WiFiManager_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <memory>

#define MAX_TOKEN 50

extern "C" {
  #include "user_interface.h"
}

const char HTTP_HEADER[] PROGMEM          = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";
const char HTTP_STYLE[] PROGMEM           = "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}</style>";
const char HTTP_SCRIPT[] PROGMEM          = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
const char HTTP_HEADER_END[] PROGMEM        = "</head><body><div style='text-align:left;display:inline-block;min-width:260px;'>";
const char HTTP_PORTAL_OPTIONS[] PROGMEM  = "<form action=\"/wifi\" method=\"get\"><button>Configure WiFi</button></form><br/><form action=\"/0wifi\" method=\"get\"><button>Configure WiFi (No Scan)</button></form><br/><form action=\"/blynk\" method=\"get\"><button>Configure Blynk</button></form><br/><form action=\"/i\" method=\"get\"><button>Info</button></form><br/><form action=\"/r\" method=\"post\"><button>Reset</button></form>";
const char HTTP_ITEM[] PROGMEM            = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char HTTP_FORM_START[] PROGMEM      = "<form method='get' action='wifisave'><input id='s' name='s' length=32 placeholder='SSID'><br/><input id='p' name='p' length=64 type='password' placeholder='password'><br/>";
const char HTTP_FORM_PARAM[] PROGMEM      = "<br/><input id='{i}' name='{n}' maxlength={l} placeholder='{p}' value='{v}' {c}>";
const char HTTP_FORM_END[] PROGMEM        = "<br/><button type='submit'>save</button></form>";
const char HTTP_SCAN_LINK[] PROGMEM       = "<br/><div class=\"c\"><a href=\"/wifi\">Scan</a></div>";
const char HTTP_SAVED[] PROGMEM           = "<div>Credentials Saved<br />Trying to connect ESP to network.<br />If it fails reconnect to AP to try again</div>";
const char HTTP_END[] PROGMEM             = "</div></body></html>";

const char HTTP_BLYNK_PAGE[] PROGMEM             = "<h3><b><font color=\"1fa3ec\">Token Blynk</font></b></h3> <h4><b><font color=\"46FF33\">{token-blynk}</font></b></h4>  <br /><br /><br /><br />   <h5><font color=\"1fa3ec\">Enter new token Blynk (max {max-value} characters)</font></h5>  <h5><font color=\"1fa3ec\">Enter \"clear\" to clear token </font></h5>  <form action=\"\" method=\"get\"><input maxlength=\"{max-leng}\" name=\"token\" /><br /><br />   <form action=\"/submit\" method=\"post\"><button>Change Token</button></form><br /><br />   <form action=\"/\" method=\"get\"><form action=\"/submit\" method=\"post\"><button>Back</button></form></form></form>";


const char HTTP_ICON_BLINK[] PROGMEM = "<img src= \"   data:image/png;base64,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   \" alt= \"Red dot \" />  ";

#ifndef WIFI_MANAGER_MAX_PARAMS
#define WIFI_MANAGER_MAX_PARAMS 10
#endif

class WiFiManagerParameter {
public:
    /** 
        Create custom parameters that can be added to the WiFiManager setup web page
        @id is used for HTTP queries and must not contain spaces nor other special characters
    */
    WiFiManagerParameter(const char *custom);
    WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length);
    WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);
    ~WiFiManagerParameter();

    const char *getID();
    const char *getValue();
    const char *getPlaceholder();
    int         getValueLength();
    const char *getCustomHTML();
private:
    const char *_id;
    const char *_placeholder;
    char       *_value;
    int         _length;
    const char *_customHTML;

    void init(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);

    friend class WiFiManager;
};


class WiFiManager
{
public:
    WiFiManager();
    ~WiFiManager();

    boolean       autoConnect();
    boolean       autoConnect(char const *apName, char const *apPassword = NULL);

    //if you want to always start the config portal, without trying to connect first
    boolean       startConfigPortal();
    boolean       startConfigPortal(char const *apName, char const *apPassword = NULL);

    // get the AP name of the config portal, so it can be used in the callback
    String        getConfigPortalSSID();

    void          resetSettings();

    //sets timeout before webserver loop ends and exits even if there has been no setup.
    //useful for devices that failed to connect at some point and got stuck in a webserver loop
    //in seconds setConfigPortalTimeout is a new name for setTimeout
    void          setConfigPortalTimeout(unsigned long seconds);
    void          setTimeout(unsigned long seconds);

    //sets timeout for which to attempt connecting, useful if you get a lot of failed connects
    void          setConnectTimeout(unsigned long seconds);


    void          setDebugOutput(boolean debug);
    //defaults to not showing anything under 8% signal quality if called
    void          setMinimumSignalQuality(int quality = 8);
    //sets a custom ip /gateway /subnet configuration
    void          setAPStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn);
    //sets config for a static IP
    void          setSTAStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn);
    //called when AP mode and config portal is started
    void          setAPCallback( void (*func)(WiFiManager*) );
    //called when settings have been changed and connection was successful
    void          setSaveConfigCallback( void (*func)(void) );
    //adds a custom parameter, returns false on failure
    bool          addParameter(WiFiManagerParameter *p);
    //if this is set, it will exit after config, even if connection is unsuccessful.
    void          setBreakAfterConfig(boolean shouldBreak);
    //if this is set, try WPS setup when starting (this will delay config portal for up to 2 mins)
    //TODO
    //if this is set, customise style
    void          setCustomHeadElement(const char* element);
    //if this is true, remove duplicated Access Points - defaut true
    void          setRemoveDuplicateAPs(boolean removeDuplicates);


    /**************************************************************
***************************************************************/

    String        getToken();
    void          saveToken(String token_w);
    void          clearToken();

private:
    std::unique_ptr<DNSServer>        dnsServer;
    std::unique_ptr<ESP8266WebServer> server;

    //const int     WM_DONE                 = 0;
    //const int     WM_WAIT                 = 10;

    //const String  HTTP_HEADER = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/><title>{v}</title>";

    void          setupConfigPortal();
    void          startWPS();

    const char*   _apName                 = "no-net";
    const char*   _apPassword             = NULL;
    String        _ssid                   = "";
    String        _pass                   = "";
    unsigned long _configPortalTimeout    = 0;
    unsigned long _connectTimeout         = 0;
    unsigned long _configPortalStart      = 0;

    IPAddress     _ap_static_ip;
    IPAddress     _ap_static_gw;
    IPAddress     _ap_static_sn;
    IPAddress     _sta_static_ip;
    IPAddress     _sta_static_gw;
    IPAddress     _sta_static_sn;

    int           _paramsCount            = 0;
    int           _minimumQuality         = -1;
    boolean       _removeDuplicateAPs     = true;
    boolean       _shouldBreakAfterConfig = false;
    boolean       _tryWPS                 = false;

    const char*   _customHeadElement      = "";

    //String        getEEPROMString(int start, int len);
    //void          setEEPROMString(int start, int len, String string);

    int           status = WL_IDLE_STATUS;
    int           connectWifi(String ssid, String pass);
    uint8_t       waitForConnectResult();

    void          handleRoot();
    void          handleWifi(boolean scan);
    void          handleWifiSave();
    void          handleBlynkSave();
    void          handleInfo();
    void          handleReset();
    void          handleNotFound();
    void          handle204();
    boolean       captivePortal();
    boolean       configPortalHasTimeout();

    // DNS server
    const byte    DNS_PORT = 53;

    //helpers
    int           getRSSIasQuality(int RSSI);
    boolean       isIp(String str);
    String        toStringIp(IPAddress ip);

    boolean       connect;
    boolean       _debug = true;

    void (*_apcallback)(WiFiManager*) = NULL;
    void (*_savecallback)(void) = NULL;

    int                    _max_params;
    WiFiManagerParameter** _params;

    template <typename Generic>
    void          DEBUG_WM(Generic text);

    template <class T>
    auto optionalIPFromString(T *obj, const char *s) -> decltype(  obj->fromString(s)  ) {
      return  obj->fromString(s);
  }
  auto optionalIPFromString(...) -> bool {
      DEBUG_WM("NO fromString METHOD ON IPAddress, you need ESP8266 core 2.1.0 or newer for Custom IP configuration to work.");
      return false;
  }
};




//**********************************************************************************************
//*                                                                                            *
//*                                     CLASS EPPROM                                           *
//*                                                                                            *
//**********************************************************************************************

class EEPROMclass
{

public:
    EEPROMclass();
    ~EEPROMclass();
    void EEPROM_Init();
    void EEPROM_End();
    void write(String data, int beginA, int endA);
    String read(int beginA, int endA);
    void clear(int beginA, int endA);
    void          DEBUG_WM(String text);

private:
    byte TIME_DELAY =  1;
    byte EEPROM_SIZE = 500;
};

#endif
