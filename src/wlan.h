/*
https://msdn.microsoft.com/en-us/library/windows/desktop/ms706274(v=vs.85).aspx
Native Wifi Functions

https://msdn.microsoft.com/ja-jp/library/windows/desktop/ms706765(v=vs.85).aspx
WlanQueryInterface function

https://msdn.microsoft.com/zh-tw/library/windows/desktop/ms706886(v=vs.85).aspx
WLAN_INTF_OPCODE enumeration
*/


#include <string>//scofield
#include <time.h>//scofield

#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


//using namespace rapidjson;
using namespace std;//scofield


int init_wlan();
int enumInterfaces(wstring&);
int getAvailableNetworkList(wstring&);
wstring getNotifications();

//typedef unsigned long DWORD, *PDWORD, *LPDWORD

/*
DWORD WINAPI WlanQueryInterface(
  _In_             HANDLE                  hClientHandle,
  _In_       const GUID                    *pInterfaceGuid,
  _In_             WLAN_INTF_OPCODE        OpCode,
  _Reserved_       PVOID                   pReserved,
  _Out_            PDWORD                  pdwDataSize,
  _Out_            PVOID                   *ppData,
  _Out_opt_        PWLAN_OPCODE_VALUE_TYPE pWlanOpcodeValueType
);

DWORD WINAPI WlanSetInterface(
  _In_             HANDLE           hClientHandle,
  _In_       const GUID             *pInterfaceGuid,
  _In_             WLAN_INTF_OPCODE OpCode,
  _In_             DWORD            dwDataSize,
  _In_       const PVOID            pData,
  _Reserved_       PVOID            pReserved
);

WLAN_INTF_OPCODE value											ppData data type
wlan_intf_opcode_autoconf_enabled								BOOL
wlan_intf_opcode_background_scan_enabled						BOOL
wlan_intf_opcode_radio_state									WLAN_RADIO_STATE
wlan_intf_opcode_bss_type										DOT11_BSS_TYPE
wlan_intf_opcode_interface_state								WLAN_INTERFACE_STATE
wlan_intf_opcode_current_connection								WLAN_CONNECTION_ATTRIBUTES
wlan_intf_opcode_channel_number									ULONG
wlan_intf_opcode_supported_infrastructure_auth_cipher_pairs		WLAN_AUTH_CIPHER_PAIR_LIST
wlan_intf_opcode_supported_adhoc_auth_cipher_pairs				WLAN_AUTH_CIPHER_PAIR_LIST
wlan_intf_opcode_supported_country_or_region_string_list		WLAN_COUNTRY_OR_REGION_STRING_LIST
wlan_intf_opcode_media_streaming_mode							BOOL
wlan_intf_opcode_statistics										WLAN_STATISTICS
wlan_intf_opcode_rssi											LONG
wlan_intf_opcode_current_operation_mode							ULONG
wlan_intf_opcode_supported_safe_mode							BOOL
wlan_intf_opcode_certified_safe_mode							BOOL

*/

