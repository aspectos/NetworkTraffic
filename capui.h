// Copyright 2021, Pejman Taslimi
//  You may distribute under the terms of either the GNU General Public
//  License or the Artistic License, as specified in the MYLICENSE file.
/*
 * FILENAME     :	
 * DESCRIPTION  :	
 * 
 * Author		    : Mani Tasl
 * Date         : Jun 2021.
 *
 */
#include<string>
#include<codecvt>
#include<locale>
#include<vector>

using convert_t = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_t, wchar_t> strconverter;

std::string to_string(std::wstring wstr){
    return strconverter.to_bytes(wstr);
}

std::wstring to_wstring(std::string str){
    return strconverter.from_bytes(str);
}

struct sLabels{
  int iInterface = 0, iTraffic = 0, 
      iSoftware = 0, iVpn = 0, iOs = 0, iInternet = 0;
};

struct sStatus{
  int iPacketCount, iDurationSec, iCaptureTime;
  std::wstring wCaptureID, wCaptureTime, wFilename, wStatus;
  sLabels Labels;
};

std::vector<std::wstring> Label_Interface;

std::vector<std::wstring> Label_Traffic = {
  L"WebBrowse",
  L"InstaBrowse",
  L"SendEmail",
  L"FileDL",
  L"TorrentDL",
  L"WatchVideo",
  L"TextMessage",
  L"TextChat",
  L"VoiceCall",
  L"VideoCall",
  L"NoTask",
};
std::vector<std::wstring> Label_Software = {
  L"Firefox",
  L"Chrome",
  L"Instagram",
  L"qBittorrent",
  L"Youtube",
  L"Whatsapp",
  L"Imo",
  L"Skype",
  L"Zoom",
};
std::vector<std::wstring> Label_VPN = {
  L"None",
  L"Lentern",
  L"HotspotShield",
  L"TunnelBear",
  L"UltraSurf",
};
std::vector<std::wstring> Label_OS = {
  L"Win10",
  L"Win7",
  L"Fedroa",
  L"Android",
  L"IOS",
  L"AndrEmuWin10",
  L"AndrEmuWin7",
  L"AndrEmuFedora",
};
std::vector<std::wstring> Label_Internet = {
  L"MCI",
  L"Irancell",
  L"AdslParsonline",
  L"AdslShatel",
  L"AdslTCl",
  L"FibreTCI",
  L"ITRC"
};

