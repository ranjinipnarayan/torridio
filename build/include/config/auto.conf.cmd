deps_config := \
	/Users/ranjininarayan/esp/esp-idf/components/app_trace/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/aws_iot/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/bt/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/driver/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/esp32/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/ethernet/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/fatfs/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/freertos/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/heap/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/libsodium/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/log/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/lwip/Kconfig \
	/Users/ranjininarayan/Desktop/18-500/torridio/main/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/mbedtls/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/openssl/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/pthread/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/spi_flash/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/spiffs/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/wear_levelling/Kconfig \
	/Users/ranjininarayan/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/Users/ranjininarayan/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/Users/ranjininarayan/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/Users/ranjininarayan/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
