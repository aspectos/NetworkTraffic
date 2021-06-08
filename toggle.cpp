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

// using namespace ftxui;

int slider_value = 15;
std::wstring captureID;
std::wstring status = L"Waiting";
auto screen = ftxui::ScreenInteractive::TerminalOutput();
ftxui::Component renderer;

void ButtonRunHandler(void){
  std::thread update([]() {
    status = L"capturing";
    for (int i=0;i<50;++i) {
      // using namespace std::chrono_literals;
      std::this_thread::sleep_for(std::chrono::milliseconds{70});
      slider_value++;
      if(slider_value>25)
        slider_value =1;
      screen.PostEvent(ftxui::Event::Custom);
    }
    status = L"idle";
    captureID = L"empty";
  });
  std::wstringstream oss;
	oss << std::this_thread::get_id();
	captureID = oss.str();
  update.detach();

  /*  // std::cout <<"Run dear\n" <<std::flush;;
    std::string reset_position;
    for(int i=0; i<3; ++i){
        std::this_thread::sleep_for(std::chrono::milliseconds{1200});
        slider_value++;
        // std::cout << reset_position;
        screen.Print();
        // reset_position = screen.ResetPosition();
        std::cout <<screen.ResetPosition() <<std::flush;
        // screen.Draw(renderer);
        // screen.Print
    }*/
}

int main(int argc, const char* argv[]) {

//   auto screen = ftxui::ScreenInteractive::TerminalOutput();

  std::vector<std::wstring> toggle_1_entries = {
      L"On",
      L"Off",
  };
  std::vector<std::wstring> toggle_2_entries = {
      L"Enabled",
      L"Disabled",
  };
  std::vector<std::wstring> toggle_3_entries = {
      L"10€",
      L"0€",
  };
  std::vector<std::wstring> toggle_4_entries = {
      L"Nothing",
      L"One element",
      L"Several elements",
  };

  int toggle_1_selected = 0;
  int toggle_2_selected = 0;
  int toggle_3_selected = 0;
  int toggle_4_selected = 0;
  ftxui::Component toggle_1 = ftxui::Toggle(&toggle_1_entries, &toggle_1_selected);
  ftxui::Component toggle_2 = ftxui::Toggle(&toggle_2_entries, &toggle_2_selected);
  ftxui::Component toggle_3 = ftxui::Toggle(&toggle_3_entries, &toggle_3_selected);
  ftxui::Component toggle_4 = ftxui::Toggle(&toggle_4_entries, &toggle_4_selected);

  // -- Slider -----------------------------------------------------------------
//   int slider_value = 15;
//   int slider_value_2 = 56;
//   int slider_value_3 = 128;
  auto slider = ftxui::Container::Vertical({
      ftxui::Slider(L"Time: ", &slider_value, 0, 60, 1),
    //   Slider(L"G:", &slider_value_2, 0, 256, 1),
    //   Slider(L"B:", &slider_value_3, 0, 256, 1),
  });

  // -- Button -----------------------------------------------------------------
  std::wstring button_label_exit = L"Quit";
  std::wstring button_label_run = L"Run";
  std::function<void()> on_button_clicked_;
  auto exitButton = ftxui::Button(&button_label_exit, screen.ExitLoopClosure());
  auto runButton = ftxui::Button(&button_label_run, &ButtonRunHandler);
//   exitButton = ftxui::Wrap(L"Button", exitButton);

  auto container = ftxui::Container::Vertical({
      toggle_1,
      toggle_2,
      toggle_3,
      toggle_4,
      slider,
      runButton,
      exitButton,
  });
  
  // auto 
  renderer = ftxui::Renderer(container, [&] {
    return ftxui::vbox({
        // ftxui::text(L"Choose your options:"),
        // ftxui::text(L""),
        ftxui::hbox(ftxui::text(L" * Poweroff on startup      : "), toggle_1->Render()),
        ftxui::hbox(ftxui::text(L" * Out of process           : "), toggle_2->Render()),
        ftxui::hbox(ftxui::text(L" * Price of the information : "), toggle_3->Render()),
        ftxui::hbox(ftxui::text(L" * Number of elements       : "), toggle_4->Render()),
        ftxui::separator(),

        // ftxui::hbox(ftxui::color(ftxui::Color::RedLight,ftxui::text(L" * Status                   : ")), StatusText(slider_value)),
        ftxui::hbox(ftxui::color(ftxui::Color::RedLight,ftxui::text(L" * Status                   : ")),
                    ftxui::color(ftxui::Color::RedLight,ftxui::text(status)) | size(ftxui::WIDTH, ftxui::EQUAL, 10),
                    ftxui::color(ftxui::Color::RedLight, ftxui::spinner(18 , slider_value))
        ),
        ftxui::hbox(ftxui::color(ftxui::Color::Green1,ftxui::text(L" * Capture ID               : ")),
                    ftxui::color(ftxui::Color::Green1,ftxui::text(captureID))),
        ftxui::hbox(ftxui::color(ftxui::Color::Blue,  ftxui::text(L" * Zeit                     : ")),
                    ftxui::color(ftxui::Color::Blue,  ftxui::text(std::to_wstring(slider_value)))),
        ftxui::hbox(ftxui::color(ftxui::Color::Green, ftxui::text(L" * Captured Packets         : ")),
                    ftxui::color(ftxui::Color::Green, ftxui::text(std::to_wstring(slider_value)))),
        ftxui::separator(),
        slider->Render(),
        runButton->Render(),
        exitButton->Render(),
    }) /*| ftxui::border*/ | ftxui::size(ftxui::WIDTH, ftxui::LESS_THAN, 80);
  });

  // std::thread update([&screen, &shift]() {
  std::thread update([]() {
    for (int i=0;i<4;++i) {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(0.5s);
      slider_value++;
      if(slider_value>25)
        slider_value =1;
      screen.PostEvent(ftxui::Event::Custom);
    }
    status = L"idle";
  });
  update.detach();

  screen.Loop(renderer);
}

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
