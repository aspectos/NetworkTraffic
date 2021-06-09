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
#include <memory>  // for allocator, __shared_ptr_access
#include <string>  // for wstring, basic_string
#include <vector>  // for vector
#include <thread>  // for sleep_for
#include <sstream> // for stringstream

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Toggle, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/button.hpp"            // for ButtonBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, vbox, Element

#include "captureFuncs.h"

struct sLabels{
  int iInterface = 0, iTraffic = 0, iSoftware = 0, iVpn = 0, iOs = 0, iInternet = 0;
};

struct sStatus{
  int iPacketCount, iDurationSec;
  std::wstring wCaptureID, wFilename, wStatus;
  sLabels Labels;
};

sStatus gsStatus = {.iDurationSec = 15, .wStatus = L"idle"};

auto screen = ftxui::ScreenInteractive::TerminalOutput();
ftxui::Component renderer;

void ButtonRunHandler(void){
  std::thread update([]() {
    gsStatus.wStatus = L"capturing";
    gsStatus.wFilename = Label_Traffic[gsStatus.Labels.iTraffic];
    for (int i=0;i<50;++i) {
      // using namespace std::chrono_literals;
      std::this_thread::sleep_for(std::chrono::milliseconds{70});
      gsStatus.iDurationSec++;
      if(gsStatus.iDurationSec>25)
        gsStatus.iDurationSec =1;
      screen.PostEvent(ftxui::Event::Custom);
    }
    gsStatus.wStatus = L"idle";
    gsStatus.wCaptureID = L"empty";
  });
  std::wstringstream oss;
	oss << std::this_thread::get_id();
	gsStatus.wCaptureID = oss.str();
  update.detach();
}

void ButtonDiscardHandler(void){
  Label_Interface = myCap.getIfNames();
  // int i;
  // for(auto& wIfName: myCap.getIfNames())
  //     std::cout <<++i <<". " <<wIfName <<std::endl;

}

PcapLib myCap;

int main(int argc, const char* argv[]) {

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
        ftxui::hbox(ftxui::text(L" * Traffic type                : "), toggleTraffic->Render()),
        ftxui::hbox(ftxui::text(L" * Software                    : "), toggleSoftware->Render()),
        ftxui::hbox(ftxui::text(L" * VPN App                     : "), toggleVpn->Render()),
        ftxui::hbox(ftxui::text(L" * OS                          : "), toggleOs->Render()),
        ftxui::hbox(ftxui::text(L" * Internet Provider           : "), toggleInternet->Render()),
        ftxui::separator(),

        // ftxui::hbox(ftxui::color(ftxui::Color::RedLight,ftxui::text(L" * Status                   : ")), StatusText(gsStatus.iDurationSec)),
        ftxui::hbox(ftxui::color(ftxui::Color::RedLight,ftxui::text(L" * Status                   : ")),
                    ftxui::color(ftxui::Color::RedLight,ftxui::text(gsStatus.wStatus)) | size(ftxui::WIDTH, ftxui::EQUAL, 10),
                    ftxui::color(ftxui::Color::RedLight, ftxui::spinner(18 , gsStatus.iDurationSec))
        ),
        ftxui::hbox(ftxui::color(ftxui::Color::Green1,    ftxui::text(L" * Capture ID               : ")),
                    ftxui::color(ftxui::Color::Green1,    ftxui::text(gsStatus.wCaptureID))),
        ftxui::hbox(ftxui::color(ftxui::Color::DeepPink1, ftxui::text(L" * Filename                 : ")),
                    ftxui::color(ftxui::Color::DeepPink1, ftxui::text(gsStatus.wFilename))),
        ftxui::hbox(ftxui::color(ftxui::Color::Blue,      ftxui::text(L" * Zeit (Seconds)           : ")),
                    ftxui::color(ftxui::Color::Blue,      ftxui::text(std::to_wstring(gsStatus.iDurationSec)))),
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
}

