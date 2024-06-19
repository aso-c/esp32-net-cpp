set(srcs "net.cpp" "wifi.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS .
                    PRIV_REQUIRES log
                    REQUIRES esp_wifi esp_eth utils
		    )

