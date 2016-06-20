#ifndef __HA_CONFIG_H
#define __HA_CONFIG_H

#ifdef HAVE_CONFIG_H
#	include "../config.h"
#endif

#if defined(_WIN32_WCE)
#	define HA_WINCE
#	define snprintf _snprintf
#elif defined(WIN32)
#	define HA_WIN32
#	define HA2
//#define snprintf sprintf_s
#else
#	define HA_LINUX
#endif

#if defined(HAVE_LUA_H) || defined(HAVE_LUA5_2_LUA_H)
#	define USE_LUA
#endif 

#ifdef HA2
#	define USE_XML
#	define USE_CONFIG
#endif

#define USE_JSON

#ifdef USE_XML
#	define _LIBUTILS_USE_XML
#	define _LIBUTILS_USE_XML_LIBXML2
#elif defined(USE_JSON) && defined(HAVE_JSON_JSON_H)
#	define CConfigItem Jsoncpp::Value
#endif



#endif
