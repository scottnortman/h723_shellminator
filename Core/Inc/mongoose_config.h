// 	File:	mongoose_config.h
//	Date:	June 4 2025
//
//	https://mongoose.ws/documentation/tutorials/device-dashboard/



#pragma once
#define MG_ARCH MG_ARCH_NEWLIB     // For all ARM GCC based environments
#define MG_ENABLE_TCPIP 1          // Enables built-in TCP/IP stack
#define MG_ENABLE_CUSTOM_MILLIS 1  // We must implement mg_millis()
#define MG_ENABLE_TCPIP_PRINT_DEBUG_STATS 1  // Enable debug stats log

#define MG_ENABLE_CUSTOM_RANDOM 1

//static IP
#define MG_TCPIP_IP		MG_IPV4(192,168,0,101)
#define MG_TCPIP_GW		MG_IPV4(192,168,0,1)
#define MG_TCPIP_MASK	MG_IPV4(255,255,255,0)

// Uncomment the driver for your device
#define MG_ENABLE_DRIVER_STM32H 1
// #define MG_ENABLE_DRIVER_STM32F 1
// #define MG_ENABLE_DRIVER_IMXRT 1
// #define MG_ENABLE_DRIVER_SAME54 1
// #define MG_ENABLE_DRIVER_TM4C 1
// #define MG_ENABLE_DRIVER_RA 1
// #define MG_ENABLE_DRIVER_W5500 1
// #define MG_ENABLE_DRIVER_XMC 1
// #define MG_ENABLE_DRIVER_XMC7 1
