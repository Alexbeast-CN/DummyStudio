#include "hello_imgui/hello_imgui.h"
#include "imgui_internal.h"

#ifdef HELLOIMGUI_USE_SDL_OPENGL3
#define SDL_MAIN_HANDLED // Tell SDL not to #define main!!!
#include <SDL.h>
#endif


// Struct that holds the application's state
struct AppState
{
  float f = 0.0f;
  int ForwardLeftCounter = 0;
  int ForwardCounter = 0;
  int ForwardRightCounter = 0;
  int BackwardLeftCounter = 0;
  int BackwardCounter = 0;
  int BackwardRightCounter = 0;

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
ImFont * gSiyuanFont = nullptr;
void MyLoadFonts()
{
  ImGuiIO& io = ImGui::GetIO(); // Get the ImGui IO object
  io.Fonts->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 16.0f);
  const char* Siyuan = "assets/fonts/SourceHanSansSC-VF.ttf";
  gSiyuanFont = io.Fonts->AddFontFromFileTTF(Siyuan, 18.0f,NULL,io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
}

void VehicleButton(const char* ButtonName, int &counter, double &timer){
  // When the button is released, do the following thing.
  if (ImGui::Button(ButtonName)) {
    HelloImGui::Log(HelloImGui::LogLevel::Info, "%s Button was pressed", ButtonName);
  }

  // Check if button is active (held down)
  if (ImGui::IsItemActive()) {
    // Get current time
    double current_time = ImGui::GetTime();
    // Check if difference is greater than 0.1 seconds
    if (current_time - timer > 0.1) {
      counter++;
      // Update last time
      timer = current_time;
    }
  }

  ImGui::SameLine();
  ImGui::Text("count: %d", counter);
}


// CommandGui: the widgets on the left panel
void CommandGui(AppState & state)
{
  ImGui::PushFont(gSiyuanFont);
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

  VehicleButton("ForwardLeft", state.ForwardLeftCounter,FLtimer);
  VehicleButton("Forward", state.ForwardCounter,Ftimer);
  VehicleButton("ForwardRight", state.ForwardRightCounter,FRtimer);
  VehicleButton("BackwardLeft", state.BackwardLeftCounter,BLtimer);
  VehicleButton("Backward", state.BackwardCounter,Btimer);
  VehicleButton("BackwardRight", state.BackwardRightCounter,BRtimer);

  ImGui::Separator();

  // Edit 1 float using a slider from 0.0f to 1.0f
  if (ImGui::SliderFloat("float", &state.f, 0.0f, 1.0f))
    HelloImGui::Log(HelloImGui::LogLevel::Warning, "state.f was changed to %f", state.f);


  switch(state.rocketState)
  {
    case AppState::RocketState::Init:
      if (ImGui::Button(ICON_FA_ROCKET " Launch rocket"))
      {
        state.rocketState = AppState::RocketState::Preparing;
        HelloImGui::Log(HelloImGui::LogLevel::Warning, "Rocket is being prepared");
      }
      break;
    case AppState::RocketState::Preparing:
      ImGui::Text(ICON_FA_ROCKET " Please Wait");
      state.rocket_progress += 0.003f;
      if (state.rocket_progress >= 1.f)
      {
        state.rocketState = AppState::RocketState::Launched;
        HelloImGui::Log(HelloImGui::LogLevel::Warning, "Rocket was launched!");
      }
      break;
    case AppState::RocketState::Launched:
      ImGui::Text(ICON_FA_ROCKET " Rocket Launched");
      if (ImGui::Button("Reset Rocket"))
      {
        state.rocketState = AppState::RocketState ::Init;
        state.rocket_progress = 0.f;
      }
      break;
  }
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
