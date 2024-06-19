/**
 * @file net.h
 *
 * @brief General backend definition for network component.
 *	  May be only inner definitions of the 'net' component
 *
 * @license This code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 *  software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 *  CONDITIONS OF ANY KIND, either express or implied.
 *
 * @date   Created on: 20 сент. 2023 г.
 * @Author aso
 */

#ifndef _NET_H_
#define _NET_H_

#ifdef __cplusplus
extern "C" {
#endif







#ifdef __cplusplus
} /* extern "C" */


namespace net
{
  /// forward declaration
  /// for the network configuration class
  class configuration_t;
}; /* namespace net */

/// namesopace for encapsulating of the esp system functions
namespace esp
{

    namespace ip4
    {

	///@brief check, if the mask is correct ip4 network mask
	bool chkmask(uint32_t mask);

	///@brief Base of ip address class: interface for manipulating with esp_ip4_addr_t
	class base
	{
	public:

	    virtual ~base() {};


	    /// @brief Set new value of the ip
	    ///
	    /// @return
	    ///  - ESP_OK   - ip address was changed sucefully
	    ///  - ESP_ERR_NOT_FINISHED - ip adress was not changed by new ip is rq the old ip
	    ///  - ESP_FAIL - ip address was not changed by the error in the new ip
	    template <typename iptype>
	    esp_err_t set(const iptype& ipaddr);

	    ///@brief conversion any types, mappable to ip address
	    ///@parameter   any type value, represented the ip-address
	    ///@return	    ip-address uint32_t value, according the passed parameter
	    template <typename iptype>
	    static uint32_t value(const iptype& ipaddr);

	    template <typename iptype>
	    const base& operator =(const iptype& ip) noexcept
				{ set(ip); return *this;};

	    operator uint32_t() const { return instance().addr;};
	    operator uint32_t&() { return instance().addr;};
	    operator const esp_ip4_addr_t&() const noexcept { return instance(); };
	    operator const char*() const;
	    operator std::string() const {return static_cast<const char*>(*this);};


	    friend bool operator == (const base& left, const base& right) noexcept {
		return left.instance().addr == right.instance().addr; };

	    template <typename iptype>
	    friend bool operator == (const base& left, const iptype& right) noexcept;

	    template <typename iptype>
	    friend bool operator == (const iptype& left, const base& right) noexcept {
		return operator ==(right, left); }

	    template <typename iptype>
	    friend bool operator != (const base& left, const iptype& right) noexcept {
		return !(operator==(left, right)); };

	    template <typename iptype>
	    friend bool operator != (const iptype& left, const base& right) noexcept {
		return !(operator ==(left, right)); }

	protected:

	    virtual esp_ip4_addr_t& instance() = 0;
	    virtual const esp_ip4_addr_t& instance() const = 0;

	}; /* class esp::ip4::base */


	template <>
	uint32_t inline base::value<uint32_t>(const uint32_t& ipvalue) {
	    return ipvalue; };

//	template <>
//	uint32_t inline base::value<const char *>(const char* const & ipaddr) {
//	    return esp_ip4addr_aton(ipaddr); };

	template <>
	uint32_t inline base::value<std::string>(const std::string& ipaddr) {
	    return esp_ip4addr_aton(ipaddr.c_str()); };

	template <>
	uint32_t inline base::value<esp_ip4_addr>(const esp_ip4_addr& ipaddr) {
	    return ipaddr.addr; };

	template <>
	uint32_t inline base::value<base>(const base& ipaddr) {
	    return ipaddr.instance().addr; };


	template <typename iptype>
	inline bool operator == (const base& left, const iptype& right) {
	    return left.instance().addr == /*address*/base::value(right); };


	/// @brief Set new value of the ip
	///
	/// @return
	///  - ESP_OK   - ip address was changed sucefully
	///  - ESP_ERR_NOT_FINISHED - ip adress was not changed by new ip is rq the old ip
	///  - ESP_FAIL - ip address was not changed by the error in the new ip
	template <typename iptype>
	esp_err_t inline base::set(const iptype& ipaddr)
	{
	    if(instance().addr == value(ipaddr))	///< if new ip is eq the old ip - not any changed
		return ESP_ERR_NOT_FINISHED;

	    instance().addr = value(ipaddr);
	    return ESP_OK; /*ESP_FAIL*/
	}; /* esp::ip4::address::set() */


	///@brief Implementation of the C++ wrapper on the esp_ip4_addr_t type
	class address: public base
	{
	public:
	    address(const base &_ip): ip{static_cast<const esp_ip4_addr_t&>(_ip)} {};
	    address(const esp_ip4_addr_t &_ip): ip{_ip.addr} {};

	    explicit address(uint32_t ipval = 0x0): ip{ipval} {};
	    explicit address(const std::string &ipstr);

	    virtual ~address() {};

	    /// @brief Set new value of the ip
	    ///
	    /// @return
	    ///  - ESP_OK   - ip address was changed sucefully
	    ///  - ESP_ERR_NOT_FINISHED - ip adress was not changed by new ip is rq the old ip
	    ///  - ESP_FAIL - ip address was not changed by the error in the new ip
	    template <typename iptype>
	    esp_err_t set(const iptype& ipaddr) { return base::set(ipaddr); };

	    template <typename iptype>
	    const address& operator =(const iptype& ip) noexcept
				{ set(ip); return *this;};

	protected:

	    esp_ip4_addr_t& instance() override { return ip; };
	    const esp_ip4_addr_t& instance() const override { return ip; };

	    esp_ip4_addr_t ip;	///< inner ip-addres storage

	}; /* class esp::ip4::address */


//	/*inline*/ address::address(const base &_ip): ip{_ip.instance().addr} {};


	inline address::address(const std::string& ipstr) {
	    ip.addr = base::value(ipstr); };



	///@brief Base of ip address class: interface for manipulating with esp_ip4_addr_t
	class addrref: public base
	{
	public:
	    addrref(esp_ip4_addr_t& ref): ipref(ref) {};
	    addrref(esp_ip4_addr_t& ref, const base &_ip): addrref(ref) {ipref.addr = static_cast<const esp_ip4_addr_t&>(_ip).addr;};
	    addrref(esp_ip4_addr_t& ref, const esp_ip4_addr_t &_ip): addrref(ref) { ipref.addr = _ip.addr; };

	    explicit addrref(esp_ip4_addr_t& ref, uint32_t ipval /*= 0x0*/): addrref(ref) { ipref.addr = ipval;};
	    explicit addrref(esp_ip4_addr_t& ref, const std::string &ipstr);

	    virtual ~addrref() {};

	    /// @brief Set new value of the ip
	    ///
	    /// @return
	    ///  - ESP_OK   - ip address was changed sucefully
	    ///  - ESP_ERR_NOT_FINISHED - ip adress was not changed by new ip is rq the old ip
	    ///  - ESP_FAIL - ip address was not changed by the error in the new ip
	    template <typename iptype>
	    esp_err_t set(const iptype& ipaddr) { return base::set(ipaddr); };

	    template <typename iptype>
	    const addrref& operator =(const iptype& ip) noexcept
				{ set(ip); return *this;};


	protected:

	    esp_ip4_addr_t& ipref;

	    virtual esp_ip4_addr_t& instance() override { return ipref; };
	    virtual const esp_ip4_addr_t& instance() const override {  return ipref;  };

	}; /* class esp::ip4::adrref */

	inline addrref::addrref(esp_ip4_addr_t& ref, const std::string& ipstr): addrref(ref) {
	    ipref.addr = base::value(ipstr); };






	///@brief C++ wrapper on the esp netif ip info data
	class info: public esp_netif_ip_info_t
	{
	public:
	    info(): info(0x0,0x0,0x0) {};
	    info(uint32_t ip, uint32_t gate = 0xc0a80001, uint32_t mask = 0xffffff00 /*255.255.255.0*/):
		esp_netif_ip_info_t{ip,mask,gate} {};
	    info(const esp_netif_ip_info_t& src): esp_netif_ip_info_t(src) {};
	    info(const info& src): esp_netif_ip_info_t(src) {};
	    info(const net::configuration_t& src);

	    const info& operator =(const info& src) {
		ip.addr = src.ip.addr;
		netmask.addr = src.netmask.addr;
		gw.addr = src.gw.addr;
		return *this;
	    };

	    const info& operator =(const esp_netif_ip_info_t& src) {
		ip.addr = src.ip.addr;
		netmask.addr = src.netmask.addr;
		gw.addr = src.gw.addr;
		return *this;
	    };

	    const info& operator =(const net::configuration_t& src);

	    operator const esp_netif_ip_info_t*() const { return this;  };
	    operator	  esp_netif_ip_info_t*()	{ return this;  };

	    ///@brief check, if the mask is correct ip4 network mask
	    bool chkmask();

	    ///@brief check, if the address set is internally consistent
	    bool consistent() { return (ip.addr & netmask.addr) == (gw.addr & netmask.addr);};

	}; /* class esp::ip4::info */

    }; /* namespace esp::ip4 */




    /// class for encapsulating/naming the esp::netif system procedures - forward declaration
    class netif_t;

    ///@brief DHCP client control class
    class dhcp_client_t
    {
    public:
	dhcp_client_t(netif_t* net_if): netif(net_if), err(ESP_OK) {};

	/// Get/set DHCP option of the DHCP client
	esp_err_t option(esp_netif_dhcp_option_mode_t opt_op, esp_netif_dhcp_option_id_t opt_id, void *opt_val, uint32_t opt_len);

	///@brief DHCP client is enabled (directly by the netif flags)
	bool enabled();

	///@brief DHCP client is initialized
	bool initialized() { return (status() == ESP_NETIF_DHCP_INIT); };
	///@brief DHCP client is started
	bool started() const { return (status() == ESP_NETIF_DHCP_STARTED); };
	///@brief DHCP client is stoped
	bool stopped() const { return (status() == ESP_NETIF_DHCP_STOPPED); };
	void ackuire(bool val) {ack = val;};	///<@brief set the 'ack' value (deferred request to start DHCP client) according to a val
	bool ackuired() const {return ack;};	///<@brief get the 'ack' value (deferred request to start DHCP client)
	esp_err_t start();	///< start DHCP client
	esp_err_t stop(); 	///< stop DHCP client
	esp_netif_dhcp_status_t
		 status() const;///< status of the DHCP client
	esp_err_t error() const;///< error status of the DHCP client
	bool ack = true;	///< flag, that indicied deferred request to start DHCP-client

    private:
	netif_t *netif;
	mutable esp_err_t err;
    }; /* class esp::dhcp_client_t */

    /// DHCP server control class
    class dhcp_server_t
    {
    public:
	dhcp_server_t(netif_t* net_if): netif(net_if), err(ESP_OK) {};

	///@brief DHCP server is enabled (directly by the netif flags)
	bool enabled();
    private:
	netif_t* netif;
	esp_err_t err;
    }; /* class esp::dhcp_server_t */

    /// DHCP client/server control class
    class dhcp_t
    {
    public:
	dhcp_t(netif_t* net_if): client(net_if), server(net_if) {};
	dhcp_client_t client;
	dhcp_server_t server;
    }; /* class esp::dhcp_t */



    /// @brief class esp::netif::info, derived from esp_netif_ip_info_t
    /// @detail class for encapsulating/naming the esp::netif system procedures
    class netif_t
    {
    public:

	/// @brief class esp::netif::Config, inner in class netif_t c++ wrapper over esp_netif_ip_info_t
	class Config_t
	{
	public:
	    Config_t(esp_netif_t* &_netif): /*ip(data.ip), mask(data.netmask), gate(data.gw),*/ netif(_netif), err(ESP_OK)  {};
	    ip4::info read() const;   ///< @brief read the all ip configuration from the netif to the  to the temporarily returned object
	    ip4::info read_old() const;   ///< @brief read the old ip configuration from the netif to the temporarily returned object
	    esp_err_t upgrade(const ip4::info&);///< @brief write the new ip configuration from param to to own netif
	    esp_err_t set(const ip4::info& info) {return upgrade(info);};///< @brief alias for the upgrade() method
	    esp_err_t set_old(const ip4::info&);///< @brief write the old ip configuration from param to own netif; inner config buffer is not changed
	    esp_err_t status() { return err; };
	    Config_t& operator =(const ip4::info& info) {upgrade(info); return *this;}; ///< assignment operator for esp::netif_t::Config
	    ip4::info info() const { return read();};	///<@brief return netif ip information
	    ip4::info get()  const { return info();};	///<@brief alias for the 'info()' method
	    operator ip4::info () const {return get();}	///<@brief generate temporary object with ip4 config info from the netif

	    const Config_t& operator=(const esp_netif_ip_info_t& src) { upgrade(src); return *this; };

	    ip4::address ip() {return get().ip;};	///< @brief get ip address of the referred netif
	    ip4::address mask() { return get().netmask;};	///< @brief get mask of the referred netif
	    ip4::address gate() {return get().gw;};	///< @brief get gateway ip address of the referred netif

	private:
	    esp_netif_t* &netif;
	    mutable esp_err_t err = ESP_ERR_INVALID_STATE;
	}; /* esp::netif_t::Config_t */


	Config_t cfg;
	dhcp_t dhcp;


	/// @brief Default constructor of the esp::netif and create the wifi esp_netif_t object
	netif_t(): cfg(instance), dhcp(this) {};

	/// @brief Create the exemplar of default esp_netif_t object
	//esp_netif_t *esp_netif_new(const esp_netif_config_t *esp_netif_config)
	netif_t(const esp_netif_config_t &config);


	/// @brief Create the exemplar of esp::netif and create the wifi esp_netif_t object -->> removed to a esp::wifi::netif
	~netif_t() { destroy(); };

	/// @brief Create the exemplar of default esp_netif_t object
	esp_netif_t *create(const esp_netif_config_t &config);


	/// @brief Destroys the esp_netif object, envelope on official API
	void inline destroy() {
	    if (instance != nullptr)
		esp_netif_destroy(instance);
	    instance = nullptr;}

	///@brief Get netif flags
	esp_netif_flags_t flags() { return esp_netif_get_flags(instance); };

	///@brief get netif status (error)
	esp_err_t status() { return err; };


	esp_err_t set_ip(const esp_netif_ip_info_t *ip_info) { err = esp_netif_set_ip_info(instance, ip_info); return err; };

	/// @brief Get the inner stored instance of the netif
	esp_netif_t* get() { return instance; };
	operator esp_netif_t* () { return instance; };

	/// @brief Create interface link-local IPv6 address, envelope on official API
	inline esp_err_t create_ip6_linklocal() {
	    return esp_netif_create_ip6_linklocal(instance); };


    protected:
	esp_netif_t* instance = nullptr; //< inner stored exemplar of the netif
	esp_err_t err = ESP_ERR_INVALID_STATE;

    }; /* class esp::netif_t */

}; /* namespace esp */



///@brief DHCP client is enabled (directly by the netif flags)
inline bool esp::dhcp_client_t::enabled() {
    return netif->flags() & ESP_NETIF_DHCP_CLIENT; };

///@brief DHCP server is enabled (directly by the netif flags)
inline bool esp::dhcp_server_t::enabled() {
    return netif->flags() & ESP_NETIF_DHCP_SERVER; };


// @brief class for managing the changing state
class chg_mgr
{
public:
    chg_mgr(bool st = false): chgst(st) {};
    bool changed() {return chgst;};
    bool is_changed() {return changed();};
    void clr_chgstat() {chgst = false;};
protected:
    void set_chgstat() {chgst = true;};
    bool chgst = false;	///< field is changed?
}; /* class chg_mgr */




namespace net
{
    namespace cfg
    {

	class ip4_addr_field: public esp::ip4::address, public chg_mgr
	{
	public:
	    ip4_addr_field(uint32_t ipval = 0x0): address(ipval) {};

	    template <typename iptype>
	    ip4_addr_field(const iptype& ipaddr): address(ipaddr) {};

	    template <typename iptype>
	    const ip4_addr_field& operator =(const iptype& ip) noexcept
					{set(ip); return *this;};
	    template <typename iptype>
	    esp_err_t set(const iptype& ipaddr) noexcept;

	    template <typename iptype>
	    friend bool operator == (const ip4_addr_field& left, const iptype& right) {
		return operator == (static_cast<const esp::ip4::base&>(left), right); };

	    template <typename iptype>
	    friend bool operator == (const iptype& left, const ip4_addr_field& right) {
		return operator ==(right, left); }

	    template <typename iptype>
	    friend bool operator != (const ip4_addr_field& left, const iptype& right) {
		return !(operator==(left, right)); };

	    template <typename iptype>
	    friend bool operator != (const iptype& left, const ip4_addr_field& right) {
		return !(operator ==(left, right)); }

	}; /* class net::cfg::ip4_addr_field */

	template <typename iptype>
	esp_err_t inline ip4_addr_field::set(const iptype& ipaddr) noexcept {
	    if (instance().addr != esp::ip4::/*address*/base::value(ipaddr))
		set_chgstat();
	    return esp::ip4::base::set(ipaddr);
	}; /* net::cfg::ip4_addr_field::set() */


	/// @brief class for boolean flag fields
	class flag_field: public chg_mgr
	{
	public:
	    flag_field(bool st): state(st) {};

	    const flag_field& operator =(const bool st) { if (st != state) { set_chgstat(); state = st;}; return *this; };
	    const flag_field& operator =(const std::string& str);

	    operator bool() const { return state; };
	    operator bool&() { return state; };	///< acess to inner data value, may be modified through lvalue
	    operator const char* () const { return  state? "1": "0"; };
	    operator const std::string () const { return  state? "1": "0"; };

	protected:
	    bool state;	// Status of the boolean field
	}; /* class net::cfg::info::flag_field */

	class string_field: public chg_mgr
	{
	public:

	    string_field(): val("") {};
	    template <typename strt>
	    string_field(strt&& str = ""): val(std::forward<strt>(str)) {};
	    string_field(const string_field& stf): chg_mgr(false), val(stf.val) {};

	    const string_field& operator =(const string_field& strf) { if (strf.val != val) { set_chgstat(); val = strf.val;}; return *this; };
	    operator std::string&() { return val; };
	    operator const std::string&() const { return val; };
	    std::string& str() { return val; };
	    const std::string& str() const { return val; };
	    const std::string& conststr() const { return val; };
	    const char* c_str() { return val.c_str(); };
	    operator const char*();

	    bool operator ==(const std::string& str) const {return val == str;};
	    bool operator ==(const char str[]) const {return val == str;};
	    bool operator !=(const std::string& str) const {return !(operator ==(str));};
	    bool operator !=(const char str[]) const {return !(operator ==(str));};

	protected:
	    std::string val;
	}; /* class net::cfg::string_field */

    }; /* namespace net::cfg */


    /**
     * @brief information block of the specified network configuration -
     *  ethernet, wifi ap, wifi sta
     */
    class configuration_t
    {
    public:
	configuration_t();
	configuration_t(const configuration_t&);
	configuration_t(bool dhcp_req, const esp::ip4::info& ipinfo,
		const std::string& login = "", const std::string& passwd = "");
	template <class iptype, class masktype, class gatetype, class logintype, class passwdtype>
	configuration_t(bool dhcp_req, iptype&& ip, masktype&& mask, gatetype&& gate, bool use_pwd,
		logintype&& login, passwdtype&& passwd);
	virtual ~configuration_t();	///< stub for virtual destructor
	virtual esp_err_t factory_reload();	///< @brief stub for reset/restore factory default configuration
	const esp_netif_ip_info_t *ipdata() const;
	operator esp::ip4::info ()const { return esp::ip4::info(ip, mask, gate); };

	cfg::ip4_addr_field ip;	///< ip address storage
	cfg::ip4_addr_field mask;	///< mask storage
	cfg::ip4_addr_field gate;	///< gateway ip storage
	cfg::flag_field dhcp    = true;	///< dhcp enabled?

	cfg::flag_field use_pwd = false;	///< used/desired login/password for the connection
	cfg::string_field login;	///< login string field
	cfg::string_field passwd;	///< password string field

	/// @brief requested wifi station cfg (SSID or password) is changed
//	bool wifi_changed();
	bool login_changed();

	/// @brief requested wifi station netif ip cfg (ip, mask and gate) is changed
//	bool netif_changed();
	bool ip_changed();

	/// Clear status of the changing the all data fields: ip4 adr/mask/gate & login/pwd
	void clr_chgst();

	static constexpr char pwd_stub[] = "< WiFi password is never shown >";	// stub for password string

    }; /* class net::configuration_t */


    //@brief main constructor with template parameters
    template <class iptype, class masktype, class gatetype, class logintype, class passwdtype>
    inline configuration_t::configuration_t(bool dhcp_req, iptype&& ipin, masktype&& maskin, gatetype&& gatin, bool usepwd_in,
			logintype&& login, passwdtype&& passwd):
			ip(std::forward<iptype>(ipin)), mask(std::forward<masktype>(maskin)), gate(std::forward<gatetype>(gatin)),
			dhcp(dhcp_req), use_pwd(usepwd_in),
			login(std::forward<logintype>(login)), passwd(std::forward<passwdtype>(passwd))
    {}; /* net::confifuration_t::configuration_t() */


    ///@brief stub for default constructor
    inline configuration_t::configuration_t() {};
    ///@brief stub for virtual destructor
    inline configuration_t::~configuration_t() {};
    ///@brief stub for reset/restore factory default configuration
    inline esp_err_t configuration_t::factory_reload() { return ESP_OK; };

    /// @brief requested wifi station cfg (SSID or password) is changed
    inline bool configuration_t::login_changed() {
        return (login.changed() || passwd.changed() || use_pwd.changed()); }; /* net::configuration::wifi_changed() */

    /// @brief requested wifi station netif ip cfg (ip, mask and gate) is changed
    inline bool configuration_t::ip_changed() {
        return (dhcp.changed() || ip.changed() || mask.changed() || gate.changed()); }; /* net::configuration::netif_changed() */



}; /* namespace esp::net */


#endif /* __cplusplus */

#endif /* _NET_H_ */
