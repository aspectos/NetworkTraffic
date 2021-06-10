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
#include "capui.h"
#include<memory>      // for allocator, __shared_ptr_access
#include<string>      // for wstring, basic_string
#include<vector>      // for vector
#include<thread>      // for sleep_for
#include<sstream>     // for stringstream
#include<algorithm>   // for std::max
// #include<iomanip>     //
#include<fstream>     // for std::ofstream

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Toggle, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/button.hpp"            // for ButtonBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, vbox, Element

#include "captureFuncs.h"

sStatus gsStatus = {.iDurationSec = 5, .wStatus = L"idle"};

auto screen = ftxui::ScreenInteractive::TerminalOutput();
ftxui::Component renderer;

PcapLib myCap;
std::ofstream myCsv;
std::ofstream myDiscard;

void ButtonRunHandler(void){
  if(gsStatus.wStatus != L"idle"){
    gsStatus.wStatus = L"busy, wait:" + std::to_wstring(gsStatus.iDurationSec - gsStatus.iCaptureTime);
    return;
  }
  gsStatus.wStatus = L"capturing";
  gsStatus.iPacketCount = 0;
  gsStatus.iCaptureTime = 0;
  std::time_t tNow = std::time(nullptr);
  tm* tmNow = localtime(&tNow);

  std::thread thrCap([&]() {
    gsStatus.wFilename = to_wstring(std::to_string(tNow) + ".pcap");
    
    wchar_t wstr[100];
    std::wcsftime(wstr, 100, L"%F %T %Z", tmNow);
    gsStatus.wCaptureTime = wstr;

    // gsStatus.wFilename = Label_Traffic[gsStatus.Labels.iTraffic];
    myCsv <<to_string(gsStatus.wFilename) <<"," 
          <<to_string(Label_Traffic[gsStatus.Labels.iTraffic]) <<","
          <<to_string(Label_Software[gsStatus.Labels.iSoftware]) <<","
          <<to_string(Label_VPN[gsStatus.Labels.iVpn]) <<","
          <<to_string(Label_OS[gsStatus.Labels.iOs]) <<","
          <<to_string(Label_Internet[gsStatus.Labels.iInternet]) <<","
          <<to_string(gsStatus.wCaptureTime)
          <<std::endl;

    myCap.captureInit(myCap.mvwIfNames[gsStatus.Labels.iInterface].c_str(), to_string(gsStatus.wFilename));
    myCap.captureFor(gsStatus.iDurationSec, [](int iCount){
      gsStatus.iPacketCount += iCount;
      // screen.PostEvent(ftxui::Event::Custom);
        // if(iCount)
        //     std::cout <<iCount <<" packets captured. (in main lambda)\n";
    });

  });
  thrCap.detach();

  std::thread thrUpdate([tNow]() {
    // for (int i=0;i<0;++i) {
    while(gsStatus.iCaptureTime < gsStatus.iDurationSec){
      // using namespace std::chrono_literals;
      std::this_thread::sleep_for(std::chrono::milliseconds{100});
      // gsStatus.iCaptureTime = tNow;
      gsStatus.iCaptureTime = (int)(std::time(nullptr) - tNow);
      screen.PostEvent(ftxui::Event::Custom);

      // gsStatus.iDurationSec++;
      // if(gsStatus.iDurationSec>25)
      //   gsStatus.iDurationSec =1;
    }
    gsStatus.wStatus = L"idle";
    gsStatus.wCaptureID = L"empty";
  });
  thrUpdate.detach();

  std::wstringstream oss;
	oss << std::this_thread::get_id();
	gsStatus.wCaptureID = oss.str();
  // update.detach();
}

void ButtonDiscardHandler(void){
  if(gsStatus.wFilename.empty() || gsStatus.wFilename == L"File Discarded")
      return;
    myDiscard <<to_string(gsStatus.wFilename) <<std::endl;
    gsStatus.wFilename = L"File Discarded";

}

int main(int argc, const char* argv[]) {

  myCap.getIfNames();
  myCsv.open("datalog.csv", std::ofstream::out | std::ofstream::app);
  myDiscard.open("discard.csv", std::ofstream::out | std::ofstream::app);

  int n = myCap.mvwIfNames.size();
  if(n>7){
    Label_Interface.resize(7);
    std::transform (myCap.mvwIfNames.begin(), std::next(myCap.mvwIfNames.begin(),7), Label_Interface.begin(), to_wstring);
  } else {
    Label_Interface.resize(n);
    std::transform (myCap.mvwIfNames.begin(), myCap.mvwIfNames.end(), Label_Interface.begin(), to_wstring);
  }

  ftxui::Component toggleInterface = ftxui::Toggle(&Label_Interface, &gsStatus.Labels.iInterface);
  ftxui::Component toggleTraffic = ftxui::Toggle(&Label_Traffic, &gsStatus.Labels.iTraffic);
  ftxui::Component toggleSoftware = ftxui::Toggle(&Label_Software, &gsStatus.Labels.iSoftware);
  ftxui::Component toggleVpn = ftxui::Toggle(&Label_VPN, &gsStatus.Labels.iVpn);
  ftxui::Component toggleOs = ftxui::Toggle(&Label_OS, &gsStatus.Labels.iOs);
  ftxui::Component toggleInternet = ftxui::Toggle(&Label_Internet, &gsStatus.Labels.iInternet);

  auto slider = ftxui::Container::Vertical({
      ftxui::Slider(L"Time: ", &gsStatus.iDurationSec, 0, 60, 1),
    //   Slider(L"G:", &slider_value_2, 0, 256, 1),
    //   Slider(L"B:", &slider_value_3, 0, 256, 1),
  });

  // std::wstring button_label_exit = L"Quit";
  // std::wstring button_label_run = L"Run";
  std::function<void()> on_button_clicked_;
  auto exitButton = ftxui::Button(L"   Quit", screen.ExitLoopClosure()); //&button_label_exit
  auto discardButton = ftxui::Button(L"   Discard Last", &ButtonDiscardHandler); //&button_label_exit
  auto runButton = ftxui::Button(L"   Run", &ButtonRunHandler); //&button_label_run
//   exitButton = ftxui::Wrap(L"Button", exitButton);
//   auto flags_win = window(text(L"Flags"), flags->Render());
  auto ButtonsContainer = ftxui::Container::Horizontal({
      runButton,
      discardButton,
      exitButton,
  });
  auto container = ftxui::Container::Vertical({
      toggleInterface,
      toggleTraffic,
      toggleSoftware,
      toggleVpn,
      toggleOs,
      toggleInternet,
      slider,
      ButtonsContainer,
      // runButton,
      // exitButton,
  });
  
  renderer = ftxui::Renderer(container, [&] {
    return ftxui::vbox({
        // ftxui::text(L"Choose your options:"),
        // ftxui::text(L""),
        ftxui::hbox(ftxui::text(L" * Network Interface           : "), toggleInterface->Render()),
        ftxui::hbox(ftxui::text(L" * Traffic type                : "), toggleTraffic->Render()),
        ftxui::hbox(ftxui::text(L" * Software                    : "), toggleSoftware->Render()),
        ftxui::hbox(ftxui::text(L" * VPN App                     : "), toggleVpn->Render()),
        ftxui::hbox(ftxui::text(L" * OS                          : "), toggleOs->Render()),
        ftxui::hbox(ftxui::text(L" * Internet Provider           : "), toggleInternet->Render()),
        ftxui::separator(),

        // ftxui::hbox(ftxui::color(ftxui::Color::RedLight,ftxui::text(L" * Status                   : ")), StatusText(gsStatus.iDurationSec)),
        ftxui::hbox(ftxui::color(ftxui::Color::RedLight,  ftxui::text(L" * Status                   : ")),
                    ftxui::color(ftxui::Color::RedLight,  ftxui::text(gsStatus.wStatus)) | size(ftxui::WIDTH, ftxui::EQUAL, 10)
                    // ftxui::color(ftxui::Color::RedLight, ftxui::spinner(18 , gsStatus.iDurationSec))
        ),
        ftxui::hbox(ftxui::color(ftxui::Color::Green1,    ftxui::text(L" * Capture ID               : ")),
                    ftxui::color(ftxui::Color::Green1,    ftxui::text(gsStatus.wCaptureID))),
        ftxui::hbox(ftxui::color(ftxui::Color::DeepPink1, ftxui::text(L" * Capture Time             : ")),
                    ftxui::color(ftxui::Color::DeepPink1, ftxui::text(gsStatus.wCaptureTime))),
        ftxui::hbox(ftxui::color(ftxui::Color::DeepPink1, ftxui::text(L" * Filename                 : ")),
                    ftxui::color(ftxui::Color::DeepPink1, ftxui::text(gsStatus.wFilename))),
        ftxui::hbox(ftxui::color(ftxui::Color::Blue,      ftxui::text(L" * Duration (Sec)           : ")),
                    ftxui::color(ftxui::Color::Blue,      ftxui::text(std::to_wstring(gsStatus.iDurationSec)))),
        ftxui::hbox(ftxui::color(ftxui::Color::Green,     ftxui::text(L" * Capture Time (Sec)       : ")),
                    ftxui::color(ftxui::Color::Green,     ftxui::text(std::to_wstring(gsStatus.iCaptureTime)))),
        ftxui::hbox(ftxui::color(ftxui::Color::Green,     ftxui::text(L" * Packet Count             : ")),
                    ftxui::color(ftxui::Color::Green,     ftxui::text(std::to_wstring(gsStatus.iPacketCount)))),
        ftxui::separator(),
        slider->Render(),
        ftxui::hbox({
          runButton->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 20),
          discardButton->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 20),
          exitButton->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 20),
        }),
        // runButton->Render(),
        // exitButton->Render(),
    }) /*| ftxui::border*/ | ftxui::size(ftxui::WIDTH, ftxui::LESS_THAN, 140);
  });

  // renderer = CatchEvent(renderer, [&](ftxui::Event event) {
  // for (auto& it : event.input())
  //   gwAppStatus = L" " + std::to_wstring((unsigned int)it);
  //   //  = std::to_wstring(event.input());
  //   return true;
  // });

  // // std::thread update([&screen, &shift]() {
  // std::thread update([]() {
  //   for (int i=0;i<4;++i) {
  //     using namespace std::chrono_literals;
  //     std::this_thread::sleep_for(0.5s);
  //     slider_value++;
  //     if(slider_value>25)
  //       slider_value =1;
  //     screen.PostEvent(ftxui::Event::Custom);
  //   }
  //   status = L"idle";
  // });
  // update.detach();

  screen.Loop(renderer);
  myCsv.close();
}

