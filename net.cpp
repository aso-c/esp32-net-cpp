/*
 * @file networking.cpp
 *
 * @brief General backend of the networking
 *
 * @date Created on: 4 окт. 2023 г.
 * @Author: aso
 */

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <string>
#include <cctype>
#include <utility>


#include <esp_log.h>

#include <esp_netif.h>

#include <esp_system.h>
#include <esp_types.h>
#include <esp_event.h>

#include "astring.h"

#include "net.h"
#include "sdkconfig.h"

using namespace std;



//--[ class esp::netif_t ]---------------------------------------------------------------------------------------------

/// @brief Create the exemplar of default esp_netif_t object
//esp_netif_t *esp_netif_new(const esp_netif_config_t *esp_netif_config)
esp::netif_t::netif_t(const esp_netif_config_t &config):
	netif_t()
{
    create(config);
}; /* esp::netif_t::netif_t(const esp_netif_config_t &config) */



/// @brief Create the exemplar of default esp_netif_t object
//esp_netif_t *esp_netif_new(const esp_netif_config_t *esp_netif_config)
esp_netif_t *esp::netif_t::create(const esp_netif_config_t &config)
{
    instance = esp_netif_new(&config);
    return instance;
}; /* esp::netif_t::create(const esp_netif_config_t &esp_netif_config) */



//--[ class esp::ip4::address ]----------------------------------------------------------------------------------------

esp::ip4::base::operator const char*() const
{
    static char buf[] = "xxx.xxx.xxx.xxx";
    return esp_ip4addr_ntoa(/*this*/&instance(), buf, sizeof(buf));
}; /* esp::ip4::address::operator const char*() */


//--[ class esp::ip4::info ]-------------------------------------------------------------------------------------------

///@brief check, if the mask is correct ip4 network mask
bool esp::ip4::info::chkmask()
{
	uint32_t pattern = (1UL << 31) - 1 /*0x7fffffff*/;

    for (int i = 1; i < (32-1); i++)
    {
	pattern = pattern << 1;
	if (netmask.addr == pattern )
	    return true;
    }; /* for (int i = 1; i < (32-1); i++) */
    return false;
}; /* esp::ip4::info::chkmask() */


//--[ class esp::dhcp_client_t ]---------------------------------------------------------------------------------------

/// Get/set DHCP option of the DHCP client
esp_err_t esp::dhcp_client_t::option(esp_netif_dhcp_option_mode_t opt_op, esp_netif_dhcp_option_id_t opt_id, void *opt_val, uint32_t opt_len)
{
//    esp_err_t esp_netif_dhcps_option(esp_netif_t *esp_netif, esp_netif_dhcp_option_mode_t opt_op, esp_netif_dhcp_option_id_t opt_id, void *opt_val, uint32_t opt_len);
    err = esp_netif_dhcps_option(netif->get(), opt_op, opt_id, opt_val, opt_len);
    return err;
}; /* esp::dhcp_client_t::option */

///@brief start DHCP client
esp_err_t esp::dhcp_client_t::start()
{
    err = esp_netif_dhcpc_start(netif->get());
    return err;
}; /* esp::dhcp_client_t::start */

///@brief stop DHCP client
esp_err_t esp::dhcp_client_t::stop()
{
    err = esp_netif_dhcpc_stop(netif->get());
    return err;
}; /* esp::dhcp_client_t::stop */



///@brief status of the DHCP client
esp_netif_dhcp_status_t esp::dhcp_client_t::status() const
{
	esp_netif_dhcp_status_t status;
    err = esp_netif_dhcpc_get_status(netif->get(), &status);
    return status;
}; /* esp::dhcp_client_t::status */




namespace net
{

    //--[ class net::configuration_t ]---------------------------------------------------------------------------------


    configuration_t::configuration_t(const configuration_t& cfg):
	ip(cfg.ip),
	mask(cfg.mask),
	gate(cfg.gate),
	dhcp(cfg.dhcp),
	use_pwd(cfg.use_pwd),
	login(cfg.login),
	passwd(cfg.passwd)
    { };


    configuration_t::configuration_t(bool dhcp_st, const esp::ip4::info& info,
		const std::string& log_in, const std::string& passw):
	ip(info.ip.addr),
	mask(info.netmask.addr),
	gate(info.gw.addr),
	dhcp(dhcp_st),
	use_pwd(log_in.length() > 0? true: false),
	login(log_in),
	passwd(passw)
    {};

    const esp_netif_ip_info_t *configuration_t::ipdata() const
    {
	    static esp_netif_ip_info_t ipcfg;

	ipcfg.ip.addr = ip;
	ipcfg.netmask.addr = mask;
	ipcfg.gw.addr = gate;
	return &ipcfg;
    }; /* net::configuration::ipdata() */

    /// Clear status of the changing the 'cfg' object
    void configuration_t::clr_chgst()
    {
	ESP_LOGW(__func__, " ###--- clear change status of the DHCP state");
	dhcp.clr_chgstat();

	ESP_LOGW(__func__, " ###--- clear change status of the ip-address");
	ip.clr_chgstat();

	ESP_LOGW(__func__, " ###--- clear change status of the net mask");
	mask.clr_chgstat();

	ESP_LOGW(__func__, " ###--- clear change status of the gateway address");
	gate.clr_chgstat();

	ESP_LOGW(__func__, " ###--- clear change status of the Using Password state");
	use_pwd.clr_chgstat();

	ESP_LOGW(__func__, " ###--- clear change status of the WiFi Login/SSID");
	login.clr_chgstat();

	ESP_LOGW(__func__, " ###--- clear change status of the WiFi password");
	passwd.clr_chgstat();
    }; /* net::configuration::clr_chgst() */




    namespace cfg
    {

	/// @brief set the DHCP status from the string
	const flag_field& flag_field::operator =(const std::string& str)
	{
	    if (astr::confirm(str))
	    {
		set_chgstat();
		state = true;
	    }; /* if configuration::yes_str(str) */
	    if (astr::decline(str))
	    {
		set_chgstat();
		state = false;
	    }; /* if configuration::no_str(str) */
	    return *this;
	}; /* net::cfg::flag_field::operator= (const string&) */

	string_field::operator const char*()
	{
		static std::string strbuf;

	    strbuf = val;
	    return strbuf.c_str();
	}; /* net::cfg::info::string_field::operator const char*() */

    }; /* namespace net::cfg */

}; /* namespace net */



// namesopace for encapsulating of the esp system functions
namespace esp
{

    /// @brief write the new ip configuration from param to to own netif; inner config buffer is not changed
    esp_err_t netif_t::Config_t::upgrade(const ip4::info& cfg)
    {
	err = esp_netif_set_ip_info(netif, &cfg);
	return err;
    }; /* netif_t::Config::upgrade(cfg) */

    ///< @brief read the all ip configuration from the netif to the to the temporary returned object
    ip4::info netif_t::Config_t::read() const
    {
	    ip4::info tmpdata;
	err = esp_netif_get_ip_info(netif, &tmpdata);
	return tmpdata;
    }; /* netif_t::Config_t::read() */


    /// @brief read the old ip configuration from the netif to the field 'data'
    ip4::info netif_t::Config_t::read_old() const
    {
	    ip4::info tmpdata;
	err = esp_netif_get_old_ip_info(netif, &tmpdata);
	return tmpdata;
    }; /* netif_t::Config::read_old() */



    /// @brief write the old ip configuration from param to own netif; inner config buffer is not changed
    esp_err_t netif_t::Config_t::set_old(const ip4::info& cfg)
    {
	err = esp_netif_set_old_ip_info(netif, &cfg);
	return err;
    }; /* netif_t::Config::set_old(cfg) */


}; /* namespace esp */


//--[ net.cpp ]--------------------------------------------------------------------------------------------------------
