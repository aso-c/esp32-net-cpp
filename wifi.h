/*
 * @file
 * wifi.h
 *
 * @brief General backend definition for wiFi network:
 * @brief C++ abstraction layer of the WiFi ESP API procedures
 *
 * @warning May be only inner definitions of the 'net' component
 *
 * This code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 *  software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 *  CONDITIONS OF ANY KIND, either express or implied.
 *
 * @date    Created on: 12 окт. 2023 г.
 * @author  aso
 */

#ifndef _WIFI_H_
#define _WIFI_H_

#ifdef __cplusplus
extern "C" {
#endif



    ///*********************************************************************************************************************
    ///
    ///@detail Decoding the WiFi error reason enum, return text message describe of it,
    ///        pure C variant of this function
    /// Таблица значений reason-code (коды причины), определенные в библиотеке ESP32-C3.
    /// - Первый столбец - имя макроса, определенное в esp_wifi_types.h.
    ///     Общий префикс WIFI_REASON из имен удален, т. е. UNSPECIFIED -> реально WIFI_REASON_UNSPECIFIED, и т.д.
    /// - Второй столбец -  значение причины.
    /// - Третий столбец - стандартное значение, которое отображается как reason (причина) в секции 8.4.1.7 стандарта IEEE 802.11-2012
    ///     (для дополнительной информации см. этот стандарт).
    /// - Последний столбец - описание reason (причины).
    ///
    ///	Reason code	Значен.	Отобр.	Описание
    ///    UNSPECIFIED	1	1	Обычно это означает внутреннюю ошибку,  например закончилась память, сбой внутреннего TX, или reason принята от противоположной стороны.
    ///    AUTH_EXPIRE	2	2	Предыдущая аутентификация не является более допустимой. Для станции ESP эта reason сообщается в случаях:
    ///					    - таймаут аутентификации.
    ///					    - причина получена от AP.
    ///					    Для ESP AP, эта reason сообщается в случаях:
    ///					    - AP не приняла какие-либо пакеты от станции за последующие 5 минут.
    ///					    - AP остановлена вызовом esp_wifi_stop().
    ///					    - станция переведена в состояние de-authed (не аутентифицирована) вызовом esp_wifi_deauth_sta().
    ///    AUTH_LEAVE	3	3	De-authenticated (не аутентифицирована),  потому что отправляющая станция отключается (или отключилась).
    ///					    Для станции ESP эта reason сообщается в случае: она получена от AP.
    ///    ASSOC_EXPIRE	4	4	Отключено из-за неактивности.
    ///					    Для станции ESP эта reason сообщается в случае: она получена от AP.
    ///					    Для ESP AP эта причина сообщается в случаях:
    ///					    - AP не приняла какие-либо пакеты от станции в течение 5 минут.
    ///					    - AP остановлена вызовом esp_wifi_stop().
    ///					    - станция переведена в состояние de-authed вызовом esp_wifi_deauth_sta().
    ///    ASSOC_TOOMANY	5	5	Отключено, потому что AP не может обработать все текущие ассоциированные STA одновременно.
    ///					    Для станции ESP эта reason сообщается в случае: она получена от AP.
    ///					    Для ESP AP эта reason сообщается в случае: станции, ассоциированные с AP, достигли максимального количества, которое может поддержать AP.
    ///    NOT_AUTHED	6	6	От не аутентифицированной станции принят кадр Class-2.
    ///					    Для станции ESP эта reason сообщается в случае: она получена от AP.
    ///					    Для ESP AP эта reason сообщается в случае: AP получила пакет с данными от не аутентифицированной станции.
    ///    NOT_ASSOCED	7	7	От не ассоциированной станции принят кадр Class-3.
    ///					    Для станции ESP эта reason сообщается в случае: она получена от AP.
    ///					    Для ESP AP эта reason сообщается в случае: AP получила пакет с данными от не ассоциированной станции.
    ///    ASSOC_LEAVE	8	8	Отключено, потому что отправляющая станция оставляет (или оставила) BSS.
    ///					    Для станции ESP, эта reason сообщается в случае:
    ///					    - она принята от AP.
    ///					    - станция отключена через esp_wifi_disconnect() и другое API.
    ///    ASSOC_NOT_AUTHED	9	9	Станция, запрашивающая (повторную) ассоциацию не аутентифицирована отвечающей STA.
    ///					    Для станции ESP, эта reason сообщается в случае:
    ///					    - она принята от AP.
    ///					    Для ESP AP эта reason сообщается в случае:
    ///					    - AP приняла пакеты с данными от ассоциированной, но не аутентифицированной станции.
    ///  DISASSOC_PWRCAP_BAD	10	10	Отключено, потому что недопустима информация в элементе Power Capability.
    ///					    Для станции ESP эта reason сообщается в случае: она получена от AP.
    /// DISASSOC_SUPCHAN_BAD	11	11	Отключено, потому что недопустима информация в элементе Supported Channels.
    ///					    Для станции ESP, эта reason сообщается в случае: она принята от AP.
    ///    IE_INVALID	13	13	Недопустимый элемент, например элемент, содержимое которого не удовлетворяет спецификациями стандарта в пункте формата кадра.
    ///					    Для станции ESP, эта reason сообщается в случае: она принята от AP.
    ///					    Для ESP AP, эта reason сообщается в случае: AP обработано неправильный WPA или RSN IE.
    ///    MIC_FAILURE	14	14	Ошибка кода целостности сообщения (MIC).
    ///					    Для станции ESP эта reason сообщается, в случае: она принята от AP.
    /// 4WAY_HANDSHAKE_TIMEOUT 15	15	Таймаут процесса four-way handshake.
    ///					По соображениям совместимости в ESP код reason заменен на WIFI_REASON_HANDSHAKE_TIMEOUT.
    ///					   Для станции ESP эта reason сообщается в случае:
    ///					    - таймаут handshake.
    ///					    - она принята от AP.
    /// GROUP_KEY_UPDATE_TIMEOUT 16	16	Таймаут Group-Key Handshake.
    ///					    Для станции ESP эта reason сообщается в случае: она принята от AP.
    ///   IE_IN_4WAY_DIFFERS	17	17	Элемент в four-way handshake отличается от кадра (Re-)Association Request/Probe и Response/Beacon frame.
    ///					    Для станции ESP эта reason сообщается в случае:
    ///					    - она принята от AP.
    ///					    - станция обнаружила, что four-way handshake IE отличается от IE в кадре (Re-)Association Request/Probe и Response/Beacon.
    ///  GROUP_CIPHER_INVALID  18	18	Invalid group cipher.
    ///					    Для станции ESP эта reason сообщается в случае: она принята от AP.
    ///  PAIRWISE_CIPHER_INVALID 19	19	Недопустимый pairwise cipher.
    ///					    Для станции ESP эта reason сообщается в случае: она принята от AP.
    ///    AKMP_INVALID	20	20	Недопустимый AKMP.
    ///					    Для станции ESP эта reason сообщается в случае: - она принята от AP.
    ///  UNSUPP_RSN_IE_VERSION  21	21	Не поддерживаемая версия RSNE.
    ///					    Для станции ESP эта reason сообщается в случае: она принята от AP.
    ///    INVALID_RSN_IE_CAP  22	22	Недопустимые возможности RSNE.
    ///					    Для станции ESP эта reason сообщается в случае: она принята от AP.
    ///    802_1X_AUTH_FAILED  23	23	Ошибка аутентификации IEEE 802.1X.
    ///					    Для станции ESP эта reason сообщается в случае: она принята от AP.
    ///					    Для ESP AP эта reason сообщается в случае: ошибка аутентификации IEEE 802.1X.
    ///  CIPHER_SUITE_REJECTED 24	24	Подсистема шифра отклонена политиками безопасности.
    ///					    Для станции ESP эта reason сообщается в случае: она принята от AP.
    ///  TDLS_PEER_UNREACHABLE 25	25	Разрушение TDLS direct-link из-за того, что недоступен TDLS peer STA через прямое соединение TDLS.
    ///  TDLS_UNSPECIFIED	26	26	Разрушение TDLS direct-link по неуказанной причине.
    ///  SSP_REQUESTED_DISASSOC  27	27	Отключено, потому что сессия прервана запросом SSP.
    ///  NO_SSP_ROAMING_AGREEMENT 28	28	Отключено из-за отсутствия соглашения SSP roaming.
    ///  BAD_CIPHER_OR_AKM	29	29	Запрошенная служба отклонена из-за требований подсистемы чипера SSP или AKM requirement.
    ///  NOT_AUTHORIZED_THIS_LOCATION 30  30	Запрошенная служба не авторизована в этом месте.
    ///  SERVICE_CHANGE_PERCLUDES_TS	 31  31	TS удалено, потому что у QoS AP отсутствует достаточная полоса для этой QoS STA из-за изменений характеристик службы BSS или рабочего режима
    ///					(например HT BSS поменялось от канала 40 МГц на канал 20 МГц).
    ///    UNSPECIFIED_QOS	32	32	Отключение по не указанной, связанной с QoS причиной.
    ///  NOT_ENOUGH_BANDWIDTH  33	33	Отключено, потому что у QoS AP отсутствует достаточная полоса для этого QoS STA.
    ///    MISSING_ACKS	34	34	Отключено, потому что должно быть подтверждено слишком большое количество кадров, они не подтверждены из-за передач AP и/или плохих условий канала.
    ///    EXCEEDED_TXOP	35	35	Отключено, потому что STA передает вне предела её TXOP.
    ///    STA_LEAVING	36	36	Запрашивающая STA покинула BSS (или сброшена).
    ///    END_BA		37	37	Запрашивающая STA не использует больше поток или сессию.
    ///    UNKNOWN_BA	38	38	Запрашивающая STA приняла кадры с использованием механизма, для которого не была завершена настройка.
    ///    TIMEOUT		39	39	Таймаут запроса пира STA.
    ///    Reserved		40..45	40..45	----
    ///    PEER_INITIATED	46	46	В отключенном кадре: отключено, потому что достигнут предел авторизованного доступа.
    ///    AP_INITIATED	47	47	В отключенном кадре: отключена из-за требований внешней службы.
    /// INVALID_FT_ACTION_FRAME_COUNT 48 48	Недопустимый счетчик кадров FT Action.
    ///    INVALID_PMKID	49	49	Недопустимый идентификатор парного мастер-ключа (PMKID).
    ///    INVALID_MDE	50	50	Недопустимый MDE.
    ///    INVALID_FTE	51	51	Недопустимый FTE.
    /// TRANSMISSION_LINK_ESTABLISH_FAILED  67  67	Неудача установки линка передачи на альтернативном канале.
    /// ALTERATIVE_CHANNEL_OCCUPIED  68  68	Альтернативный канал занят.
    ///    BEACON_TIMEOUT	200  reserved	Код reason Wi-Fi, специфичный для Espressif: когда станция потеряла N непрерывных beacon-ов, она разрушает соединение и сообщает эту reason.
    ///    NO_AP_FOUND	201  reserved	Код reason Wi-Fi, специфичный для Espressif: когда станция не смогла просканировать целевую AP.
    ///    AUTH_FAIL		202  reserved	Код reason Wi-Fi, специфичный для Espressif: неудачная аутентификация, но не из-за таймаута.
    ///    ASSOC_FAIL	203  reserved	Код reason Wi-Fi, специфичный для Espressif: ассоциация была неудачной, но не по причине ASSOC_EXPIRE или ASSOC_TOOMANY.
    ///    HANDSHAKE_TIMEOUT	204  reserved	Код reason Wi-Fi, специфичный для Espressif: неудача handshake для той же reason, как в случае WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT.
    ///    CONNECTION_FAIL	205  reserved	Код reason Wi-Fi, специфичный для Espressif: неудачное соединение с AP.
    ///
    ///*********************************************************************************************************************
    ///
    ///@brief  reason-codes, defined in ESP-IDF SDK
    ///
    ///@param [in] reason reason of the WiFi error
    ///@return ASCIIZ C-string with text name of error reason
    const char* wifi_err_reason(wifi_err_reason_t reason);

#ifdef __cplusplus
}


// namesopace for encapsulating of the esp system functions
namespace esp
{

    namespace wifi
    {
	class netif_t/*: public esp::netif_t*/;
    }; /* esp::wifi */

    namespace net
    {
	// namespace for encapsulating/naming the esp::wifi system calls
	namespace wifi
	{

	    class config_t;

	    /// @brief Class for updating configuration parameters of the netif
	    class Updater
	    {
	    public:
		Updater(esp::wifi::netif_t *netif);	///< @brief Constructor for the class net::wifi::sta::netif_t::Updater

		esp_err_t status() { return err; };

		/** @brief Save current network configuration for bacup
		 *  @return ESP_OK	  - success updating configuration
		 *	ESP_ERR		  - any error
		 *	ESP_ERR_NO_MEM	  - insufficient memory for backup */
		esp_err_t backup();

		/** @brief Restore previously saved network configuration from backup
		 *  @return ESP_OK	  - success updating configuration
		 *	ESP_ERR		  - any error
		 *	ESP_ERR_NO_MEM	  - backup buffer is absent, it's not found */
		esp_err_t revert();

		/** @brief Finalize updating procedure - clearing the backup buffer */
		void finalize();

		/** @brief Invoke immediately update WiFi cfg procedure: WiFi connect + netif config
		 *	    only return code without any restoring/rollback configurations
		 *  @param[in]   cfg       - new network configuration for updating
		 *  @param[in]   forcecon - is need reconnecting djuring updating network parameters
		 * @return
		 *	ESP_OK   - Setup configuration successfully
		 *	other    - as in operator()(::net::configuration& cfg), see late
		 */
		esp_err_t invoke(::net::configuration_t& cfg, bool forcecon = false);

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
		 */
		esp_err_t operator()(::net::configuration_t& cfg);

	    protected:

		/** @brief Preliliminary Set status of request to the dhcp-client - request start/stop after the connection
		 *  @param[in]   dhcp_st  - needed DHCP client run status
		 *  @return ESP_OK        - success updating configuration */
		esp_err_t dhcp_adv(bool dhcp_st);

		/** @brief Actually statr/stop dhcp-client manually - according requested status for it
		 *  @return ESP_OK        - success updating configuration */
		esp_err_t dhcp_do();

		/** @brief Set the ip cfg - start/stop dhcp-client, set or get ip address, mask and gateway
		 *  @param[in]   inf      - new ip parameters network configuration
		 *  @return ESP_OK        - success updating configuration */
		esp_err_t ip(const esp::ip4::info& inf);

		/** @brief Set the ip cfg - start/stop dhcp-client, set or get ip address, mask and gateway, alias for class net::configuration
		 *  @param[in]   cfg      - new parameters network configuration buffer
		 *  @return ESP_OK        - success updating configuration */
		esp_err_t ip(const ::net::configuration_t& cfg);

		/** @brief Apply the login cfg: wifi cfg - ssid, password - to the netif
		 *  @param[in]   cfg      - new parameters network configuration buffer
		 *  @return ESP_OK        - success updating configuration */
		esp_err_t login(const ::net::configuration_t& cfg);
		/** @brief Apply the login cfg: wifi cfg - ssid, password - to the netif
		 *  @param[in]   cfg      - new parameters network configuration buffer
		 *  @return ESP_OK        - success updating configuration */
		esp_err_t login(const config_t& cfg);
//		/** @brief Apply the login cfg: wifi cfg - ssid, password - to the netif
//		 *  @param[in]   ssid     - SSID (login) for setup
//		 *  @param[in]   passwd   - password for setup
//		 *  @return ESP_OK        - success updating configuration */
//		esp_err_t login(const std::string& ssid, const std::string& passwd);

		/** @brief simply execute connection to the selected WiFi AP
		 *  @param[in]   cfg      - new network configuration buffer
		 *  @return               - passed the 'err' value */
		esp_err_t connect(const ::net::configuration_t& cfg);


	    private:

		esp::wifi::netif_t &its_netif;

		::net::configuration_t *wifibkp = nullptr; ///<@brief storage for wifi cfg - login/password etc
		esp_err_t err = ESP_OK;

	    }; /* class esp::net::wifi::Updater */

	}; /* esp::net::wifi */

    }; /* esp::net */


    namespace wifi
    {

	namespace err
	{
	    ///@brief  reason-codes, defined in ESP-IDF SDK
	    ///@detail Decoding the WiFi error reason enum, return text message describe of it,
	    ///        C++ variant of this function
	    ///@param [in] areason   reason of the WiFi error
	    ///@return ASCIIZ C-string with text name of error reason
	    const char* reason(wifi_err_reason_t areason);

	    ///@brief  reason-codes, defined in ESP-IDF SDK with uint8_t argument
	    ///@param [in] areason   reason of the WiFi error
	    ///@return ASCIIZ C-string with text name of error reason
	    const char* reason(uint8_t areason);

	}; /* namespace esp::wifi::err */

	/// NetIf for WiFi Specialization
	class netif_t: public esp::netif_t
	{
	public:
	    /// @brief Default constructor
	    netif_t(): esp::netif_t(), update(this) {};
	    /// @brief Create the exemplar of esp::netif and create the wifi esp_netif_t object
	    netif_t(wifi_interface_t wifi_if, esp_netif_inherent_config_t &config);
	    /// Main procedure for creation the WiFi Netif
	    esp_netif_t* create(wifi_interface_t wifi_if, esp_netif_inherent_config_t &config);

	    net::wifi::Updater update;

	protected:
	    friend class esp::net::wifi::Updater;
	    wifi_interface_t type = WIFI_IF_STA;

	}; /* class esp::wifi::netif_t */

    }; /* esp::wifi */

    namespace net
    {
	// namespace for encapsulating/naming the esp::wifi system calls
	namespace wifi
	{

	    namespace storage
	    {

		/// @brief	Set the WiFi API configuration storage type (envelope on official API)
		inline esp_err_t set(wifi_storage_t storage) {
		    return esp_wifi_set_storage(storage); };
	    }; /* esp::wifi::storage */


	    namespace sta {
		namespace handlers {

		    /// @brief Set default handlers for station (envelope on official API)
		    inline esp_err_t set_default() {
			return esp_wifi_set_default_wifi_sta_handlers();};
		}; /* esp::wifi::sta */

	    }; /* esp::wifi::sta */



	    namespace mode
	    {
		/// @brief	Set the WiFi operating mode (envelope on official API)
		esp_err_t inline set(wifi_mode_t mode) {
		    return esp_wifi_set_mode(mode);};

		/// @brief  Get current operating mode of WiFi (envelope on official API)
		esp_err_t inline get(wifi_mode_t *mode) {
		    return esp_wifi_get_mode(mode); };

	    }; /* esp::wifi::mode */


	    /// C++ wrapper on the wifi_config_t
	    class config_t
	    {
	    public:

		/// minimal constructor, default type is "STA"
		config_t(wifi_interface_t _type = WIFI_IF_STA);

		/// General constructor from parent type
		config_t(wifi_interface_t, const wifi_config_t&);

		/// copy-constructor
		template <typename T>
		config_t(const T& config): type() { set(config);};

		/// copy-constructor with separate define type of the WiFi netif
//		config_t(wifi_interface_t wifs, const ::net::configuration& cfg): type(wifs) {};
		template <typename T>
		config_t(wifi_interface_t wifs, const T& config): type(wifs) { set(config); };


		/// Assigment parent type with kind of interface
		const config_t& set(wifi_interface_t, const wifi_config_t&);

		/// The assigment (setup) procedure
		template <typename T>
		const config_t& set(const T&);

		/// assigment operator
		template <typename T>
		const config_t& operator =(const T& config) {
		    return set(config); }

		/// convertion to wifi_config_t&
		operator wifi_config_t&() { return data; };
		operator const wifi_config_t&() const { return data; };
		/// get the data - with wifi_config_t& type
		wifi_config_t& get() { return data; };
		const wifi_config_t& get() const { return data; };
		wifi_interface_t getype() const { return type; };


		// Set the SSID
		esp_err_t ssid(const std::string& name) {
		    return ssid(name.c_str());};
		esp_err_t ssid(const char[]);

		// Get the SSID
		std::string ssid() const {return ssid_cstr();};
		const char* ssid_cstr() const;

		// Set Password
		esp_err_t passwd(const std::string& name) {
		    return passwd(name.c_str()); }
		esp_err_t passwd(const char[]);

		// Get the Password
		std::string passwd() const { return passwd_cstr(); };
		const char* passwd_cstr() const;

		// Set the BSSID
		esp_err_t bssid(const std::string& name) {
		    return bssid(name.c_str()); }
		esp_err_t bssid(const char[]);

		// Get the BSSID
		std::string bssid() const { return bssid_cstr(); };
		const char* bssid_cstr() const;

		/// Get Failure Retry counter value
		/// @return - if type == STA, return value of the sta.failure_retry_cnt
		///	else - return 0xff
		uint8_t retry() const {return (type == WIFI_IF_STA)? data.sta.failure_retry_cnt: 0xff;};

		/// Set Failure Retry counter value, if type == STA; else - nothing to do
		/// @param val	- new value of sta.failure_retry_cnt
		/// @retry	- old value of sta.failure_retry_cnt if type == STA,
		///		- if type == AP, return 0xff
		uint8_t retry(uint8_t val) {
		    if (type == WIFI_IF_STA) {
			std::swap(val, data.sta.failure_retry_cnt);
			return val; }
		    else return 0xff;
		};

		friend class stack;

	    protected:
		wifi_interface_t type;
		wifi_config_t data;
	    }; /* class esp::net::wifi::config_t */

	    /// @brief	namespace for pretty naming of the ESP32 WiFi STA or AP network configuration getting (wrapper on official API)
	    namespace cfg
	    {

		/// @brief	Set the configuration of the ESP32 STA or AP (envelope on official API)
		static inline esp_err_t set(wifi_interface_t interface, wifi_config_t *conf) {
		    return esp_wifi_set_config(interface, conf);};

		/// @brief	Get the configuration of the ESP32 STA or AP (envelope on official API)
		static inline esp_err_t get(wifi_interface_t if_type, wifi_config_t *conf) {
		    return esp_wifi_get_config(if_type, conf); }

	    }; /* namespace esp::wifi::config */

	    class stack
	    {
	    public:
		/// @brief  Initialize WiFi (envelope on official API)
		static esp_err_t init(const wifi_init_config_t *config) {
		    return (err = esp_wifi_init(config)); };

		/// @brief  Start WiFi according to current configuration (C++ wrapper on official API)
		static esp_err_t start() {
		    return (err = esp_wifi_start()); };

		/// @brief	Connect the ESP32 WiFi station to the AP. (C++ wrapper on official API)
		static esp_err_t connect() {
		    return (err = esp_wifi_connect()); };

		/// @brief	Disconnect the ESP32 WiFi station to the AP. (C++ wrapper on official API)
		static esp_err_t disconnect() {
		    return (err = esp_wifi_disconnect()); };

		/** @brief	Set the configuration of the ESP32 STA or AP
		 *		(C++ wrapper on official API)
		 * Attention
		 *	1. This API can be called only when specified interface is enabled, otherwise, API fail
		 * Attention
		 *	2. For station configuration, bssid_set needs to be 0; and it needs to be 1 only when users need to check the MAC address of the AP.
		 * Attention
		 *	3. ESP32 is limited to only one channel, so when in the soft-AP+station mode, the soft-AP will adjust its channel automatically to be the same as the channel of the ESP32 station.
		 * Attention
		 *	4. The configuration will be stored in NVS
		 *
		 * @Parameter
		 *	  conf – station or soft-AP configuration
		 * @Returns
		 *	  ESP_OK: succeed
		 *	  ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
		 *	  ESP_ERR_INVALID_ARG: invalid argument
		 *	  ESP_ERR_WIFI_IF: invalid interface
		 *	  ESP_ERR_WIFI_MODE: invalid mode
		 *	  ESP_ERR_WIFI_PASSWORD: invalid password
		 *	  ESP_ERR_WIFI_NVS: WiFi internal NVS error
		 *	 others: refer to the erro code in esp_err.h
		 */
		static esp_err_t configure(config_t &conf) {
		    return (err = esp_wifi_set_config(conf.type, &conf.data));};

		/// @brief	Get the configuration of the specified interface
		///		of ESP32 STA or AP (C++ wrapper on official API)
		/// @parameter	if_type – type of the interface
		/// @Return
		///	WiFi interface configuration
		static config_t configuration(wifi_interface_t if_type = WIFI_IF_STA);


		/// @brief	Stop WiFi (envelope on official API)
		static esp_err_t stop() {
		    return (err = esp_wifi_stop()); };

		/// @brief	Deinit WiFi (envelope on official API)
		static esp_err_t deinit() {
		    return (err = esp_wifi_deinit()); };

		/// @brief Clear default handlers and destroy appropriate objects (envelope on official API)
		static esp_err_t clear_all(void *esp_netif) {
		    return (err = esp_wifi_clear_default_wifi_driver_and_handlers(esp_netif)); };

		/// @brief Restore WiFi stack persistent settings to default values.
		///    This function will reset settings made using the following APIs:
		///    esp_wifi_set_bandwidth,
		///    esp_wifi_set_protocol,
		///    esp_wifi_set_config related
		///    esp_wifi_set_mode
		///
		/// @Returns
		///	ESP_OK: succeed
		///	ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
		static esp_err_t restore(void *esp_netif) {
		    return (err = esp_wifi_restore()); };

	    protected:
		static esp_err_t err /*= ESP_ERR_WIFI_NOT_INIT*/;

	    }; /* class esp::net::wifi::stack */

	}; /* namespace esp::net::wifi */

    }; /* namespace esp::net */

}; /* namespace esp */


#endif	//  __cplusplus


#endif /* _WIFI_H_ */
