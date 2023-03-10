#include "hello_imgui/hello_imgui.h"
#include "imgui_internal.h"
#include "myAds.h"
#include "JAKAZuRobot.h"
#include "jktypes.h"

#define PI 3.1415926

#ifdef HELLOIMGUI_USE_SDL_OPENGL3
#define SDL_MAIN_HANDLED // Tell SDL not to #define main!!!
#include <SDL.h>
#endif

TCAds ads;
JAKAZuRobot demo;

// Struct that holds the application's state
struct AppState
{
  int ForwardLeftCounter = 0;
  int ForwardCounter = 0;
  int ForwardRightCounter = 0;
  int BackwardLeftCounter = 0;
  int BackwardCounter = 0;
  int BackwardRightCounter = 0;

  float JointSpeed = 0.0f;
  double J1Pos = 0.0;
  double J2Pos = 0.0;
  double J3Pos = 0.0;
  double J4Pos = 0.0;
  double J5Pos = 0.0;
  double J6Pos = 0.0;

  enum class RocketState
  {
    Init,
    Preparing,
    Launched
  };
  float rocket_progress = 0.f;
  RocketState rocketState = RocketState::Init;
};

// MyLoadFonts: demonstrate
// * how to load additional fonts
// * how to use assets from the local assets/ folder
//   Files in the application assets/ folder are embedded automatically
//   (on iOS/Android/Emscripten)
ImFont * gSiyuanBFont = nullptr;

void MyLoadFonts()
{
  ImGuiIO& io = ImGui::GetIO(); // Get the ImGui IO object
  io.Fonts->AddFontFromFileTTF("assets/fonts/SourceHanSansCN-Regular.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
  const char* SiyuanBold = "assets/fonts/SourceHanSansCN-Bold.ttf";

  gSiyuanBFont = io.Fonts->AddFontFromFileTTF(SiyuanBold, 30.0f,NULL,io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
}

void FunctionButton(const char* ButtonName, ImGuiDir dir, double& timer,
                    std::function<void()> ButOn, std::function<void()> ButOff) {

  // When the button is released, do the following thing.
  if (ImGui::ArrowButton(ButtonName, dir)) {
    ButOff();
    HelloImGui::Log(HelloImGui::LogLevel::Info, "%s Button was pressed", ButtonName);
  }

  // Check if button is active (held down)
  if (ImGui::IsItemActive()) {
    // Get current time
    double current_time = ImGui::GetTime();
    // Check if difference is greater than 0.1 seconds
    if (current_time - timer > 0.1) {
      ButOn();
      // Update last time
      timer = current_time;
    }
  }
}


// CommandGui: the widgets on the left panel
void CommandGui(AppState & state)
{
  ImGui::PushFont(gSiyuanBFont);
  ImGui::Text("????????????????????????");
  ImGui::PopFont();
  if (ImGui::IsItemHovered())
  {
    ImGui::SetTooltip(
        "The custom font and the globe image below were loaded \n"
        "from the application assets folder\n"\
        "(those files are embedded automatically).");
  }

  static double FLtimer = 0;
  static double Ftimer = 0;
  static double FRtimer = 0;
  static double BLtimer = 0;
  static double Btimer = 0;
  static double BRtimer = 0;

  static double J1Subtimer = 0;
  static double J2Subtimer = 0;
  static double J3Subtimer = 0;
  static double J4Subtimer = 0;
  static double J5Subtimer = 0;
  static double J6Subtimer = 0;

  static double J1Inctimer = 0;
  static double J2Inctimer = 0;
  static double J3Inctimer = 0;
  static double J4Inctimer = 0;
  static double J5Inctimer = 0;
  static double J6Inctimer = 0;

  float spacing = ImGui::GetStyle().ItemInnerSpacing.x;

  FunctionButton("FL", ImGuiDir_Left, FLtimer,
                 [ObjectPtr = &ads] { ObjectPtr->FLButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->FLButtonToggle(false); });
  ImGui::SameLine(0.0f, spacing);

  FunctionButton("F", ImGuiDir_Up, Ftimer,
                 [ObjectPtr = &ads] { ObjectPtr->FButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->FButtonToggle(false); });
  ImGui::SameLine(0.0f, spacing);

  FunctionButton("FR", ImGuiDir_Right, FRtimer,
                 [ObjectPtr = &ads] { ObjectPtr->FRButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->FRButtonToggle(false); });

  FunctionButton("BL", ImGuiDir_Left, BLtimer,
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(false); });
  ImGui::SameLine(0.0f, spacing);

  FunctionButton("B", ImGuiDir_Down, Btimer,
                 [ObjectPtr = &ads] { ObjectPtr->BButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->BButtonToggle(false); });
  ImGui::SameLine(0.0f, spacing);

  FunctionButton("BR", ImGuiDir_Right, BRtimer,
                 [ObjectPtr = &ads] { ObjectPtr->BRButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->BRButtonToggle(false); });

  ImGui::Separator();

  ImGui::PushFont(gSiyuanBFont);
  ImGui::Text("??????????????????");
  ImGui::PopFont();

  ImGui::SliderFloat("????????????", &state.JointSpeed, 0.0f, 1.0f);

  FunctionButton("J1Sub", ImGuiDir_Left, J1Subtimer,
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(false); });
  ImGui::SameLine(0.0f, spacing);
  ImGui::Text("??????1: %.2f",&state.J1Pos);
  ImGui::SameLine(0.0f, spacing);
  FunctionButton("J1Inc", ImGuiDir_Right, J1Inctimer,
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(false); });

  FunctionButton("J2Sub", ImGuiDir_Left, J2Subtimer,
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(false); });
  ImGui::SameLine(0.0f, spacing);
  ImGui::Text("??????2: %.2f",&state.J2Pos);
  ImGui::SameLine(0.0f, spacing);
  FunctionButton("J2Inc", ImGuiDir_Right, J2Inctimer,
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(false); });

  FunctionButton("J3Sub", ImGuiDir_Left, J3Subtimer,
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(false); });
  ImGui::SameLine(0.0f, spacing);
  ImGui::Text("??????3: %.2f",&state.J3Pos);
  ImGui::SameLine(0.0f, spacing);
  FunctionButton("J3Inc", ImGuiDir_Right, J3Inctimer,
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(false); });

  FunctionButton("J4Sub", ImGuiDir_Left, J4Subtimer,
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(false); });
  ImGui::SameLine(0.0f, spacing);
  ImGui::Text("??????4: %.2f",&state.J4Pos);
  ImGui::SameLine(0.0f, spacing);
  FunctionButton("J4Inc", ImGuiDir_Right, J4Inctimer,
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(false); });

  FunctionButton("J5Sub", ImGuiDir_Left, J5Subtimer,
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(false); });
  ImGui::SameLine(0.0f, spacing);
  ImGui::Text("??????5: %.2f",&state.J5Pos);
  ImGui::SameLine(0.0f, spacing);
  FunctionButton("J5Inc", ImGuiDir_Right, J5Inctimer,
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(false); });

  FunctionButton("J6Sub", ImGuiDir_Left, J6Subtimer,
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(false); });
  ImGui::SameLine(0.0f, spacing);
  ImGui::Text("??????6: %.2f",&state.J6Pos);
  ImGui::SameLine(0.0f, spacing);
  FunctionButton("J6Inc", ImGuiDir_Right, J6Inctimer,
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(true); },
                 [ObjectPtr = &ads] { ObjectPtr->BLButtonToggle(false); });
}

// Our Gui in the status bar
void StatusBarGui(const AppState &appState)
{
  if (appState.rocketState == AppState::RocketState::Preparing) {
    ImGui::Text("Rocket completion: ");
    ImGui::SameLine();
    ImGui::ProgressBar(appState.rocket_progress, HelloImGui::EmToVec2(12.f, 1.f));
  }
}

void MainGui(const AppState &appState){
  HelloImGui::ImageFromAsset("pics/jaka.png");
}

// Example of an optional native event callback for the backend (implemented here only for SDL)
bool NativeBackendEventCallback(void * event)
{
#ifdef HELLOIMGUI_USE_SDL_OPENGL3
  SDL_Event*  sdlEvent = (SDL_Event *)event;
  switch( sdlEvent->type)
  {
    case SDL_KEYDOWN:
      HelloImGui::Log(HelloImGui::LogLevel::Warning, "It SDL_KEYDOWN detected");
      return false; // if you return true, the event is not processd further
  }
  return false;
#else
  return false;
#endif
};


int main(int, char **)
{
  // ADS init
  ads.InitAds();

  // JAKA init
  std::string ipaddr = "10.5.5.100";
  auto isLogin = demo.login_in(ipaddr.c_str());
  //???????????????
  auto isPowerON = demo.power_on();
  //??????????????????
  auto isEnable = demo.enable_robot();

  if (isLogin & isPowerON & isEnable){
    HelloImGui::Log(HelloImGui::LogLevel::Info, "??????????????????????????????");
  } else {
    HelloImGui::Log(HelloImGui::LogLevel::Error, "??????????????????????????????");
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Part 1: Define the application state, fill the status and menu bars, and load additional font
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Our application state
  AppState appState;

  // Hello ImGui params (they hold the settings as well as the Gui callbacks)
  HelloImGui::RunnerParams runnerParams;

  runnerParams.appWindowParams.windowTitle = "Robot Controller";
  runnerParams.appWindowParams.windowGeometry.size = {800, 600};
  // runnerParams.appWindowParams.restorePreviousGeometry = true;

  //
  // Status bar
  //
  // We use the default status bar of Hello ImGui
  runnerParams.imGuiWindowParams.showStatusBar = true;
  // uncomment next line in order to hide the FPS in the status bar
  // runnerParams.imGuiWindowParams.showStatus_Fps = false;
  runnerParams.callbacks.ShowStatus = [&appState] { StatusBarGui(appState); };

  //
  // Menu bar
  //
  // We use the default menu of Hello ImGui, to which we add some more items
  runnerParams.imGuiWindowParams.showMenuBar = true;
  runnerParams.callbacks.ShowMenus = []() {
    if (ImGui::BeginMenu("My Menu"))
    {
      if (ImGui::MenuItem("Test me"))
        HelloImGui::Log(HelloImGui::LogLevel::Debug, "It works");
      ImGui::EndMenu();
    }
  };

  // Custom load fonts
  runnerParams.callbacks.LoadAdditionalFonts = MyLoadFonts;

  // optional native events handling
  runnerParams.callbacks.AnyBackendEventCallback = NativeBackendEventCallback;

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Part 2: Define the application layout and windows
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //
  //    2.1 Define the docking splits,
  //    i.e. the way the screen space is split in different target zones for the dockable windows
  //     We want to split "MainDockSpace" (which is provided automatically) into three zones, like this:
  //
  //    ___________________________________________
  //    |        |                                |
  //    | Left   |                                |
  //    | Space  |    MainDockSpace               |
  //    |        |                                |
  //    |        |                                |
  //    |        |                                |
  //    -------------------------------------------
  //    |     BottomSpace                         |
  //    -------------------------------------------
  //

  // First, tell HelloImGui that we want full screen dock space (this will create "MainDockSpace")
  runnerParams.imGuiWindowParams.defaultImGuiWindowType = HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;
  // In this demo, we also demonstrate multiple viewports.
  // you can drag windows outside out the main window in order to put their content into new native windows
  runnerParams.imGuiWindowParams.enableViewports = true;

  // Then, add a space named "BottomSpace" whose height is 25% of the app height.
  // This will split the preexisting default dockspace "MainDockSpace" in two parts.
  HelloImGui::DockingSplit splitMainBottom;
  splitMainBottom.initialDock = "MainDockSpace";
  splitMainBottom.newDock = "BottomSpace";
  splitMainBottom.direction = ImGuiDir_Down;
  splitMainBottom.ratio = 0.25f;

  // Then, add a space to the left which occupies a column whose width is 25% of the app width
  HelloImGui::DockingSplit splitMainLeft;
  splitMainLeft.initialDock = "MainDockSpace";
  splitMainLeft.newDock = "LeftSpace";
  splitMainLeft.direction = ImGuiDir_Left;
  splitMainLeft.ratio = 0.25f;

  // Finally, transmit these splits to HelloImGui
  runnerParams.dockingParams.dockingSplits = { splitMainBottom, splitMainLeft };

  //
  // 2.1 Define our dockable windows : each window provide a Gui callback, and will be displayed
  //     in a docking split.
  //

  // A Command panel named "Commands" will be placed in "LeftSpace". Its Gui is provided calls "CommandGui"
  HelloImGui::DockableWindow commandsWindow;
  commandsWindow.label = "????????????";
  commandsWindow.dockSpaceName = "LeftSpace";
  commandsWindow.GuiFunction = [&appState]() { CommandGui(appState); };
  // A Log  window named "Logs" will be placed in "BottomSpace". It uses the HelloImGui logger gui
  HelloImGui::DockableWindow logsWindow;
  logsWindow.label = "Logs";
  logsWindow.dockSpaceName = "BottomSpace";
  logsWindow.GuiFunction = [] { HelloImGui::LogGui(); };
  // A Window named "Dear ImGui Demo" will be placed in "MainDockSpace"
  HelloImGui::DockableWindow demoWindow;
  demoWindow.label = "????????????";
  demoWindow.dockSpaceName = "MainDockSpace";
  demoWindow.GuiFunction = [&appState]() {MainGui(appState); };
  // Finally, transmit these windows to HelloImGui
  runnerParams.dockingParams.dockableWindows = { commandsWindow, logsWindow, demoWindow };

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Part 3: Run the app
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  HelloImGui::Run(runnerParams);

  return 0;
}