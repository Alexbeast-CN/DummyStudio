#include "hello_imgui/hello_imgui.h"
#include "imgui_internal.h"
#include "myAds.h"

#ifdef HELLOIMGUI_USE_SDL_OPENGL3
#define SDL_MAIN_HANDLED // Tell SDL not to #define main!!!
#include <SDL.h>
#endif

TCAds ads;

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
  float J1Pos = 0.0f;
  float J2Pos = 0.0f;
  float J3Pos = 0.0f;
  float J4Pos = 0.0f;
  float J5Pos = 0.0f;
  float J6Pos = 0.0f;

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
  ImGui::Text("四轮底盘控制器：");
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
                 std::bind(&TCAds::FLButtonToggle, &ads, true),
                 std::bind(&TCAds::FLButtonToggle, &ads, false));
  ImGui::SameLine(0.0f, spacing);

  FunctionButton("F", ImGuiDir_Up, Ftimer,
                 std::bind(&TCAds::FButtonToggle, &ads, true),
                 std::bind(&TCAds::FButtonToggle, &ads, false));
  ImGui::SameLine(0.0f, spacing);

  FunctionButton("FR", ImGuiDir_Right, FRtimer,
                 std::bind(&TCAds::FRButtonToggle, &ads, true),
                 std::bind(&TCAds::FRButtonToggle, &ads, false));

  FunctionButton("BL", ImGuiDir_Left, BLtimer,
                 std::bind(&TCAds::BLButtonToggle, &ads, true),
                 std::bind(&TCAds::BLButtonToggle, &ads, false));
  ImGui::SameLine(0.0f, spacing);

  FunctionButton("B", ImGuiDir_Down, Btimer,
                 std::bind(&TCAds::BButtonToggle, &ads, true),
                 std::bind(&TCAds::BButtonToggle, &ads, false));
  ImGui::SameLine(0.0f, spacing);

  FunctionButton("BR", ImGuiDir_Right, BRtimer,
                 std::bind(&TCAds::BRButtonToggle, &ads, true),
                 std::bind(&TCAds::BRButtonToggle, &ads, false));

  ImGui::Separator();

  ImGui::PushFont(gSiyuanBFont);
  ImGui::Text("节卡机械臂：");
  ImGui::PopFont();

  ImGui::SliderFloat("关节速度", &state.JointSpeed, 0.0f, 1.0f);

  FunctionButton("J1Sub", ImGuiDir_Left, J1Subtimer,
                 std::bind(&TCAds::BLButtonToggle, &ads, true),
                 std::bind(&TCAds::BLButtonToggle, &ads, false));
  ImGui::SameLine(0.0f, spacing);
  ImGui::Text("关节1: %d",&state.J1Pos);
  ImGui::SameLine(0.0f, spacing);
  FunctionButton("J1Inc", ImGuiDir_Right, J1Inctimer,
                 std::bind(&TCAds::BLButtonToggle, &ads, true),
                 std::bind(&TCAds::BLButtonToggle, &ads, false));

  FunctionButton("J2Sub", ImGuiDir_Left, J2Subtimer,
                 std::bind(&TCAds::BLButtonToggle, &ads, true),
                 std::bind(&TCAds::BLButtonToggle, &ads, false));
  ImGui::SameLine(0.0f, spacing);
  ImGui::Text("关节2: %d",&state.J2Pos);
  ImGui::SameLine(0.0f, spacing);
  FunctionButton("J2Inc", ImGuiDir_Right, J2Inctimer,
                 std::bind(&TCAds::BLButtonToggle, &ads, true),
                 std::bind(&TCAds::BLButtonToggle, &ads, false));

  FunctionButton("J3Sub", ImGuiDir_Left, J3Subtimer,
                 std::bind(&TCAds::BLButtonToggle, &ads, true),
                 std::bind(&TCAds::BLButtonToggle, &ads, false));
  ImGui::SameLine(0.0f, spacing);
  ImGui::Text("关节3: %d",&state.J3Pos);
  ImGui::SameLine(0.0f, spacing);
  FunctionButton("J3Inc", ImGuiDir_Right, J3Inctimer,
                 std::bind(&TCAds::BLButtonToggle, &ads, true),
                 std::bind(&TCAds::BLButtonToggle, &ads, false));

  FunctionButton("J4Sub", ImGuiDir_Left, J4Subtimer,
                 std::bind(&TCAds::BLButtonToggle, &ads, true),
                 std::bind(&TCAds::BLButtonToggle, &ads, false));
  ImGui::SameLine(0.0f, spacing);
  ImGui::Text("关节4: %d",&state.J4Pos);
  ImGui::SameLine(0.0f, spacing);
  FunctionButton("J4Inc", ImGuiDir_Right, J4Inctimer,
                 std::bind(&TCAds::BLButtonToggle, &ads, true),
                 std::bind(&TCAds::BLButtonToggle, &ads, false));

  FunctionButton("J5Sub", ImGuiDir_Left, J5Subtimer,
                 std::bind(&TCAds::BLButtonToggle, &ads, true),
                 std::bind(&TCAds::BLButtonToggle, &ads, false));
  ImGui::SameLine(0.0f, spacing);
  ImGui::Text("关节5: %d",&state.J5Pos);
  ImGui::SameLine(0.0f, spacing);
  FunctionButton("J5Inc", ImGuiDir_Right, J5Inctimer,
                 std::bind(&TCAds::BLButtonToggle, &ads, true),
                 std::bind(&TCAds::BLButtonToggle, &ads, false));

  FunctionButton("J6Sub", ImGuiDir_Left, J6Subtimer,
                 std::bind(&TCAds::BLButtonToggle, &ads, true),
                 std::bind(&TCAds::BLButtonToggle, &ads, false));
  ImGui::SameLine(0.0f, spacing);
  ImGui::Text("关节6: %d",&state.J6Pos);
  ImGui::SameLine(0.0f, spacing);
  FunctionButton("J6Inc", ImGuiDir_Right, J6Inctimer,
                 std::bind(&TCAds::BLButtonToggle, &ads, true),
                 std::bind(&TCAds::BLButtonToggle, &ads, false));
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
  ads.InitAds();

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Part 1: Define the application state, fill the status and menu bars, and load additional font
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Our application state
  AppState appState;

  // Hello ImGui params (they hold the settings as well as the Gui callbacks)
  HelloImGui::RunnerParams runnerParams;

  runnerParams.appWindowParams.windowTitle = "Docking demo";
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
  commandsWindow.label = "Commands";
  commandsWindow.dockSpaceName = "LeftSpace";
  commandsWindow.GuiFunction = [&appState]() { CommandGui(appState); };
  // A Log  window named "Logs" will be placed in "BottomSpace". It uses the HelloImGui logger gui
  HelloImGui::DockableWindow logsWindow;
  logsWindow.label = "Logs";
  logsWindow.dockSpaceName = "BottomSpace";
  logsWindow.GuiFunction = [] { HelloImGui::LogGui(); };
  // A Window named "Dear ImGui Demo" will be placed in "MainDockSpace"
  HelloImGui::DockableWindow demoWindow;
  demoWindow.label = "Dear ImGui Demo";
  demoWindow.dockSpaceName = "MainDockSpace";
  demoWindow.GuiFunction = [] { ImGui::ShowDemoWindow(); };
  // Finally, transmit these windows to HelloImGui
  runnerParams.dockingParams.dockableWindows = { commandsWindow, logsWindow, demoWindow };

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Part 3: Run the app
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  HelloImGui::Run(runnerParams);
  return 0;
}