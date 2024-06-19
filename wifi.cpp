/*
 * @file wifi.cpp
 *
 * @brief C++ abstraction layer of the WiFi ESP API procedures
 *
 * @date Created on: 17 дек. 2023 г.
 * @Author: aso
 */

//#include <algorithm>
//#include <cstdint>
#include <cstring>
#include <string>
#include <cctype>
#include <utility>

#include <esp_wifi_types.h>
#include <esp_wifi.h>

#include <esp_log.h>

#include <esp_netif.h>

#include <esp_system.h>
#include <esp_types.h>
#include <esp_event.h>

#include <asemaphore>
#include <event_ctrl.hpp>
#include <sync.hpp>

#include "net.h"
#include "wifi.h"
#include "sdkconfig.h"
//#include "net_cfg.h"



using namespace std;



/// @brief Create exemplar of the wifi esp::netif
esp::wifi::netif_t::netif_t(wifi_interface_t wifi_if, esp_netif_inherent_config_t &config): netif_t()
{
    type = wifi_if;
    create(wifi_if, config);
}; /* esp::netif::netif */

/// Main procedure for creation the WiFi Netif
esp_netif_t* esp::wifi::netif_t::create(wifi_interface_t wifi_if, esp_netif_inherent_config_t &config)
{
    type = wifi_if;
    instance = esp_netif_create_wifi(wifi_if, &config);
    return instance;
}; /* esp::netif::create */



//--[ class esp::wifi::stack ]-----------------------------------------------------------------------------------------


esp::net::wifi::config_t esp::net::wifi::stack::configuration(wifi_interface_t if_type)
{
	config_t conf;

    esp_wifi_get_config(if_type, &conf.data);
    return conf;
}; /* esp::net::wifi::stack::configuration(wifi_interface_t) */


esp_err_t esp::net::wifi::stack::err = ESP_ERR_WIFI_NOT_INIT;



//--[ class esp::wifi::config_t ]--------------------------------------------------------------------------------------


template <>
const esp::net::wifi::config_t& esp::net::wifi::config_t::set(const wifi_ap_config_t& config);
template <>
const esp::net::wifi::config_t& esp::net::wifi::config_t::set(const wifi_sta_config_t& config);
template <>
const esp::net::wifi::config_t& esp::net::wifi::config_t::set(::net::configuration_t const&);

/// Default constructor
esp::net::wifi::config_t::config_t(wifi_interface_t _type):
	type(_type)
{ }; /* esp::wifi::config_t::config_t() */

/// General constructor from parent type
esp::net::wifi::config_t::config_t(wifi_interface_t tp, const wifi_config_t& config)
	//config_t(tp)
{
    set(tp, config);
}; /* esp::wifi::config_t::config_t(wifi_interface_t, wifi_config_t) */

/// copy-constructor
//template <typename T>
//esp::net::wifi::config_t::config_t(const T&);


/// Assigment parent type with kind of interface
const esp::net::wifi::config_t&
esp::net::wifi::config_t::set(wifi_interface_t tp, const wifi_config_t& config)
{
    //type = tp;
    switch (tp)
    {
    case WIFI_IF_STA:
//	strncpy(data.sta.ssid, config.sta.ssid, sizeof(data.sta.ssid));	// uint8_t ssid[32];      /**< SSID of target AP. */
//	strncpy(data.sta.password, config.sta.password, sizeof(data.sta.password));	// uint8_t password[64];  /**< Password of target AP. */
//	data.sta.scan_method	= config.sta.scan_method;	/**< do all channel scan or fast scan */
//	data.sta.bssid_set	= config.sta.bssid_set;	/**< whether set MAC address of target AP or not. Generally, station_config.bssid_set needs to be 0; and it needs to be 1 only when users need to check the MAC address of the AP.*/
//	strncpy(data.sta.bssid, config.sta.bssid, sizeof(data.sta.bssid));     /**< MAC address of target AP*/
//	data.sta.channel	= config.sta.channel;		/**< channel of target AP. Set to 1~13 to scan starting from the specified channel before connecting to AP. If the channel of AP is unknown, set it to 0.*/
//	data.sta.listen_interval = config.sta.listen_interval;   /**< Listen interval for ESP32 station to receive beacon when WIFI_PS_MAX_MODEM is set. Units: AP beacon intervals. Defaults to 3 if set to 0. */
//	data.sta.sort_method	= config.sta.sort_method;    /**< sort the connect AP in the list by rssi or security mode */
//	data.sta.threshold	= config.sta.threshold;     /**< When sort_method is set, only APs which have an auth mode that is more secure than the selected auth mode and a signal stronger than the minimum RSSI will be used. */
//	data.sta.pmf_cfg.capable = config.sta.pmf_cfg.capable;	/**< Configuration for Protected Management Frame. Will be advertized in RSN Capabilities in RSN IE. */
//	data.sta.pmf_cfg.required = config.sta.pmf_cfg.required;
//	data.sta.rm_enabled	= config.sta.rm_enabled;	/**< Whether Radio Measurements are enabled for the connection */
//	data.sta.btm_enabled	= config.sta.btm_enabled;	/**< Whether BSS Transition Management is enabled for the connection */
//	data.sta.mbo_enabled	= config.sta.mbo_enabled;	/**< Whether MBO is enabled for the connection */
//	data.sta.transition_disable = config.sta.transition_disable;	/**< Whether to enable transition disable feature */
//	data.sta.reserved	= config.sta.reserved;	/**< Reserved for future feature set */
//	data.sta.sae_pwe_h2e	= config.sta.sae_pwe_h2e;	/**< Whether SAE hash to element is enabled */
//	data.sta.failure_retry_cnt = config.sta.failure_retry_cnt;/**< Number of connection retries station will do before moving to next AP. scan_method should be set as WIFI_ALL_CHANNEL_SCAN to use this config. Note: Enabling this may cause connection time to increase incase best AP doesn't behave properly. */
	return set(config.sta);
	break;

    case WIFI_IF_AP:
//	strncpy(data.ap.ssid, config.ap.ssid, sizeof(data.ap.ssid));	// uint8_t ssid[32];           /**< SSID of ESP32 soft-AP. If ssid_len field is 0, this must be a Null terminated string. Otherwise, length is set according to ssid_len. */
//	strncpy(data.ap.password, config.ap.password, sizeof(data.ap.password)); // uint8_t password[64];       /**< Password of ESP32 soft-AP. */
//	data.ap.ssid_len    = config.ap.ssid_len;	/**< Optional length of SSID field. */
//	data.ap.channel	    = config.ap.channel;	/**< Channel of soft-AP */
//	data.ap.authmode    = config.ap.authmode;  /**< Auth mode of soft-AP. Do not support AUTH_WEP, AUTH_WAPI_PSK and AUTH_OWE in soft-AP mode. When the auth mode is set to WPA2_PSK, WPA2_WPA3_PSK or WPA3_PSK, the pairwise cipher will be overwritten with WIFI_CIPHER_TYPE_CCMP. */
//	data.ap.ssid_hidden = config.ap.ssid_hidden;	/**< Broadcast SSID or not, default 0, broadcast the SSID */
//	data.ap.max_connection	= config.ap.max_connection;	/**< Max number of stations allowed to connect in */
//	data.ap.beacon_interval	= config.ap.beacon_interval;	/**< Beacon interval which should be multiples of 100. Unit: TU(time unit, 1 TU = 1024 us). Range: 100 ~ 60000. Default value: 100 */
//	data.ap.pairwise_cipher	= config.ap.pairwise_cipher;	/**< Pairwise cipher of SoftAP, group cipher will be derived using this. Cipher values are valid starting from WIFI_CIPHER_TYPE_TKIP, enum values before that will be considered as invalid and default cipher suites(TKIP+CCMP) will be used. Valid cipher suites in softAP mode are WIFI_CIPHER_TYPE_TKIP, WIFI_CIPHER_TYPE_CCMP and WIFI_CIPHER_TYPE_TKIP_CCMP. */
//	data.ap.ftm_responder	= config.ap.ftm_responder;	/**< Enable FTM Responder mode */
	return set(config.ap);
    }; /* switch type */
    return *this;
}; /* esp::net::wifi::config_t::set(wifi_interface_t, const wifi_config_t&) */

/// The assigment (setup) procedure
//template <typename T>
//const esp::net::wifi::config_t& esp::net::wifi::config_tset(T&);

template <>
const esp::net::wifi::config_t& esp::net::wifi::config_t::set(const config_t& config)
{
    return set(config.type, config.data);
}; /* esp::net::wifi::config_t::set(const esp::net::wifi::config_t&) */
template <>
const esp::net::wifi::config_t& esp::net::wifi::config_t::set<esp::net::wifi::config_t>(const config_t& config);

template <>
const esp::net::wifi::config_t& esp::net::wifi::config_t::set(const ::net::configuration_t& config)
{
    ssid(config.login);
    passwd(config.passwd);
    return *this;
}; /* esp::net::wifi::config_t::set<net::configuration>() */



template <>
const esp::net::wifi::config_t& esp::net::wifi::config_t::set(const wifi_ap_config_t& config)
{
    type = WIFI_IF_AP;
//    strncpy(data.ap.ssid, config.ssid, sizeof(data.ap.ssid));	// uint8_t ssid[32];           /**< SSID of ESP32 soft-AP. If ssid_len field is 0, this must be a Null terminated string. Otherwise, length is set according to ssid_len. */
    memcpy(data.ap.ssid, config.ssid, sizeof(data.ap.ssid));	// uint8_t ssid[32];           /**< SSID of ESP32 soft-AP. If ssid_len field is 0, this must be a Null terminated string. Otherwise, length is set according to ssid_len. */
//    strncpy(data.ap.password, config.password, sizeof(data.ap.password)); // uint8_t password[64];       /**< Password of ESP32 soft-AP. */
    memcpy(data.ap.password, config.password, sizeof(data.ap.password)); // uint8_t password[64];       /**< Password of ESP32 soft-AP. */
    data.ap.ssid_len    = config.ssid_len;	/**< Optional length of SSID field. */
    data.ap.channel	    = config.channel;	/**< Channel of soft-AP */
    data.ap.authmode    = config.authmode;  /**< Auth mode of soft-AP. Do not support AUTH_WEP, AUTH_WAPI_PSK and AUTH_OWE in soft-AP mode. When the auth mode is set to WPA2_PSK, WPA2_WPA3_PSK or WPA3_PSK, the pairwise cipher will be overwritten with WIFI_CIPHER_TYPE_CCMP. */
    data.ap.ssid_hidden = config.ssid_hidden;	/**< Broadcast SSID or not, default 0, broadcast the SSID */
    data.ap.max_connection	= config.max_connection;	/**< Max number of stations allowed to connect in */
    data.ap.beacon_interval	= config.beacon_interval;	/**< Beacon interval which should be multiples of 100. Unit: TU(time unit, 1 TU = 1024 us). Range: 100 ~ 60000. Default value: 100 */
    data.ap.pairwise_cipher	= config.pairwise_cipher;	/**< Pairwise cipher of SoftAP, group cipher will be derived using this. Cipher values are valid starting from WIFI_CIPHER_TYPE_TKIP, enum values before that will be considered as invalid and default cipher suites(TKIP+CCMP) will be used. Valid cipher suites in softAP mode are WIFI_CIPHER_TYPE_TKIP, WIFI_CIPHER_TYPE_CCMP and WIFI_CIPHER_TYPE_TKIP_CCMP. */
    data.ap.ftm_responder	= config.ftm_responder;	/**< Enable FTM Responder mode */
    return *this;;
}; /* esp::net::wifi::config_t::set(wifi_ap_config_t&) */

template <>
const esp::net::wifi::config_t& esp::net::wifi::config_t::set(const wifi_sta_config_t& config)
{
    type = WIFI_IF_STA;
//    strncpy(data.sta.ssid, config.ssid, sizeof(data.sta.ssid));	// uint8_t ssid[32];      /**< SSID of target AP. */
    memcpy(data.sta.ssid, config.ssid, sizeof(data.sta.ssid));	// uint8_t ssid[32];      /**< SSID of target AP. */
//    strncpy(data.sta.password, config.password, sizeof(data.sta.password));	// uint8_t password[64];  /**< Password of target AP. */
    memcpy(data.sta.password, config.password, sizeof(data.sta.password));	// uint8_t password[64];  /**< Password of target AP. */
    data.sta.scan_method	= config.scan_method;	/**< do all channel scan or fast scan */
    data.sta.bssid_set	= config.bssid_set;	/**< whether set MAC address of target AP or not. Generally, station_config.bssid_set needs to be 0; and it needs to be 1 only when users need to check the MAC address of the AP.*/
//    strncpy(data.sta.bssid, config.bssid, sizeof(data.sta.bssid));     /**< MAC address of target AP*/
    memcpy(data.sta.bssid, config.bssid, sizeof(data.sta.bssid));     /**< MAC address of target AP*/
    data.sta.channel	= config.channel;		/**< channel of target AP. Set to 1~13 to scan starting from the specified channel before connecting to AP. If the channel of AP is unknown, set it to 0.*/
    data.sta.listen_interval = config.listen_interval;   /**< Listen interval for ESP32 station to receive beacon when WIFI_PS_MAX_MODEM is set. Units: AP beacon intervals. Defaults to 3 if set to 0. */
    data.sta.sort_method	= config.sort_method;    /**< sort the connect AP in the list by rssi or security mode */
    data.sta.threshold	= config.threshold;     /**< When sort_method is set, only APs which have an auth mode that is more secure than the selected auth mode and a signal stronger than the minimum RSSI will be used. */
    data.sta.pmf_cfg.capable = config.pmf_cfg.capable;	/**< Configuration for Protected Management Frame. Will be advertized in RSN Capabilities in RSN IE. */
    data.sta.pmf_cfg.required = config.pmf_cfg.required;
    data.sta.rm_enabled	= config.rm_enabled;	/**< Whether Radio Measurements are enabled for the connection */
    data.sta.btm_enabled	= config.btm_enabled;	/**< Whether BSS Transition Management is enabled for the connection */
    data.sta.mbo_enabled	= config.mbo_enabled;	/**< Whether MBO is enabled for the connection */
    data.sta.transition_disable = config.transition_disable;	/**< Whether to enable transition disable feature */
    data.sta.reserved	= config.reserved;	/**< Reserved for future feature set */
    data.sta.sae_pwe_h2e	= config.sae_pwe_h2e;	/**< Whether SAE hash to element is enabled */
    data.sta.failure_retry_cnt = config.failure_retry_cnt;/**< Number of connection retries station will do before moving to next AP. scan_method should be set as WIFI_ALL_CHANNEL_SCAN to use this config. Note: Enabling this may cause connection time to increase incase best AP doesn't behave properly. */
    return *this;;
}; /* esp::net::wifi::config_t::set(wifi_sta_config_t&) */


/// ssid/bssid/password data string copy utility
const char* xssid_cstr(const uint8_t buf[], size_t maxlen)
{
	static char strbuf[65];
    memcpy(strbuf, buf, min(maxlen, 64U));
    strbuf[min(maxlen, 64U)] = '\0';	///< terminate the output string, for safety
    return strbuf;
}; /* xssid_cstr() */

// Set the SSID
//esp_err_t esp::net::wifi::config_t::ssid(const std::string&);

esp_err_t esp::net::wifi::config_t::ssid(const char name[])
{
//	size_t size = min(sizeof(data.sta.ssid), (strlen(name) + 1));// (sizeof(data.sta.ssid) < (strlen(name) + 1))? sizeof(data.sta.ssid): (strlen(name) + 1));
	size_t size = strlen(name) + 1;

    if (type == WIFI_IF_STA)
    {
	size = min(sizeof(data.sta.ssid), size);// (sizeof(data.sta.ssid) < (strlen(name) + 1))? sizeof(data.sta.ssid): (strlen(name) + 1));
	memcpy(data.sta.ssid, name, size);
    } /* if type == WIFI_IF_STA */
    else
    {
	size = min(sizeof(data.ap.ssid), size);
	memcpy(data.ap.ssid, name, size);
    }; /* else if type == WIFI_IF_STA  */
    return ESP_OK;
}; /* esp::net::wifi::config_t::ssid(char[]) */

// Get the SSID
//std::string ssid();
const char* esp::net::wifi::config_t::ssid_cstr() const
{
    if (type == WIFI_IF_STA)
	return xssid_cstr(data.sta.ssid, sizeof(data.sta.ssid));
    else
	return xssid_cstr(data.ap.ssid, sizeof(data.ap.ssid));
}; /* esp::net::wifi::config_t::ssid_csctr() */

// Set Password
//esp_err_t passwd(const std::string&);
esp_err_t esp::net::wifi::config_t::passwd(const char pwd[])
{
	size_t size = strlen(pwd) + 1;

    if (type == WIFI_IF_STA)
    {
	size = min(sizeof(data.sta.password), size);
	memcpy(data.sta.password, pwd, size);
    } /* if type == WIFI_IF_STA */
    else
    {
	size = min(sizeof(data.ap.password), size);
	memcpy(data.ap.password, pwd, size);
    }; /* else if type == WIFI_IF_STA  */
    return ESP_OK;
}; /* esp::net::wifi::config_t::passwd(char[]) */

// Get the Password
//std::string passwd();
const char* esp::net::wifi::config_t::passwd_cstr() const
{
    if (type == WIFI_IF_STA)
	return xssid_cstr(data.sta.password, sizeof(data.sta.password));
    else
	return xssid_cstr(data.ap.password, sizeof(data.ap.password));
}; /* esp::net::wifi::config_t::ssid_csctr() */

/// Set the BSSID
/// @return
///	ESP_OK	- operation comleted successfully
///	ESP_ERR_WIFI_MODE - requested BSSID in AP-mode is absent
//esp_err_t bssid(const std::string&);
esp_err_t esp::net::wifi::config_t::bssid(const char name[])
{

    if (type == WIFI_IF_STA)
    {
	    size_t size = min(sizeof(data.sta.bssid), (strlen(name) + 1));

	memcpy(data.sta.bssid, name, size);
	return ESP_OK;
    }; /* if type == WIFI_IF_STA */
    return ESP_ERR_WIFI_MODE;
}; /* esp::net::wifi::config_t::bssid(char[]) */

// Get the BSSID
//std::string bssid();
const char* esp::net::wifi::config_t::bssid_cstr() const
{
    if (type == WIFI_IF_STA)
	return xssid_cstr(data.sta.bssid, sizeof(data.sta.bssid));
    else
	return "";
}; /* esp::net::wifi::config_t::bssid_csctr() */


#if 0
/// assigment operators
//template <typename T>
//const T& esp::net::wifi::config_t::operator =(T&);

template <>
const esp::net::wifi::config_t& esp::net::wifi::config_t::operator =(const esp::net::wifi::config_t& config)
{
    set(config.type, config.data);
    return *this;
}; /* esp::net::wifi::config_t::operator =(const esp::net::wifi::config_t&) */

template <>
const esp::net::wifi::config_t& esp::net::wifi::config_t::operator =(const wifi_ap_config_t& config);

template <>
const esp::net::wifi::config_t& esp::net::wifi::config_t::operator =(const wifi_sta_config_t& config);
#endif



///@brief  reason-codes, defined in ESP-IDF SDK
///@detail Decoding the WiFi error reason enum, return text message describe of it,
///        pure C implementation of this function
///@param [in] reason reason of the WiFi error
///@return ASCIIZ C-string with text name of error reason
const char* wifi_err_reason(wifi_err_reason_t reason)
{
    return esp::wifi::err::reason(reason);
}; /* wifi_err_reason(reason) */


//*********************************************************************************************************************
//
// Таблица значений reason-code (коды причины), определенные в библиотеке ESP32-C3.
// - Первый столбец - имя макроса, определенное в esp_wifi_types.h.
//     Общий префикс WIFI_REASON из имен удален, т. е. UNSPECIFIED -> реально WIFI_REASON_UNSPECIFIED, и т.д.
// - Второй столбец -  значение причины.
// - Третий столбец - стандартное значение, которое отображается как reason (причина) в секции 8.4.1.7 стандарта IEEE 802.11-2012
//     (для дополнительной информации см. этот стандарт).
// - Последний столбец - описание reason (причины).
//
//	Reason code	Значен.	Отобр.	Описание
//    UNSPECIFIED	1	1	Обычно это означает внутреннюю ошибку,  например закончилась память, сбой внутреннего TX, или reason принята от противоположной стороны.
//    AUTH_EXPIRE	2	2	Предыдущая аутентификация не является более допустимой. Для станции ESP эта reason сообщается в случаях:
//					    - таймаут аутентификации.
//					    - причина получена от AP.
//					    Для ESP AP, эта reason сообщается в случаях:
//					    - AP не приняла какие-либо пакеты от станции за последующие 5 минут.
//					    - AP остановлена вызовом esp_wifi_stop().
//					    - станция переведена в состояние de-authed (не аутентифицирована) вызовом esp_wifi_deauth_sta().
//    AUTH_LEAVE	3	3	De-authenticated (не аутентифицирована),  потому что отправляющая станция отключается (или отключилась).
//					    Для станции ESP эта reason сообщается в случае: она получена от AP.
//    ASSOC_EXPIRE	4	4	Отключено из-за неактивности.
//					    Для станции ESP эта reason сообщается в случае: она получена от AP.
//					    Для ESP AP эта причина сообщается в случаях:
//					    - AP не приняла какие-либо пакеты от станции в течение 5 минут.
//					    - AP остановлена вызовом esp_wifi_stop().
//					    - станция переведена в состояние de-authed вызовом esp_wifi_deauth_sta().
//    ASSOC_TOOMANY	5	5	Отключено, потому что AP не может обработать все текущие ассоциированные STA одновременно.
//					    Для станции ESP эта reason сообщается в случае: она получена от AP.
//					    Для ESP AP эта reason сообщается в случае: станции, ассоциированные с AP, достигли максимального количества, которое может поддержать AP.
//    NOT_AUTHED	6	6	От не аутентифицированной станции принят кадр Class-2.
//					    Для станции ESP эта reason сообщается в случае: она получена от AP.
//					    Для ESP AP эта reason сообщается в случае: AP получила пакет с данными от не аутентифицированной станции.
//    NOT_ASSOCED	7	7	От не ассоциированной станции принят кадр Class-3.
//					    Для станции ESP эта reason сообщается в случае: она получена от AP.
//					    Для ESP AP эта reason сообщается в случае: AP получила пакет с данными от не ассоциированной станции.
//    ASSOC_LEAVE	8	8	Отключено, потому что отправляющая станция оставляет (или оставила) BSS.
//					    Для станции ESP, эта reason сообщается в случае:
//					    - она принята от AP.
//					    - станция отключена через esp_wifi_disconnect() и другое API.
//    ASSOC_NOT_AUTHED	9	9	Станция, запрашивающая (повторную) ассоциацию не аутентифицирована отвечающей STA.
//					    Для станции ESP, эта reason сообщается в случае:
//					    - она принята от AP.
//					    Для ESP AP эта reason сообщается в случае:
//					    - AP приняла пакеты с данными от ассоциированной, но не аутентифицированной станции.
//  DISASSOC_PWRCAP_BAD	10	10	Отключено, потому что недопустима информация в элементе Power Capability.
//					    Для станции ESP эта reason сообщается в случае: она получена от AP.
// DISASSOC_SUPCHAN_BAD	11	11	Отключено, потому что недопустима информация в элементе Supported Channels.
//					    Для станции ESP, эта reason сообщается в случае: она принята от AP.
//    IE_INVALID	13	13	Недопустимый элемент, например элемент, содержимое которого не удовлетворяет спецификациями стандарта в пункте формата кадра.
//					    Для станции ESP, эта reason сообщается в случае: она принята от AP.
//					    Для ESP AP, эта reason сообщается в случае: AP обработано неправильный WPA или RSN IE.
//    MIC_FAILURE	14	14	Ошибка кода целостности сообщения (MIC).
//					    Для станции ESP эта reason сообщается, в случае: она принята от AP.
// 4WAY_HANDSHAKE_TIMEOUT 15	15	Таймаут процесса four-way handshake.
//					По соображениям совместимости в ESP код reason заменен на WIFI_REASON_HANDSHAKE_TIMEOUT.
//					   Для станции ESP эта reason сообщается в случае:
//					    - таймаут handshake.
//					    - она принята от AP.
// GROUP_KEY_UPDATE_TIMEOUT 16	16	Таймаут Group-Key Handshake.
//					    Для станции ESP эта reason сообщается в случае: она принята от AP.
//   IE_IN_4WAY_DIFFERS	17	17	Элемент в four-way handshake отличается от кадра (Re-)Association Request/Probe и Response/Beacon frame.
//					    Для станции ESP эта reason сообщается в случае:
//					    - она принята от AP.
//					    - станция обнаружила, что four-way handshake IE отличается от IE в кадре (Re-)Association Request/Probe и Response/Beacon.
//  GROUP_CIPHER_INVALID  18	18	Invalid group cipher.
//					    Для станции ESP эта reason сообщается в случае: она принята от AP.
//  PAIRWISE_CIPHER_INVALID 19	19	Недопустимый pairwise cipher.
//					    Для станции ESP эта reason сообщается в случае: она принята от AP.
//    AKMP_INVALID	20	20	Недопустимый AKMP.
//					    Для станции ESP эта reason сообщается в случае: - она принята от AP.
//  UNSUPP_RSN_IE_VERSION  21	21	Не поддерживаемая версия RSNE.
//					    Для станции ESP эта reason сообщается в случае: она принята от AP.
//    INVALID_RSN_IE_CAP  22	22	Недопустимые возможности RSNE.
//					    Для станции ESP эта reason сообщается в случае: она принята от AP.
//    802_1X_AUTH_FAILED  23	23	Ошибка аутентификации IEEE 802.1X.
//					    Для станции ESP эта reason сообщается в случае: она принята от AP.
//					    Для ESP AP эта reason сообщается в случае: ошибка аутентификации IEEE 802.1X.
//  CIPHER_SUITE_REJECTED 24	24	Подсистема шифра отклонена политиками безопасности.
//					    Для станции ESP эта reason сообщается в случае: она принята от AP.
//  TDLS_PEER_UNREACHABLE 25	25	Разрушение TDLS direct-link из-за того, что недоступен TDLS peer STA через прямое соединение TDLS.
//  TDLS_UNSPECIFIED	26	26	Разрушение TDLS direct-link по неуказанной причине.
//  SSP_REQUESTED_DISASSOC  27	27	Отключено, потому что сессия прервана запросом SSP.
//  NO_SSP_ROAMING_AGREEMENT 28	28	Отключено из-за отсутствия соглашения SSP roaming.
//  BAD_CIPHER_OR_AKM	29	29	Запрошенная служба отклонена из-за требований подсистемы чипера SSP или AKM requirement.
//  NOT_AUTHORIZED_THIS_LOCATION 30  30	Запрошенная служба не авторизована в этом месте.
//  SERVICE_CHANGE_PERCLUDES_TS	 31  31	TS удалено, потому что у QoS AP отсутствует достаточная полоса для этой QoS STA из-за изменений характеристик службы BSS или рабочего режима
//					(например HT BSS поменялось от канала 40 МГц на канал 20 МГц).
//    UNSPECIFIED_QOS	32	32	Отключение по не указанной, связанной с QoS причиной.
//  NOT_ENOUGH_BANDWIDTH  33	33	Отключено, потому что у QoS AP отсутствует достаточная полоса для этого QoS STA.
//    MISSING_ACKS	34	34	Отключено, потому что должно быть подтверждено слишком большое количество кадров, они не подтверждены из-за передач AP и/или плохих условий канала.
//    EXCEEDED_TXOP	35	35	Отключено, потому что STA передает вне предела её TXOP.
//    STA_LEAVING	36	36	Запрашивающая STA покинула BSS (или сброшена).
//    END_BA		37	37	Запрашивающая STA не использует больше поток или сессию.
//    UNKNOWN_BA	38	38	Запрашивающая STA приняла кадры с использованием механизма, для которого не была завершена настройка.
//    TIMEOUT		39	39	Таймаут запроса пира STA.
//    Reserved		40..45	40..45	----
//    PEER_INITIATED	46	46	В отключенном кадре: отключено, потому что достигнут предел авторизованного доступа.
//    AP_INITIATED	47	47	В отключенном кадре: отключена из-за требований внешней службы.
// INVALID_FT_ACTION_FRAME_COUNT 48 48	Недопустимый счетчик кадров FT Action.
//    INVALID_PMKID	49	49	Недопустимый идентификатор парного мастер-ключа (PMKID).
//    INVALID_MDE	50	50	Недопустимый MDE.
//    INVALID_FTE	51	51	Недопустимый FTE.
// TRANSMISSION_LINK_ESTABLISH_FAILED  67  67	Неудача установки линка передачи на альтернативном канале.
// ALTERATIVE_CHANNEL_OCCUPIED  68  68	Альтернативный канал занят.
//    BEACON_TIMEOUT	200  reserved	Код reason Wi-Fi, специфичный для Espressif: когда станция потеряла N непрерывных beacon-ов, она разрушает соединение и сообщает эту reason.
//    NO_AP_FOUND	201  reserved	Код reason Wi-Fi, специфичный для Espressif: когда станция не смогла просканировать целевую AP.
//    AUTH_FAIL		202  reserved	Код reason Wi-Fi, специфичный для Espressif: неудачная аутентификация, но не из-за таймаута.
//    ASSOC_FAIL	203  reserved	Код reason Wi-Fi, специфичный для Espressif: ассоциация была неудачной, но не по причине ASSOC_EXPIRE или ASSOC_TOOMANY.
//    HANDSHAKE_TIMEOUT	204  reserved	Код reason Wi-Fi, специфичный для Espressif: неудача handshake для той же reason, как в случае WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT.
//    CONNECTION_FAIL	205  reserved	Код reason Wi-Fi, специфичный для Espressif: неудачное соединение с AP.
//
//*********************************************************************************************************************

///@brief  reason-codes, defined in ESP-IDF SDK
///@detail Decoding the WiFi error reason enum, return text message describe of it,
///        C++ implementation of this function
///@param [in] reason reason of the WiFi error
///@return ASCIIZ C-string with text name of error reason
const char* esp::wifi::err::reason(wifi_err_reason_t reasonner)
{
#define Rea_pfx(reason) WIFI_REASON_##reason
#define Rea_msg(reason) "WiFi Err Reason: " #reason
#define case_reason(reason) case Rea_pfx(reason):	return  #reason //Rea_msg(reason)

    switch (reasonner)
    {
    // Внутренняя ошибка,  например закончилась память, сбой внутреннего TX,
    // или reason принята от противоположной стороны.
    case_reason(UNSPECIFIED); /* 1 (1) */

    // Предыдущая аутентификация не является более допустимой.
    // Для станции ESP эта reason сообщается в случаях:
    //  - таймаут аутентификации.
    //  - причина получена от AP.
    // Для ESP AP, эта reason сообщается в случаях:
    //  - AP не приняла какие-либо пакеты от станции за последующие 5 минут.
    //  - AP остановлена вызовом esp_wifi_stop().
    //  - станция переведена в состояние de-authed (не аутентифицирована) вызовом esp_wifi_deauth_sta().
    case_reason(AUTH_EXPIRE); /* 2 (2) */

    // De-authenticated (не аутентифицирована),  потому что отправляющая станция отключается (или отключилась).
    // Для станции ESP эта reason сообщается в случае: она получена от AP.
    case_reason(AUTH_LEAVE); /* 3 (3) */

    // Отключено из-за неактивности.
    // Для станции ESP эта reason сообщается в случае: она получена от AP.
    // Для ESP AP эта причина сообщается в случаях:
    //  - AP не приняла какие-либо пакеты от станции в течение 5 минут.
    //  - AP остановлена вызовом esp_wifi_stop().
    //  - станция переведена в состояние de-authed вызовом esp_wifi_deauth_sta().
    case_reason(ASSOC_EXPIRE); /* 4 (4) */

    // Отключено, потому что AP не может обработать все текущие ассоциированные STA одновременно.
    // Для станции ESP эта reason сообщается в случае: она получена от AP.
    // Для ESP AP эта reason сообщается в случае: станции, ассоциированные с AP, достигли максимального количества,
    //  которое может поддержать AP.
    case_reason(ASSOC_TOOMANY); /* 5 (5) */

    // От не аутентифицированной станции принят кадр Class-2.
    // Для станции ESP эта reason сообщается в случае: она получена от AP.
    // Для ESP AP эта reason сообщается в случае: AP получила пакет с данными от не аутентифицированной станции.
    case_reason(NOT_AUTHED); /* 6 (6) */

    // От не ассоциированной станции принят кадр Class-3.
    // Для станции ESP эта reason сообщается в случае: она получена от AP.
    // Для ESP AP эта reason сообщается в случае: AP получила пакет с данными от не ассоциированной станции.
    case_reason(NOT_ASSOCED); /* 7 (7) */

    // Отключено, потому что отправляющая станция оставляет (или оставила) BSS.
    // Для станции ESP, эта reason сообщается в случае:
    //  - она принята от AP.
    //  - станция отключена через esp_wifi_disconnect() и другое API.
    case_reason(ASSOC_LEAVE); /* 8 (8) */

    // Станция, запрашивающая (повторную) ассоциацию не аутентифицирована отвечающей STA.
    // Для станции ESP, эта reason сообщается в случае:
    //  - она принята от AP.
    // Для ESP AP эта reason сообщается в случае:
    //  - AP приняла пакеты с данными от ассоциированной, но не аутентифицированной станции.
    case_reason(ASSOC_NOT_AUTHED); /* 9 (9) */

    // Отключено, потому что недопустима информация в элементе Power Capability.
    // Для станции ESP эта reason сообщается в случае: она получена от AP.
    case_reason(DISASSOC_PWRCAP_BAD); /* 10 (10) */

    // Отключено, потому что недопустима информация в элементе Supported Channels.
    // Для станции ESP, эта reason сообщается в случае: она принята от AP.
    case_reason(DISASSOC_SUPCHAN_BAD); /* 11 (11) */

    // Any Reason code
    case_reason(BSS_TRANSITION_DISASSOC); /* 12 */;

    // Недопустимый элемент, например элемент, содержимое которого не удовлетворяет спецификациями стандарта
    //   в пункте формата кадра.
    // Для станции ESP, эта reason сообщается в случае: она принята от AP.
    // Для ESP AP, эта reason сообщается в случае: AP обработано неправильный WPA или RSN IE.
    case_reason(IE_INVALID); /* 13 (13) */

    // Ошибка кода целостности сообщения (MIC).
    // Для станции ESP эта reason сообщается, в случае: она принята от AP.
    case_reason(MIC_FAILURE); /* 14 (14) */

    // Таймаут процесса four-way handshake.
    // По соображениям совместимости в ESP код reason заменен на WIFI_REASON_HANDSHAKE_TIMEOUT.
    // Для станции ESP эта reason сообщается в случае:
    //  - таймаут handshake.
    //  - она принята от AP.
    case_reason(4WAY_HANDSHAKE_TIMEOUT); /* 15 (15) */

    // Таймаут Group-Key Handshake.
    // Для станции ESP эта reason сообщается в случае: она принята от AP.
    case_reason(GROUP_KEY_UPDATE_TIMEOUT); /* 16 (16) */

    // Элемент в four-way handshake отличается от кадра (Re-)Association Request/Probe и Response/Beacon frame.
    // Для станции ESP эта reason сообщается в случае:
    //  - она принята от AP.
    //  - станция обнаружила, что four-way handshake IE отличается от IE в кадре (Re-)Association Request/Probe и Response/Beacon.
    case_reason(IE_IN_4WAY_DIFFERS); /* 17 (17) */

    // Invalid group cipher.
    // Для станции ESP эта reason сообщается в случае: она принята от AP.
    case_reason(GROUP_CIPHER_INVALID); /* 18 (18) */

    // Недопустимый pairwise cipher.
    // Для станции ESP эта reason сообщается в случае: она принята от AP.
    case_reason(PAIRWISE_CIPHER_INVALID); /* 19 (19) */

    // Недопустимый AKMP.
    // Для станции ESP эта reason сообщается в случае: - она принята от AP.
    case_reason(AKMP_INVALID); /* 20 (20) */

    // Не поддерживаемая версия RSNE.
    // Для станции ESP эта reason сообщается в случае: она принята от AP.
    case_reason(UNSUPP_RSN_IE_VERSION); /* 21 (21) */

    // Недопустимые возможности RSNE.
    // Для станции ESP эта reason сообщается в случае: она принята от AP.
    case_reason(INVALID_RSN_IE_CAP); /* 22 (22) */

    // Ошибка аутентификации IEEE 802.1X.
    // Для станции ESP эта reason сообщается в случае: она принята от AP.
    // Для ESP AP эта reason сообщается в случае: ошибка аутентификации IEEE 802.1X.
    case_reason(802_1X_AUTH_FAILED); /* 23 (23) */

    // Подсистема шифра отклонена политиками безопасности.
    // Для станции ESP эта reason сообщается в случае: она принята от AP.
    case_reason(CIPHER_SUITE_REJECTED); /* 24 (24) */

    // Разрушение TDLS direct-link из-за того, что недоступен TDLS peer STA через прямое соединение TDLS.
    case_reason(TDLS_PEER_UNREACHABLE); /* 25 (25) */

    // Разрушение TDLS direct-link по неуказанной причине.
    case_reason(TDLS_UNSPECIFIED); /* 26 (26) */

    // Отключено, потому что сессия прервана запросом SSP.
    case_reason(SSP_REQUESTED_DISASSOC); /* 27 (27) */

    // Отключено из-за отсутствия соглашения SSP roaming.
    case_reason(NO_SSP_ROAMING_AGREEMENT); /* 28 (28) */

    // Запрошенная служба отклонена из-за требований подсистемы чипера SSP или AKM requirement.
    case_reason(BAD_CIPHER_OR_AKM); /* 29 (29) */

    // Запрошенная служба не авторизована в этом месте.
    case_reason(NOT_AUTHORIZED_THIS_LOCATION); /* 30 (30) */

    // TS удалено, потому что у QoS AP отсутствует достаточная полоса для этой QoS STA из-за изменений характеристик
    //	службы BSS или рабочего режима (например HT BSS поменялось от канала 40 МГц на канал 20 МГц).
    case_reason(SERVICE_CHANGE_PERCLUDES_TS); /* 31 (31) */

    // Отключение по не указанной, связанной с QoS причиной.
    case_reason(UNSPECIFIED_QOS); /* 32 (32) */

    // Отключено, потому что у QoS AP отсутствует достаточная полоса для этого QoS STA.
    case_reason(NOT_ENOUGH_BANDWIDTH); /* 33 (33) */

    // Отключено, потому что должно быть подтверждено слишком большое количество кадров, они не подтверждены
    //	из-за передач AP и/или плохих условий канала.
    case_reason(MISSING_ACKS); /* 34 (34) */

    // Отключено, потому что STA передает вне предела её TXOP.
    case_reason(EXCEEDED_TXOP); /* 35 (35) */

    // Запрашивающая STA покинула BSS (или сброшена).
    case_reason(STA_LEAVING); /* 36 (36) */

    // Запрашивающая STA не использует больше поток или сессию.
    case_reason(END_BA); /* 37 (37) */

    // Запрашивающая STA приняла кадры с использованием механизма, для которого не была завершена настройка.
    case_reason(UNKNOWN_BA); /* 38 (38) */

    // Таймаут запроса пира STA.
    case_reason(TIMEOUT); /* 39 (39) */

    //    Reserved		40..45	40..45	----

    // В отключенном кадре: отключено, потому что достигнут предел авторизованного доступа.
    case_reason(PEER_INITIATED); /* 46 (46) */

    // В отключенном кадре: отключена из-за требований внешней службы.
    case_reason(AP_INITIATED); /* 47 (47) */

    // Недопустимый счетчик кадров FT Action.
    case_reason(INVALID_FT_ACTION_FRAME_COUNT); /* 48 (48) */

    // Недопустимый идентификатор парного мастер-ключа (PMKID).
    case_reason(INVALID_PMKID); /* 49 (49) */

    // Недопустимый MDE.
    case_reason(INVALID_MDE); /* 50 (50) */

    // Недопустимый FTE.
    case_reason(INVALID_FTE); /* 51 (51) */

    // Неудача установки линка передачи на альтернативном канале.
    case_reason(TRANSMISSION_LINK_ESTABLISH_FAILED); /* 67 (67) */

    // Альтернативный канал занят.
    case_reason(ALTERATIVE_CHANNEL_OCCUPIED); /* 68 (68) */

    // Код reason Wi-Fi, специфичный для Espressif: когда станция потеряла N непрерывных beacon-ов,
    // она разрушает соединение и сообщает эту reason.
    case_reason(BEACON_TIMEOUT); /* 200 (reserved) */

    // Код reason Wi-Fi, специфичный для Espressif: когда станция не смогла просканировать целевую AP.
    case_reason(NO_AP_FOUND); /* 201 (reserved) */

    // Код reason Wi-Fi, специфичный для Espressif: неудачная аутентификация, но не из-за таймаута.
    case_reason(AUTH_FAIL); /* 202 (reserved) */

    // Код reason Wi-Fi, специфичный для Espressif: ассоциация была неудачной, но не по причине ASSOC_EXPIRE или ASSOC_TOOMANY.
    case_reason(ASSOC_FAIL); /* 203 (reserved) */

    // Код reason Wi-Fi, специфичный для Espressif: неудача handshake для той же reason, как в случае WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT.
    case_reason(HANDSHAKE_TIMEOUT); /* 204 (reserved) */

    // Код reason Wi-Fi, специфичный для Espressif: неудачное соединение с AP.
    case_reason(CONNECTION_FAIL); /* 205 (reserved) */

    // Код reason Wi-Fi, специфичный для Espressif: ???
    case_reason(AP_TSF_RESET); /* 206 */;
    // Код reason Wi-Fi, специфичный для Espressif: ???
    case_reason(ROAMING); /* 207 */;
    // Код reason Wi-Fi, специфичный для Espressif: ???
    case_reason(ASSOC_COMEBACK_TIME_TOO_LONG); /* 208 */
    // Код reason Wi-Fi, специфичный для Espressif: ???
    case_reason(SA_QUERY_TIMEOUT); /* 209 */

    default:
	return "Unknown_code";

    }; /* switch reason */
    return "";
}; /* esp::wifi::err::reason() */

///@brief  reason-codes, defined in ESP-IDF SDK with uint8_t argument
///@param [in] reason reason of the WiFi error
///@return ASCIIZ C-string with text name of error reason
const char* esp::wifi::err::reason(uint8_t areason) {
    return reason(static_cast<wifi_err_reason_t>(areason));
}; /* esp::wifi::err::reason(uint8_t) */





//--[ class net::wifi::sta::netif_t::Updater ]-----------------------------------------------------


/// @brief Constructor for the class net::wifi::sta::netif_t::Updater
//net::wifi::sta::netif_t::Updater::Updater(netif_t *netif):
esp::net::wifi::Updater::Updater(esp::wifi::netif_t *netif):
	its_netif(*netif)
{
    ESP_LOGW(__FUNCTION__, "    ####  Entering ti the Updater construction  ####");

}; /* esp::net::wifi::Updater::Updater() */


/** @brief Preliliminary Set status of request to the dhcp-client - request start/stop after the connection
 *  @param[in]   dhcp_st  - needed DHCP client run status
 *  @return ESP_OK        - success updating configuration */
//esp_err_t net::wifi::sta::netif_t::Updater::dhcp(bool dhcp_st)
esp_err_t esp::net::wifi::Updater::dhcp_adv(bool dhcp_st)
{
    ESP_LOGI("NAMI Demo", "Preliminary request to Updating DHCP state...");

    its_netif.dhcp.client.ackuire(dhcp_st);	//< set flag deferred DHCP client start/stop accoding the dhcp_st value

    if (dhcp_st)
	ESP_LOGI("NAMI Tester Demo", "DHCP Enabled, obtain the network ip-parameters from DHCP-server");

    if (!dhcp_st)
	ESP_LOGI("NAMI Tester Demo", "DHCP Disabled, direct setup network ip-configuration");

    return (err = ESP_OK);
}; /* esp::net::wifi::Updater::dhcp() */


/** @brief Actually statr/stop dhcp-client manually - according requested status for it
 *  @return ESP_OK        - success updating configuration */
esp_err_t esp::net::wifi::Updater::dhcp_do()
{

    ESP_LOGI("NAMI Demo", "====== Update DHCP requested state...");

    //XXX Remove after needed expired
    ESP_LOGI(__func__, "Requested dhcp status is .......... : [ %s ]", its_netif.dhcp.client.ackuired()? "Yes": "No");
    ESP_LOGI(__func__, "Netif DHCP client is started? ..... : [ %s ]", its_netif.dhcp.client.started()? "Yes": "No");
    ESP_LOGI(__func__, "Netif DHCP client is enabled? ..... : [ %s ]\n", its_netif.dhcp.client.enabled()? "Yes": "No");

    if (its_netif.dhcp.client.ackuired())
    {
	ESP_LOGI("NAMI Tester Demo", "DHCP Enabled, obtain the network ip-parameters from DHCP-server");
	if (!its_netif.dhcp.client.started())
	{	ESP_LOGI("DHCP Client stopped", "Start the DHCP Client");
	    err = its_netif.dhcp.client.start();
	    ESP_LOGI("Starting of the DHCP Client", "*** returned code: %i", err);
	}; /* if !its_netif.dhcp.client.started() */
    }; /* its_netif.dhcp.client.ackuired() */

    if (!its_netif.dhcp.client.ackuired())
    {
	ESP_LOGI("NAMI Tester Demo", "DHCP Disabled, direct setup network ip-configuration");
	if (its_netif.dhcp.client.started())
	{	ESP_LOGI("DHCP Client started", "Stop the DHCP Client");
	    err = its_netif.dhcp.client.stop();
	    ESP_LOGI("Stopping of the DHCP Client", "*** returned code: %i", err);
	}; /* if its_netif.dhcp.client.started() */
    }; /* if !its_netif.dhcp.client.ackuired() */

    //XXX Remove after needed expired
    ESP_LOGI("NAMI Tester Demo", "###### Statuses after actual updating ip configuration ######\n");

    ESP_LOGI(__func__, "Requested DHCP status is ......... : [ %s ]", its_netif.dhcp.client.ackuired()? "Yes": "No");
    ESP_LOGI(__func__, "Netif DHCP client is started? .... : [ %s ]", its_netif.dhcp.client.started()? "Yes": "No");
    ESP_LOGI(__func__, "Netif DHCP client is enabled? .... : [ %s ]\n", its_netif.dhcp.client.enabled()? "Yes": "No");

    return err;
}; /* esp::net::wifi::Updater::dhcp_do() */

static inline
std::string curr_login(const esp::net::wifi::config_t& cfg/*, esp::wifi::netif_t& netif*/)
{
    //esp::net::wifi::stack::configuration(netif.type);
//    netif.type;
    switch (cfg.getype())
    {
    case WIFI_IF_AP:
	return reinterpret_cast<const char*>(cfg.get().ap.ssid);

    case WIFI_IF_STA:
    default:
	return reinterpret_cast<const char*>(cfg.get().sta.ssid);
    }; /* switch cfg.getype() */
}; /* curr_login() */

static inline
std::string curr_passwd(const esp::net::wifi::config_t& cfg)
{
    switch (cfg.getype())
    {
    case WIFI_IF_AP:
	return reinterpret_cast<const char*>(cfg.get().ap.password);

    case WIFI_IF_STA:
    default:
	return reinterpret_cast<const char*>(cfg.get().sta.password);
    }; /* switch cfg.getype() */
}; /* curr_login() */


/** @brief Save current network configuration for bacup
 *  @return ESP_OK	  - success updating configuration
 *	ESP_ERR		  - any error
 *	ESP_ERR_NO_MEM	  - insufficient memory for backup */
esp_err_t esp::net::wifi::Updater::backup()
{
    if (wifibkp == nullptr)
    {
	wifibkp = new ::net::configuration_t(its_netif.dhcp.client.ackuired(), its_netif.cfg.get(),
		curr_login(stack::configuration(its_netif.type)), curr_passwd(stack::configuration(its_netif.type)));
	if (wifibkp)
	    err = ESP_OK;
	else
	{
	    ESP_LOGE(__PRETTY_FUNCTION__, "Insufficient memory for backup network configuration");
	    err = ESP_ERR_NO_MEM;
	    return err;
	}; /* else if wifibackup == nullptr */
    }; /* if wifibackup == nullptr */

    //XXX: Remove after debug
    ESP_LOGW(__PRETTY_FUNCTION__, "##### Bacuped WiFi IP cfg is:");
        ESP_LOGW(__FUNCTION__, "~~~~~~~~~~~");
//        ESP_LOGW(__func__, "IP     : " IPSTR, IP2STR(&ipbackup.ip) );
//        ESP_LOGW(__func__, "IP     : " IPSTR, IP2STR(&(static_cast<const esp_ip4_addr_t&>(wifibkp->ip))) );
        ESP_LOGW(__func__, "IP      : %s", static_cast<const char*>(wifibkp->ip));
//        ESP_LOGI(__func__, "Mask   : " IPSTR, IP2STR(&ipbackup.netmask));
        ESP_LOGI(__func__, "Mask    : %s", static_cast<const char*>(wifibkp->mask));
//        ESP_LOGI(__func__, "Gateway: " IPSTR, IP2STR(&ipbackup.gw));
        ESP_LOGI(__func__, "Gateway : %s", static_cast<const char*>(wifibkp->gate));
//        ESP_LOGI(__func__, "DHCP is: %s", ipbackup? "Enabled": "Disabled");
        ESP_LOGI(__func__, "DHCP is : %s", wifibkp->dhcp? "Enabled": "Disabled");
        ESP_LOGI(__func__, "Password  %s", wifibkp->passwd? "Used": "Unused");
        ESP_LOGI(__func__, "Login is: %s", wifibkp->login.c_str());
        ESP_LOGI(__func__, "Password: %s", wifibkp->passwd.c_str());
        ESP_LOGW(__FUNCTION__, "~~~~~~~~~~~");

    return ESP_OK;
}; /* esp::net::wifi::Updater::backup() */


/** @brief Restore previously saved network configuration from backup
 *  @return ESP_OK	  - success updating configuration
 *	ESP_ERR		  - any error
 *	ESP_ERR_NO_MEM	  - backup buffer is absent, it's not found */
esp_err_t esp::net::wifi::Updater::revert()
{
    ESP_LOGW(__PRETTY_FUNCTION__, "###-- Error updating new network parameters, restore it's from backup --###");
    return invoke(*wifibkp, true);
}; /* esp::net::wifi::Updater::revert() */

/** @brief Finalize updating procedure - clearing the backup buffer */
void esp::net::wifi::Updater::finalize()
{
    ESP_LOGI(__PRETTY_FUNCTION__, "##### Clear the Backup buffer #####");
    delete wifibkp;
    wifibkp = nullptr;
}; /* esp::net::wifi::Updater::finalize() */



/** @brief Set the ip cfg - start/stop dhcp-client, set or get ip address, mask and gateway, alias for class net::configuration
 *  @param[in]   cfg      - new parameters network configuration buffer
 *  @return ESP_OK        - success updating configuration */
esp_err_t esp::net::wifi::Updater::ip(const ::net::configuration_t& cfg)
{
    dhcp_adv(cfg.dhcp);
    return ip(*cfg.ipdata());
}; /* esp::net::wifi::Updater::ip(net::configuration&) */

/** @brief Set the ip cfg - start/stop dhcp-client, set or get ip address, mask and gateway
 *  @param[in]   dhcp_st  - DHCP client run status
 *  @param[in]   inf      - new ip parameters network configuration
 *  @return ESP_OK        - success updating configuration */
esp_err_t esp::net::wifi::Updater::ip(const esp::ip4::info& info)
{
    ESP_LOGI("NAMI Demo", "Update IP configuration...");

    if (!its_netif.dhcp.client.ackuired())
	its_netif.set_ip(info);

    return (err = ESP_OK);
}; /* esp::net::wifi::Updater::ip(bool, const esp::ip4::info&) */



/** @brief Apply the login cfg: wifi cfg - ssid, password - to the netif
 *  @param[in]   cfg      - new parameters network configuration buffer
 *  @return ESP_OK        - success updating configuration */
esp_err_t esp::net::wifi::Updater::login(const ::net::configuration_t& cfg)
{
	ESP_LOGW(__FUNCTION__, "##### WiFi Login Updater with (const net::configuration& cfg) parameter #####");
    return login(config_t(its_netif.type, cfg));
}; /* esp::net::wifi::Updater::login(const net::configuration&) */


/** @brief Apply the login cfg: wifi cfg - ssid, password - to the netif
 *  @param[in]   cfg      - new parameters network configuration buffer
 *  @return ESP_OK        - success updating configuration */
esp_err_t esp::net::wifi::Updater::login(const esp::net::wifi::config_t& cfg)
{
	ESP_LOGW(__FUNCTION__, "##### WiFi Login Updater with (const esp::net::wifi::config_t& cfg) parameter #####");

	esp::net::wifi::config_t conf;

    conf = stack::configuration(its_netif.type);
    /// set ssid & passd
    conf.ssid(cfg.ssid());
    conf.passwd(cfg.passwd());

    /// set failure retry counter to
    uint8_t tmp = conf.retry(CONFIG_WIFI_STA_MAXIMUM_RETRY);
    ESP_LOGW(__func__, "======= New value of data.sta.failure_retry_cnt is %u, prev value is %u, real current value is %u", CONFIG_WIFI_STA_MAXIMUM_RETRY, tmp, conf.retry());

    ESP_LOGW(__PRETTY_FUNCTION__, "====>> New  SSID  for connecting to AP is: %s", conf.ssid_cstr());
    ESP_LOGW(__PRETTY_FUNCTION__, "====>> New Passwd for connecting to AP is: %s", conf.passwd_cstr());

    /*esp::net::wifi::*/stack::configure(conf);

    return (err = ESP_OK);
}; /* esp::net::wifi::Updater::login() */




/** @brief simply execute connection to the selected WiFi AP
 *  @param[in]   cfg      - new network configuration buffer
 *  @return               - passed the 'err' value */
esp_err_t esp::net::wifi::Updater::connect(const ::net::configuration_t& cfg)
{

	static event::sync::stat reconnect(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED);
	event::ctrl<reconnect>::automatic autorecon;

    ESP_LOGW(__FUNCTION__, ">> wifi disconnect ");
    /// wifi disconnect
    esp::net::wifi::stack::disconnect();

    ESP_LOGW(__FUNCTION__, ">> update wifi login configuration ");
    login(cfg);
    ESP_LOGW(__func__, "==>>> sta::cfg::login::update() return the %d error code", err);

    //ESP_ERROR_CHECK(event::ctrl<reconnect>::enroll());
    ESP_ERROR_CHECK(autorecon.enroll());

    ESP_LOGW(__FUNCTION__, ">> connect to the new WiFi AP...");
    err = stack::connect();


    if(reconnect.wait.Take(secticks(CONFIG_WIFI_STA_WAITING_CONNECT)) != pdTRUE)
    {   // Semaphore broken by timeout expired

	ESP_LOGW(__FUNCTION__, "Wrong new WiFi SSID or Password.");
	ESP_LOGW(__FUNCTION__, "Need Rollback to old config.");
	// Rollback WiFi configuration
	// Rolback network WiFi ip config
	// Reconnect
//	    ESP_LOGW(__func__, "=== UnRegister WiFi connection handler, WiFi Station configuration reverted ===");
//	    ESP_ERROR_CHECK(reconnect.ctrl.unreg());
	//ESP_ERROR_CHECK(event::ctrl<reconnect>::unreg());
	return (err = ESP_ERR_WIFI_PASSWORD);
    }; /* if xSemaphoreTake( s_semph_wait_wifi_connect, <timeout> ) != pdTRUE */
    ESP_LOGI("Updating connection", "Wi-Fi Station connected, release waiting WiFi connection semaphore & restart DHCP client, if needed...");

    ESP_LOGW(__func__, "=== UnRegister WiFi connection handler, sucсess WiFi connection ===");
    //ESP_ERROR_CHECK(event::ctrl<reconnect>::unreg());
    ESP_LOGI(__func__, "Exit from esp::net::wifi::stack::connect() with return code: %i", err);

    return err;
}; /* esp::net::wifi::Updater::connect() */



/** @brief Invoke immediately update WiFi cfg procedure: WiFi connect + netif config
 *	    only return code without any restoring/rollback configurations
 *  @param[in]   cfg       - new network configuration for updating
 *  @param[in]   forcecon - is need reconnecting djuring updating network parameters
 * @return
 *	ESP_OK - Setup configuration successfully
 *	other  - as in operator()(::net::configuration& cfg), see late
 */
esp_err_t esp::net::wifi::Updater::invoke(::net::configuration_t& cfg, bool forcecon)
{
	static event::sync::stat on_got_ip(IP_EVENT, IP_EVENT_STA_GOT_IP);
	event::ctrl<on_got_ip>::automatic auto_ip;


    ESP_LOGW(__PRETTY_FUNCTION__, "> !!! Perform updating WiFi station configuration");

    ESP_LOGI(__FUNCTION__, "Register event handler for: IP_EVENT, IP_EVENT_STA_GOT_IP");
    ESP_ERROR_CHECK(auto_ip.enroll());

    dhcp_adv(cfg.dhcp);

    if (forcecon || cfg.login_changed())
    {
	connect(cfg);
	if (err != ESP_OK)
	{
	    ESP_LOGE(__FUNCTION__, "New connection failed with error status %i", err);
	    return err;
	}; /* if err != ESP_OK */

    }; /* if forcecon cfg.login_changed() */


    ESP_LOGI("NAMI C++ Demo", "###### DHCP Statuses during update procedure ######");


    //XXX Remove after needed expired
    ESP_LOGI(__func__, "Shadow configuration DHCP status is: [ %s ]", cfg.dhcp? "Yes": "No");
    ESP_LOGI(__func__, "Netif DHCP client is started? .... : [ %s ]", its_netif.dhcp.client.started()? "Yes": "No");
    ESP_LOGI(__func__, "Netif DHCP client is enabled? .... : [ %s ]", its_netif.dhcp.client.enabled()? "Yes": "No");

    dhcp_do();
    ip(cfg);

    if (on_got_ip.wait.Take(secticks(CONFIG_WIFI_STA_WAITING_IP)) != pdTRUE)
    {   // Semaphore broken by timeout expired
	ESP_LOGW(__FUNCTION__, "Wrong IP-configuration, but the new WiFi SSID & Password are correct.");
	ESP_LOGW(__FUNCTION__, "Rollback to old config.");
	// Rollback WiFi configuration
	// Rolback network WiFi ip config
	// Reconnect
	ESP_LOGW(__func__, "=== UnRegister WiFi Station ip given handler, WiFi Station configuration reverted ===");

	return (err = ESP_ERR_WIFI_PASSWORD);
//	return (err = ESP_ERR_WIFI_IF);
    }; /* if xSemaphoreTake( s_semph_wait_wifi_connect, <timeout> ) != pdTRUE */

    ESP_LOGW(__func__, "=== UnRegister WiFi Station ip given handler, WiFi station ip cfg sucessfully changed ===");
    return (err = ESP_OK);

}; /* esp::net::wifi::Updater::invoke() */

/** @brief Full update procedure of the WiFi configuration: WiFi connect + netif config & restart netif, if needed
 * @return
 *	ESP_OK - Setup configuration successfully
 *	ESP_ERR_NOT_FOUND     - network cfg is not changed, nothing to do
 *	ESP_ERR_INVALID_RESPONSE
 *	ESP_ERR_WIFI_SSID     - invalid SSID (not connected?)
 *	ESP_ERR_WIFI_PASSWORD - invalid the WiFi password
 *	ESP_ERR_WIFI_TIMEOUT  - timeout(???)
 *	ESP_ERR_WIFI_IF	      - invalid ip cfg netif params
 * events:
 *	WIFI_EVENT_WIFI_READY - ESP32 station ready
 *	WIFI_EVENT_STA_START  - ESP32 station start
 *	WIFI_EVENT_STA_STOP   - ESP32 station stop
 *	WIFI_EVENT_STA_CONNECTED    - ESP32 station connected
 *	WIFI_EVENT_STA_DISCONNECTED - ESP32 station disconnected
 *	WIFI_EVENT_MAX	      - Invalid WiFi event ID
 *
 */
esp_err_t esp::net::wifi::Updater::operator()(::net::configuration_t& cfg)
{

	static event::sync::stat on_got_ip(IP_EVENT, IP_EVENT_STA_GOT_IP);
	event::ctrl<on_got_ip>::automatic auto_ip;

    ESP_LOGW(__PRETTY_FUNCTION__, "> Setup new WiFi station configuration");

    err = ESP_OK;

    if (!cfg.ip_changed())
    {
	ESP_LOGW(__func__, "# WiFi station netif configuration was not changed");
	if (!cfg.login_changed())
	{
	    ESP_LOGW(__func__, "# WiFi config of station was not changed - nothong to do");
	    err = ESP_ERR_NOT_FOUND;
	    return err;
	}; /* if !cfg.wifi_changed() */
    }; /* if !cfg.netif_changed() */

    ESP_LOGW(__func__, ">> Configuration of WiFi station or it's netif is changed");

    backup();

    if (err != ESP_OK)
    {
	ESP_LOGW(__FUNCTION__, "Fail backup network configuration with error code %i", err);
	return err;
    }; /* if backup() != ESP_OK */

    invoke(cfg);
    if (err != ESP_OK)
    {
	    esp_err_t savederr = err;
	ESP_LOGW(__FUNCTION__, "Fail updating network configuration with error code %i, reverted it", err);
	revert();
	ESP_LOGW(__FUNCTION__, "Revert network configuration with error code %i", err);
	err = savederr;

    }; /* if err != ESP_OK */
    /*
    sta::save();
    sta::config.clr_chgst();
    return stream.status();
    */
    return err;

}; /* esp::net::wifi::Updater::operator() */



//--[ wifi.cpp ]-------------------------------------------------------------------------------------------------------
