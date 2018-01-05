/*
https://msdn.microsoft.com/zh-tw/library/windows/desktop/ms706716(v=vs.85).aspx
WlanEnumInterfaces function
The WlanEnumInterfaces function enumerates all of the wireless LAN interfaces currently enabled on the local computer.
DWORD WINAPI WlanEnumInterfaces(
  _In_		 HANDLE 				   hClientHandle,
  _Reserved_ PVOID					   pReserved,
  _Out_ 	 PWLAN_INTERFACE_INFO_LIST *ppInterfaceList
);
*/

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <wlanapi.h>
#include <objbase.h>
#include <wtypes.h>

#include <stdio.h>
#include <stdlib.h>

#include "wlan.h"//scofield

// Need to link with Wlanapi.lib and Ole32.lib
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")


bool isWlanInitiated = false;
HANDLE g_hClient = NULL;
WLAN_INTERFACE_INFO g_IfInfo;
//PWLAN_INTERFACE_INFO g_pIfInfo = NULL;
//GUID g_InterfaceGuid;
wstring g_notification = L"";


// get ACM notification string
LPWSTR
GetAcmNotificationString(
    DWORD acmNotif,
    PVOID pData
)
{
    LPWSTR strRetCode;

    switch(acmNotif)
    {
        case wlan_notification_acm_autoconf_enabled:
            strRetCode = L"\"autoconf enabled\"";
            break;
        case wlan_notification_acm_autoconf_disabled:
            strRetCode = L"\"autoconf disabled\"";
            break;
        case wlan_notification_acm_background_scan_enabled:
            strRetCode = L"\"background scan enabled\"";
            break;
        case wlan_notification_acm_background_scan_disabled:
            strRetCode = L"\"background scan disabled\"";
            break;
        case wlan_notification_acm_power_setting_change:
            strRetCode = L"\"power setting change\"";
            break;
        case wlan_notification_acm_scan_complete:
            strRetCode = L"\"scan complete\"";
            break;
        case wlan_notification_acm_scan_fail:
            strRetCode = L"\"scan fail\"";
            break;
        case wlan_notification_acm_connection_start:
            strRetCode = L"\"connection start\"";
            break;
        case wlan_notification_acm_connection_complete:
            strRetCode = L"\"connection complete\"";
            break;
        case wlan_notification_acm_connection_attempt_fail:
            strRetCode = L"\"connection fail\"";
            break;
        case wlan_notification_acm_filter_list_change:
            strRetCode = L"\"filter list change\"";
            break;
        case wlan_notification_acm_interface_arrival:
            strRetCode = L"\"interface arrival\"";
            break;
        case wlan_notification_acm_interface_removal:
            strRetCode = L"\"interface removal\"";
            break;
        case wlan_notification_acm_profile_change:
            strRetCode = L"\"profile change\"";
            break;
        case wlan_notification_acm_profiles_exhausted:
            strRetCode = L"\"profiles exhausted\"";
            break;
        case wlan_notification_acm_network_not_available:
            strRetCode = L"\"network not available\"";
            break;
        case wlan_notification_acm_network_available:
            strRetCode = L"\"network available\"";
            break;
        case wlan_notification_acm_disconnecting:
            strRetCode = L"\"disconnecting\"";
            break;
        case wlan_notification_acm_disconnected:
            strRetCode = L"\"disconnected\"";
            break;
        case wlan_notification_acm_adhoc_network_state_change:
            strRetCode = L"\"ad hoc network state changes\"";
            break;
        default:
            strRetCode = L"\"unknown ACM notification\"";
    }

    return strRetCode;
}

// get MSMM notification string
LPWSTR
GetMsmNotificationString(
    DWORD msmNotif, 
    PVOID pData
)
{
	LPWSTR strRetCode;
	
	//WCHAR* == LPWSTR
	//WCHAR strRet[256] = {L""};

	//WLAN_SIGNAL_QUALITY is of type ULONG
	ULONG signal_quality = 0;

	PWLAN_PHY_RADIO_STATE pPHY_RADIO_STATE;

	switch(msmNotif)
	{
		case wlan_notification_msm_associating:
			//strRetCode = L"\"associating\"";
			g_notification += L"\"associating\"";
			break;
		case wlan_notification_msm_associated:
			//strRetCode = L"\"associated\"";
			g_notification += L"\"associated\"";
			break;
		case wlan_notification_msm_authenticating:
			//strRetCode = L"\"authenticating\"";
			g_notification += L"\"authenticating\"";
			break;
		case wlan_notification_msm_connected:
			//strRetCode = L"\"connected\"";
			g_notification += L"\"connected\"";
			break;
		case wlan_notification_msm_roaming_start:
			//strRetCode = L"\"roaming start\"";
			g_notification += L"\"roaming start\"";
			break;
		case wlan_notification_msm_roaming_end:
			//strRetCode = L"\"roaming end\"";
			g_notification += L"\"roaming end\"";
			break;
		case wlan_notification_msm_radio_state_change:
			//strRetCode = L"\"radio state change\"";
			pPHY_RADIO_STATE = (PWLAN_PHY_RADIO_STATE) pData;
			g_notification += L"\"radio state change\" PhyIndex:" + 
				to_wstring(pPHY_RADIO_STATE->dwPhyIndex);
			if (pPHY_RADIO_STATE->dot11SoftwareRadioState == dot11_radio_state_on)
				g_notification += L" SoftwareRadioState:on ";
			else if (pPHY_RADIO_STATE->dot11SoftwareRadioState == dot11_radio_state_off)
				g_notification += L" SoftwareRadioState:off ";
			else
				g_notification += L" SoftwareRadioState:? ";

			if (pPHY_RADIO_STATE->dot11HardwareRadioState == dot11_radio_state_on)
				g_notification += L"HardwareRadioState:on";
			else if (pPHY_RADIO_STATE->dot11HardwareRadioState == dot11_radio_state_off)
				g_notification += L"HardwareRadioState:off";
			else
				g_notification += L"HardwareRadioState:?";
			
			break;
		case wlan_notification_msm_signal_quality_change:	
			//strRetCode = L"\"signal quality change\"";
			signal_quality = *((ULONG*)pData);
			g_notification += L"\"signal quality change\" " + to_wstring(signal_quality);
			break;
		case wlan_notification_msm_disassociating:
			//strRetCode = L"\"disassociating\"";
			g_notification += L"\"disassociating\"";
			break;
		case wlan_notification_msm_disconnected:
			//strRetCode = L"\"disconnected\"";
			g_notification += L"\"disconnected\"";
			break;
		case wlan_notification_msm_peer_join:
			//strRetCode = L"\"a peer joins the ad hoc network\"";
			g_notification += L"\"a peer joins the ad hoc network\"";
			break;
		case wlan_notification_msm_peer_leave:
			//strRetCode = L"\"a peer leaves the ad hoc network\"";
			g_notification += L"\"a peer leaves the ad hoc network\"";
			break;
		case wlan_notification_msm_adapter_removal:
			//strRetCode = L"\"adapter is in a bad state\"";
			g_notification += L"\"adapter is in a bad state\"";
			break;
		default:
			//strRetCode = L"\"unknown MSM notification\"";
			g_notification += L"\"unknown MSM notification\"";
	}

	return strRetCode;
}

// Notification callback function
VOID WINAPI
NotificationCallback(
    PWLAN_NOTIFICATION_DATA pNotifData, 
    PVOID pContext  // this parameter is not used
)
{
	time_t rawtime;
	struct tm * timeinfo;
	char* time_str;	
	WCHAR GuidString[40] = {0};
	int iRet = 0;
		
	if (pNotifData == NULL)
		return;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	time_str = asctime(timeinfo);
	//strftime(time_str, 64, "%b %d %T", timeinfo);

printf("NotificationCallback() @%s\n", time_str);

	if (g_notification.length() > 8192)
		g_notification = L"";
	
	mbstate_t state = mbstate_t();
	int len = strlen(time_str);
	const char* temp = (const char*) time_str;
	wchar_t wstr[32] = {0};
	std::mbsrtowcs(wstr, &temp, len, &state);
	g_notification += wstr;

	iRet = StringFromGUID2(pNotifData->InterfaceGuid, (LPOLESTR) &GuidString, 39); 
	// For c rather than C++ source code, the above line needs to be
	//iRet = StringFromGUID2(&pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString, 39); 
	if (iRet == 0) {
		//wprintf(L"StringFromGUID2 failed\n");
		g_notification += L"StringFromGUID2 failed<br>\n";//scofield
	}
	else {
		//wprintf(L"	InterfaceGUID[%d]: %ws\n",i, GuidString);
		//g_notification += L" InterfaceGUID:" + GuidString;
		g_notification += L"> InterfaceGUID:";
		g_notification += GuidString;
		
	}	 
	
	g_notification += L" Source:" + to_wstring(pNotifData->NotificationSource);
	g_notification += L" Code:" + to_wstring(pNotifData->NotificationCode) + L"<br>\n";
	
	switch (pNotifData->NotificationSource) {
		case WLAN_NOTIFICATION_SOURCE_ACM:
			//g_notification += L" Source:\t the Auto Configuration Module (ACM)<br>\n";
			g_notification += GetAcmNotificationString(pNotifData->NotificationCode, pNotifData->pData);
			break;

		case WLAN_NOTIFICATION_SOURCE_HNWK:
			g_notification += L" Source:\t the wireless Hosted Network<br>\n";
			break;

		case WLAN_NOTIFICATION_SOURCE_ONEX:
			g_notification += L" Source:\t the 802.1X<br>\n";
			break;

		case WLAN_NOTIFICATION_SOURCE_MSM:
			//g_notification += L" Source:\t the Media Specific Module (MSM)<br>\n";
			//g_notification += GetMsmNotificationString(pNotifData->NotificationCode, pNotifData->pData);
			GetMsmNotificationString(pNotifData->NotificationCode, pNotifData->pData);
			break;

		case WLAN_NOTIFICATION_SOURCE_SECURITY:
			g_notification += L" Source:\t the security module<br>\n";
			break;

		case WLAN_NOTIFICATION_SOURCE_IHV:
			g_notification += L" Source:\t the independent hardware vendors (IHV)<br>\n";
			break;
	}	
	g_notification += L"<br><br>\n";
}

int init_wlan() {
	wstring output = L"";
	HANDLE hClient;
	DWORD dwMaxClient = 2;	 //
	DWORD dwCurVersion = 0;
	//DWORD dwResult = 0;
	unsigned long dwResult = 0;
	DWORD dwPrevNotifType = 0;

//printf("\t%s\n", __FUNCTION__);

	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS)	{
		wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
		output += L"WlanOpenHandle failed with error: " + to_wstring(dwResult) + L"<br>\n";//scofield
	
		g_hClient = NULL;

		// FormatMessage can be used to find out why the function failed
		return 1;
	}

	if (dwCurVersion == 2)
		output += L"WLAN API version:\t for Windows Vista or Server 2008 or above<br>\n";//scofield
	else		
		output += L"WLAN API version:\t "+ to_wstring(dwCurVersion) + L"<br>\n";//scofield

	g_hClient = hClient;

	enumInterfaces(output);

	dwResult = WlanRegisterNotification(hClient, 
		WLAN_NOTIFICATION_SOURCE_ALL,
		FALSE,					// do not ignore duplications
		NotificationCallback,
		NULL,					// no callback context is needed
		NULL,					// reserved
		//&dwPrevNotifType
		NULL
		);
	if (dwResult != ERROR_SUCCESS)	{
		wprintf(L"WlanRegisterNotification failed with error: %u\n", dwResult);
		g_notification += L"WlanRegisterNotification failed with error: " + to_wstring(dwResult) + L"<br>\n";//scofield
	
		return 1;
	}

	isWlanInitiated = true;
	
	return 0;
}


/*int init(HANDLE& hClient, wstring& output) {
	DWORD dwMaxClient = 2;	 //
	DWORD dwCurVersion = 0;
	//DWORD dwResult = 0;
	unsigned long dwResult = 0;
	DWORD dwPrevNotifType = 0;

//printf("\t%s\n", __FUNCTION__);

	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS)	{
		wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
		output += L"WlanOpenHandle failed with error: " + to_wstring(dwResult) + L"<br>\n";//scofield
	
		g_hClient = NULL;

		// FormatMessage can be used to find out why the function failed
		return 1;
	}

	if (dwCurVersion == 2)
		output += L"WLAN API version:\t for Windows Vista or Server 2008 or above<br>\n";//scofield
	else		
		output += L"WLAN API version:\t "+ to_wstring(dwCurVersion) + L"<br>\n";//scofield

	g_hClient = hClient;

	dwResult = WlanRegisterNotification(hClient, 
		WLAN_NOTIFICATION_SOURCE_ALL,
		FALSE,					// do not ignore duplications
		NotificationCallback,
		NULL,					// no callback context is needed
		NULL,					// reserved
		//&dwPrevNotifType
		NULL
		);
	//if (dwResult != ERROR_SUCCESS)	{
		wprintf(L"WlanRegisterNotification failed with error: %u\n", dwResult);
		g_notification += L"WlanRegisterNotification failed with error: " + to_wstring(dwResult) + L"<br>\n";//scofield
	
		//return 1;
	//}
	
	return 0;
}*/

wstring getNotifications() {
	//PWLAN_INTERFACE_INFO pIfInfo;
	//HANDLE hClient;
	//int iRet = 0;
	
	//printf("getNotifications()\n");

	/*if (g_hClient == NULL) {
		iRet = init(hClient, g_notification);	
		if (iRet == 1)
			return L"fail";

		//enumInterfaces(g_notification);
	}
	else {
		hClient = g_hClient;
	}

	pIfInfo = &g_IfInfo;

	

	WlanScan(
                    hClient, 
                    &pIfInfo->InterfaceGuid, 
                    NULL,                   // don't perform additional probe for a specific SSID
                    NULL,                   // no IE data for the additional probe
                    NULL                    // reserved
                    );*/

	if (!isWlanInitiated)
		g_notification = L"Wlan is not initiated successfully<br>\n";
	
	return g_notification;
}


void printEnumInterfaces(wstring& output, PWLAN_INTERFACE_INFO_LIST pIfList) {
	int i;
	int iRet = 0;
	WCHAR GuidString[40] = {0};
	PWLAN_INTERFACE_INFO pIfInfo = NULL;
	
	wprintf(L"Num Entries: %lu\n", pIfList->dwNumberOfItems);
	//output += L"Num Entries: "+ to_wstring(pIfList->dwNumberOfItems) + L"<br>\n";//scofield
	output += L"\"NumInterfaces\":\""+ to_wstring(pIfList->dwNumberOfItems) + L"\"";//scofield

	wprintf(L"Current Index: %lu\n", pIfList->dwIndex);
	//output += L"Current Index: " + to_wstring(pIfList->dwIndex) + L"<br>\n";//scofield
	output += L",\"CurrentIndex\":\"" + to_wstring(pIfList->dwIndex) + L"\"";//scofield

//printf("\n\toutput: \n%s", output);

		//g_IfInfo = pIfList->InterfaceInfo[pIfList->dwIndex];//scofield

		output += L",\"InterfaceInfos\":[";
		for (i = 0; i < (int) pIfList->dwNumberOfItems; i++) {
			pIfInfo = (WLAN_INTERFACE_INFO *) &pIfList->InterfaceInfo[i];
			wprintf(L"	Interface Index[%d]:\t %lu\n", i, i);
			//output += L"  Interface Index[" + to_wstring(i) + L"]:\t " + to_wstring(i) + L"<br>\n";//scofield
			output += L"{\"InterfaceIndex\":\"" + to_wstring(i) + L"\"";//scofield

			iRet = StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString, 39); 
			// For c rather than C++ source code, the above line needs to be
			//iRet = StringFromGUID2(&pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString, 39); 
			if (iRet == 0) {
				wprintf(L"StringFromGUID2 failed\n");
				//s += sprintf(output+s, "StringFromGUID2 failed<br>\n");//scofield
				//output += L"StringFromGUID2 failed<br>\n";//scofield
			}
			else {
				wprintf(L"	InterfaceGUID[%d]: %ws\n",i, GuidString);
				//s += sprintf(output+s, "  InterfaceGUID[%d]: %ws<br>\n",i, GuidString);//scofield
				//output += L"  InterfaceGUID[" + to_wstring(i) + L"]: " + GuidString + L"<br>\n";//scofield
				output += L",\"InterfaceGUID\":\"";
				output += GuidString;
				output += L"\"";
			}	 
			wprintf(L"	Interface Description[%d]: %ws", i, pIfInfo->strInterfaceDescription);
			//s += sprintf(output+s, "  Interface Description[%d]: %ws", i, pIfInfo->strInterfaceDescription);//scofield
			//output += L"  Interface Description[" + to_wstring(i) + L"]: " + pIfInfo->strInterfaceDescription;//scofield
			output += L",\"InterfaceDescription\":\"";
			output += pIfInfo->strInterfaceDescription;
			output += L"\"";
			
			wprintf(L"\n");
			//s += sprintf(output+s, "<br>\n");//scofield
			//output += L"<br>\n";//scofield
			
			wprintf(L"	Interface State[%d]:\t ", i);
			//s += sprintf(output+s, "  Interface State[%d]:\t ", i);//scofield
			//output += L"  Interface State[" + to_wstring(i) + L"]:\t ";//scofield
			output += L",\"InterfaceState\":";
			
			switch (pIfInfo->isState) {
			case wlan_interface_state_not_ready:
				wprintf(L"Not ready\n");
				//s += sprintf(output+s, "Not ready<br>\n");//scofield
				//output += L"Not ready<br>\n";//scofield
				output += L"\"Not ready\"";
				break;
			case wlan_interface_state_connected:
				wprintf(L"Connected\n");
				//s += sprintf(output+s, "Connected<br>\n");//scofield
				//output += L"Connected<br>\n";//scofield
				output += L"\"Connected\"";
				break;
			case wlan_interface_state_ad_hoc_network_formed:
				wprintf(L"First node in a ad hoc network\n");
				//s += sprintf(output+s, "First node in a ad hoc network<br>\n");//scofield
				//output += L"First node in a ad hoc network<br>\n";//scofield
				output += L"\"First node in a ad hoc network\"";
				break;
			case wlan_interface_state_disconnecting:
				wprintf(L"Disconnecting\n");
				//s += sprintf(output+s, "Disconnecting<br>\n");//scofield
				//output += L"Disconnecting<br>\n";//scofield
				output += L"\"Disconnecting\"";
				break;
			case wlan_interface_state_disconnected:
				wprintf(L"Not connected\n");
				//s += sprintf(output+s, "Not connected<br>\n");//scofield
				//output += L"Not connected<br>\n";//scofield
				output += L"\"Not connected\"";
				break;
			case wlan_interface_state_associating:
				wprintf(L"Attempting to associate with a network\n");
				//s += sprintf(output+s, "Attempting to associate with a network<br>\n");//scofield
				//output += L"Attempting to associate with a network<br>\n";//scofield
				output += L"\"Attempting to associate with a network\"";
				break;
			case wlan_interface_state_discovering:
				wprintf(L"Auto configuration is discovering settings for the network\n");
				//s += sprintf(output+s, "Auto configuration is discovering settings for the network<br>\n");//scofield
				//output += L"Auto configuration is discovering settings for the network<br>\n";//scofield
				output += L"\"Auto configuration is discovering settings for the network\"";
				break;
			case wlan_interface_state_authenticating:
				wprintf(L"In process of authenticating\n");
				//s += sprintf(output+s, "In process of authenticating<br>\n");//scofield
				//output += L"In process of authenticating<br>\n";//scofield
				output += L"\"In process of authenticating\"";
				break;
			default:
				wprintf(L"Unknown state %ld\n", pIfInfo->isState);
				//s += sprintf(output+s, "Unknown state %ld<br>\n", pIfInfo->isState);//scofield
				//output += L"Unknown state " + to_wstring(pIfInfo->isState) + L"<br>\n";//scofield
				output += L"\"Unknown state " + to_wstring(pIfInfo->isState) + L"\"";
				break;
			}
			wprintf(L"\n");
			//s += sprintf(output+s, "<br>\n");//scofield
			
			if (pIfList->dwNumberOfItems > 1)
				output += L"},";
			else 
				output += L"}";
		}
		output += L"]";
	}

void getRegionList(wstring& output) {
	PWLAN_COUNTRY_OR_REGION_STRING_LIST pCOUNTRY_OR_REGION_STRING_LIST = NULL;
	DWORD dwDataSize;
	PWLAN_INTERFACE_INFO pIfInfo = &g_IfInfo;
	HANDLE hClient = g_hClient;

	DWORD rc = WlanQueryInterface(hClient, &pIfInfo->InterfaceGuid, 
		wlan_intf_opcode_supported_country_or_region_string_list, NULL, &dwDataSize, 
		(PVOID *)&(pCOUNTRY_OR_REGION_STRING_LIST), NULL);

	if (rc == ERROR_SUCCESS && pCOUNTRY_OR_REGION_STRING_LIST) {
		printf ("number of supported Region: %lu\n", pCOUNTRY_OR_REGION_STRING_LIST->dwNumberOfItems);
		output += L"number of supported Region:\t " + to_wstring(pCOUNTRY_OR_REGION_STRING_LIST->dwNumberOfItems) + L"<br>\n";

		int len = pCOUNTRY_OR_REGION_STRING_LIST->dwNumberOfItems;
		for (int i=0; i<len; i++) {
			mbstate_t state = mbstate_t();
		    int len = 3;
		    const char* temp = (const char*) pCOUNTRY_OR_REGION_STRING_LIST->pCountryOrRegionStringList[i];
		    wchar_t wstr[4] = {0};
		    std::mbsrtowcs(wstr, &temp, len, &state);

			output += L"Region[" + to_wstring(i) + L"]:\t " + 
				wstr + L"<br>\n";
		}
	
		WlanFreeMemory (pCOUNTRY_OR_REGION_STRING_LIST);
	}
	else {
		output += L"error number of supported Regions:\t " + to_wstring(rc) + L"<br>\n";
		switch (rc) {
			case ERROR_ACCESS_DENIED:
				output += L"error:\t ERROR_ACCESS_DENIED<br>\n";
				break;

			//case ERROR_INVALID PARAMETER:
				//output += L"error:\t ERROR_INVALID PARAMETER<br>\n";
				//break;
				
			case ERROR_INVALID_STATE:
				output += L"error:\t ERROR_INVALID_STATE<br>\n";
				break;

			case ERROR_NOT_ENOUGH_MEMORY:
				output += L"error:\t ERROR_NOT_ENOUGH_MEMORY<br>\n";
				break;
		}
	}
}

void getAutoConf(wstring& output) {
	BOOL *autoConf = NULL;
	DWORD dwSize;
	PWLAN_INTERFACE_INFO pIfInfo = &g_IfInfo;
	HANDLE hClient = g_hClient;

	DWORD rc = WlanQueryInterface(hClient, &pIfInfo->InterfaceGuid, 
		wlan_intf_opcode_autoconf_enabled, NULL, &dwSize, (PVOID *)&autoConf, NULL);

	if (rc == ERROR_SUCCESS && autoConf) {
		//printf ("Channel: %lu\n", *channel);
		output += L"Auto Config:\t " + to_wstring(*autoConf) + L"<br>\n";
	
		WlanFreeMemory (autoConf);
	}
}

void setAutoConf(wstring& output, BOOL autoConf) {
	DWORD dwSize = sizeof(BOOL);
	PWLAN_INTERFACE_INFO pIfInfo = &g_IfInfo;
	HANDLE hClient = g_hClient;

	DWORD rc = WlanSetInterface(hClient, &pIfInfo->InterfaceGuid, 
		wlan_intf_opcode_autoconf_enabled, dwSize, (PVOID)&autoConf, NULL);

	if (rc == ERROR_SUCCESS) {
		//printf ("Channel: %lu\n", *channel);
		output += L"Auto Config:\t " + to_wstring(autoConf) + L" set ok<br>\n";
	
		//WlanFreeMemory (autoConf);
	}
	else {
		/*
			ERROR_ACCESS_DENIED
			5 (0x5)
			Access is denied.
		*/
		output += L"Auto Config:\t " + to_wstring(autoConf) + L" set fail with rc=" + 
			to_wstring(rc) + L"<br>\n";
	}
}


void getCurrentChannel(wstring& output) {
	ULONG *channel = NULL;
	DWORD dwSizeChannel = sizeof(*channel);
	PWLAN_INTERFACE_INFO pIfInfo = &g_IfInfo;
	HANDLE hClient = g_hClient;

	DWORD rc = WlanQueryInterface(hClient, &pIfInfo->InterfaceGuid, 
		wlan_intf_opcode_channel_number, NULL, &dwSizeChannel, (PVOID *)&channel, NULL);

	if (rc == ERROR_SUCCESS && channel) {
		printf ("Channel: %lu\n", *channel);
		output += L"Current Channel:\t " + to_wstring(*channel) + L"<br>\n";
	
		WlanFreeMemory (channel);
	}
}

void getCurrentMode(wstring& output) {
	ULONG *mode = NULL;
	DWORD dwSizeMode = sizeof(*mode);
	PWLAN_INTERFACE_INFO pIfInfo = &g_IfInfo;
	HANDLE hClient = g_hClient;

	DWORD rc = WlanQueryInterface(hClient, &pIfInfo->InterfaceGuid, 
		wlan_intf_opcode_current_operation_mode, NULL, &dwSizeMode, (PVOID *)&mode, NULL);

	if (rc == ERROR_SUCCESS && mode) {
		printf ("Mode: %lu\n", *mode);

		/*
		output += L"Current Mode:\t " + to_wstring(*mode) + L"<br>\n";
output += L"WFD_DEVICE:\t " + to_wstring(DOT11_OPERATION_MODE_WFD_DEVICE) + L"<br>\n";
output += L"EXTENSIBLE_STATION:\t " + to_wstring(DOT11_OPERATION_MODE_EXTENSIBLE_STATION) + L"<br>\n";
		*/
		output += L"Current Mode:\t ";
		switch (*mode) {
			case DOT11_OPERATION_MODE_EXTENSIBLE_AP:
				output += L"Extensible Access Point (ExtAP)<br>\n";
				break;

			case DOT11_OPERATION_MODE_EXTENSIBLE_STATION:
				output += L"Extensible Station (ExtSTA)<br>\n";
				break;

			default:
				output += to_wstring(DOT11_OPERATION_MODE_WFD_DEVICE) + L"<br>\n";
				
		}
	
		WlanFreeMemory (mode);
	}
}


int enumInterfaces(wstring& output) {
	// Declare and initialize variables.
	HANDLE hClient = NULL;
	//DWORD dwResult = 0;
	unsigned long dwResult = 0;
	int iRet = 0;

	/* variables used for WlanEnumInterfaces  */
	PWLAN_INTERFACE_INFO_LIST pIfList = NULL;

	hClient = g_hClient;
	
	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList); 
	if (dwResult != ERROR_SUCCESS)	{
		wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
		output += L"WlanEnumInterfaces failed with error: "+ to_wstring(dwResult) + L"<br>\n";//scofield

		// FormatMessage can be used to find out why the function failed
		return 1;
	}

	g_IfInfo = pIfList->InterfaceInfo[pIfList->dwIndex];//scofield

	output = L"{";
	printEnumInterfaces(output, pIfList);
	output += L"}";

	if (pIfList != NULL) {
		WlanFreeMemory(pIfList);
		pIfList = NULL;
	}

	//getCurrentChannel(output);
	//getCurrentMode(output);
	//getRegionList(output);
	//getAutoConf(output);
	
	//setAutoConf(output, true);
	//getAutoConf(output);
	
	return 0;
}

void printAvailableNetworkList(wstring& output, PWLAN_AVAILABLE_NETWORK_LIST pBssList) {
	unsigned int j, k;
	PWLAN_AVAILABLE_NETWORK pBssEntry = NULL;
	int iRSSI = 0;
	
	wprintf(L"WLAN_AVAILABLE_NETWORK_LIST for this interface\n");
	//output += L"WLAN_AVAILABLE_NETWORK_LIST for this interface<br>\n";//scofield

	wprintf(L"	Num Entries: %lu\n\n", pBssList->dwNumberOfItems);
	output += L"	Num Entries: " + to_wstring(pBssList->dwNumberOfItems) + L"<br>\n\n";//scofield

	wstring _j = L"";
	for (j = 0; j < pBssList->dwNumberOfItems; j++) {
		_j = to_wstring(j);
	
		pBssEntry = (WLAN_AVAILABLE_NETWORK *) & pBssList->Network[j];

		wprintf(L"	Profile Name[%u]:  %ws\n", j, pBssEntry->strProfileName);
		output += L"	Profile Name[" + _j + L"]:  " + pBssEntry->strProfileName + L"<br>\n";//scofield
					
		wprintf(L"	SSID[%u]:\t\t ", j);
		output += L"	SSID[" + _j + L"]:\t\t ";//scofield
		
		if (pBssEntry->dot11Ssid.uSSIDLength == 0) {
			wprintf(L"\n");
			output += L"<br>\n";//scofield
		}
		else {	 
			for (k = 0; k < pBssEntry->dot11Ssid.uSSIDLength; k++) {
				wprintf(L"%c", (int) pBssEntry->dot11Ssid.ucSSID[k]);
			}
			/*
			std::size_t mbsrtowcs( wchar_t* dst,
                       const char** src,
                       std::size_t len,
                       std::mbstate_t* ps );
			*/
			mbstate_t state = mbstate_t();
		    int len = pBssEntry->dot11Ssid.uSSIDLength;
		    const char* temp = (const char*) pBssEntry->dot11Ssid.ucSSID;
		    wchar_t wstr[32] = {0};
		    std::mbsrtowcs(wstr, &temp, len, &state);
			output += wstr;//scofield
//wprintf(L"\nwstr: %s \n", wstr);			
			
			wprintf(L"\n");
			output += L"<br>\n";//scofield
		}
						
		wprintf(L"	BSS Network type[%u]:\t ", j);
		output += L"	BSS Network type[" + _j + L"]:\t ";//scofield
		
		switch (pBssEntry->dot11BssType) {
			case dot11_BSS_type_infrastructure	 :
				wprintf(L"Infrastructure (%u)\n", pBssEntry->dot11BssType);
				output += L"Infrastructure (" + to_wstring(pBssEntry->dot11BssType) + L")<br>\n";//scofield
				break;
			case dot11_BSS_type_independent:
				wprintf(L"Infrastructure (%u)\n", pBssEntry->dot11BssType);
				output += L"Infrastructure (" + to_wstring(pBssEntry->dot11BssType) + L")<br>\n";//scofield
				break;
			default:
				wprintf(L"Other (%lu)\n", pBssEntry->dot11BssType);
				output += L"Other (" + to_wstring(pBssEntry->dot11BssType) + L")<br>\n";//scofield
				break;
		}
								
		wprintf(L"	Number of BSSIDs[%u]:\t %u\n", j, pBssEntry->uNumberOfBssids);
		output += L"	Number of BSSIDs[" + _j + L"]:\t " + 
			to_wstring(pBssEntry->uNumberOfBssids) + L"<br>\n";//scofield		

		wprintf(L"	Connectable[%u]:\t ", j);
		output += L"	Connectable[" + _j + L"]:\t ";//scofield
		
		if (pBssEntry->bNetworkConnectable) {
			wprintf(L"Yes\n");
			output += L"Yes<br>\n";//scofield
		}
		else {
			wprintf(L"No\n");
			output += L"No<br>\n";//scofield
			
			wprintf(L"	Not connectable WLAN_REASON_CODE value[%u]:\t %u\n", 
				j, pBssEntry->wlanNotConnectableReason);
			output += L"	Not connectable WLAN_REASON_CODE value[" + _j + 
				L"]:\t " + to_wstring(pBssEntry->wlanNotConnectableReason) + L"<br>\n";//scofield
		}		 

		wprintf(L"	Number of PHY types supported[%u]:\t %u\n", j, pBssEntry->uNumberOfPhyTypes);
		output += L"	Number of PHY types supported[" + _j + L"]:\t " +
			to_wstring(pBssEntry->uNumberOfPhyTypes) + L"<br>\n";//scofield

		if (pBssEntry->wlanSignalQuality == 0)
			iRSSI = -100;
		else if (pBssEntry->wlanSignalQuality == 100)	
			iRSSI = -50;
		else
			iRSSI = -100 + (pBssEntry->wlanSignalQuality/2);	
						
		wprintf(L"	Signal Quality[%u]:\t %u (RSSI: %i dBm)\n", j, 
			pBssEntry->wlanSignalQuality, iRSSI);
		output += L"	Signal Quality[" + _j + L"]:\t " + 
			to_wstring(pBssEntry->wlanSignalQuality) + L"<br>\n";
		output += L"	RSSI[" + _j + L"]:\t " + to_wstring(iRSSI) + L" dBm<br>\n";//scofield

		wprintf(L"	Security Enabled[%u]:\t ", j);
		output += L"	Security Enabled[" + _j + L"]:\t ";//scofield
		
		if (pBssEntry->bSecurityEnabled) {
			wprintf(L"Yes\n");
			output += L"Yes<br>\n";//scofield
		}
		else {
			wprintf(L"No\n");
			output += L"No<br>\n";//scofield
		}

		wprintf(L"	Default AuthAlgorithm[%u]: ", j);
		output += L"	Default AuthAlgorithm[" + _j + L"]: ";//scofield
		
		switch (pBssEntry->dot11DefaultAuthAlgorithm) {
			case DOT11_AUTH_ALGO_80211_OPEN:
				wprintf(L"802.11 Open (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
				output += L"802.11 Open (" + to_wstring(pBssEntry->dot11DefaultAuthAlgorithm) + L")<br>\n";//scofield
				break;
			case DOT11_AUTH_ALGO_80211_SHARED_KEY:
				wprintf(L"802.11 Shared (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
				output += L"802.11 Shared (" + to_wstring(pBssEntry->dot11DefaultAuthAlgorithm) + L")<br>\n";//scofield
				break;
			case DOT11_AUTH_ALGO_WPA:
				wprintf(L"WPA (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
				output += L"WPA (" + to_wstring(pBssEntry->dot11DefaultAuthAlgorithm) + L")<br>\n";//scofield
				break;
			case DOT11_AUTH_ALGO_WPA_PSK:
				wprintf(L"WPA-PSK (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
				output += L"WPA-PSK (" + to_wstring(pBssEntry->dot11DefaultAuthAlgorithm) + L")<br>\n";//scofield
				break;
			case DOT11_AUTH_ALGO_WPA_NONE:
				wprintf(L"WPA-None (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
				output += L"WPA-None (" + to_wstring(pBssEntry->dot11DefaultAuthAlgorithm) + L")<br>\n";//scofield
				break;
			case DOT11_AUTH_ALGO_RSNA:
				wprintf(L"RSNA (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
				output += L"RSNA (" + to_wstring(pBssEntry->dot11DefaultAuthAlgorithm) + L")<br>\n";//scofield
				break;
			case DOT11_AUTH_ALGO_RSNA_PSK:
				wprintf(L"RSNA with PSK(%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
				output += L"RSNA with PSK(" + to_wstring(pBssEntry->dot11DefaultAuthAlgorithm) + L")<br>\n";//scofield
				break;
			default:
				wprintf(L"Other (%lu)\n", pBssEntry->dot11DefaultAuthAlgorithm);
				output += L"Other (" + to_wstring(pBssEntry->dot11DefaultAuthAlgorithm) + L"<br>\n";//scofield
				break;
		}
						
		wprintf(L"	Default CipherAlgorithm[%u]: ", j);
		output += L"	Default CipherAlgorithm[" + _j + L"]: ";//scofield
		
		switch (pBssEntry->dot11DefaultCipherAlgorithm) {
			case DOT11_CIPHER_ALGO_NONE:
				wprintf(L"None (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
				output += L"None (0x" + to_wstring(pBssEntry->dot11DefaultCipherAlgorithm) + L")<br>\n";//scofield
				break;
			case DOT11_CIPHER_ALGO_WEP40:
				wprintf(L"WEP-40 (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
				output += L"WEP-40 (0x" + to_wstring(pBssEntry->dot11DefaultCipherAlgorithm) + L")<br>\n";//scofield
				break;
			case DOT11_CIPHER_ALGO_TKIP:
				wprintf(L"TKIP (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
				output += L"TKIP (0x" + to_wstring(pBssEntry->dot11DefaultCipherAlgorithm) + L")<br>\n";//scofield
				break;
			case DOT11_CIPHER_ALGO_CCMP:
				wprintf(L"CCMP (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
				output += L"CCMP (0x" + to_wstring(pBssEntry->dot11DefaultCipherAlgorithm) + L")<br>\n";//scofield
				break;
			case DOT11_CIPHER_ALGO_WEP104:
				wprintf(L"WEP-104 (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
				output += L"WEP-104 (0x" + to_wstring(pBssEntry->dot11DefaultCipherAlgorithm) + L")<br>\n";//scofield
				break;
			case DOT11_CIPHER_ALGO_WEP:
				wprintf(L"WEP (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
				output += L"WEP (0x" + to_wstring(pBssEntry->dot11DefaultCipherAlgorithm) + L")<br>\n";//scofield
				break;
			default:
				wprintf(L"Other (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
				output += L"Other (0x" + to_wstring(pBssEntry->dot11DefaultCipherAlgorithm) + L")<br>\n";//scofield
				break;
		}

		wprintf(L"	Flags[%u]:\t 0x%x", j, pBssEntry->dwFlags);
		output += L"	Flags[" + _j + L"]:\t 0x" + to_wstring(pBssEntry->dwFlags);//scofield
		
		if (pBssEntry->dwFlags) {
			if (pBssEntry->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED) {
				wprintf(L" - Currently connected");
				output += L" - Currently connected";//scofield
			}
			
			if (pBssEntry->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED) {
				wprintf(L" - Has profile");
				output += L" - Has profile";//scofield
			}
		}
		
		wprintf(L"\n");
		output += L"<br>\n";//scofield
			
		wprintf(L"\n");
		output += L"<br>\n";//scofield
	}
}

int getAvailableNetworkList(wstring& output) {
	HANDLE hClient = NULL;
	DWORD dwResult = 0;
	int iRet = 0;
	DWORD dwRetVal = 0;
	PWLAN_INTERFACE_INFO pIfInfo = NULL;
	PWLAN_AVAILABLE_NETWORK_LIST pBssList = NULL;
	wstring _tmp = L"";

	if (!isWlanInitiated) {
		output = L"Wlan is not initiated successfully<br>\n";
		return 1;
	}
	
	hClient = g_hClient;
	pIfInfo = &g_IfInfo;

	dwResult = WlanGetAvailableNetworkList(hClient, &pIfInfo->InterfaceGuid,
		0, NULL, &pBssList);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanGetAvailableNetworkList failed with error: %u\n", dwResult);
		output += L"WlanGetAvailableNetworkList failed with error: " + to_wstring(dwResult) + L"<br>\n";//scofield

		/*
		https://msdn.microsoft.com/en-us/library/windows/desktop/ms681382(v=vs.85).aspx
		*/
		if (dwResult == ERROR_INVALID_PARAMETER)
			output += L"WlanGetAvailableNetworkList failed with error: 'The parameter is incorrect'<br>\n";//scofield
		else if (dwResult == ERROR_NOT_FOUND)
			output += L"WlanGetAvailableNetworkList failed with error: 'Element not found'<br>\n";//scofield
		else
			output += L"WlanGetAvailableNetworkList failed with error: " + to_wstring(dwResult) + L"<br>\n";//scofield			
		
		dwRetVal = 1;
		// You can use FormatMessage to find out why the function failed
		return dwRetVal;
	}

	printAvailableNetworkList(output, pBssList);

	dwRetVal = 0;
	return dwRetVal;
}


