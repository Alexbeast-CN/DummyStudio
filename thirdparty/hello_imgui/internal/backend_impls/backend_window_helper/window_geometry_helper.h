#pragma once
#include "hello_imgui/app_window_params.h"
#include "hello_imgui/internal/backend_impls/backend_window_helper/backend_window_helper.h"

#include <optional>


namespace HelloImGui
{
    class WindowGeometryHelper
    {
    public:
        WindowGeometry& mGeometry;
        bool mRestoreLast;
        std::string mWindowGeometryIniFilename;

        WindowGeometryHelper(WindowGeometry &geometry, bool restoreLast, const std::string& windowGeometryIniFilename);

        bool HasInitialWindowSizeInfo();
        ScreenBounds AppWindowBoundsInitial(const std::vector<ScreenBounds>& allMonitorsWorkAreas);

        // Save / Load last run window bounds
        void WriteLastRunWindowBounds(const ScreenBounds& windowBounds);
        std::optional<ScreenBounds> ReadLastRunWindowBounds();


        void EnsureWindowFitsMonitor(BackendApi::IBackendWindowHelper* helper, BackendApi::WindowPointer window);
        void CenterWindowOnMonitor(BackendApi::IBackendWindowHelper* helper, BackendApi::WindowPointer window);
        ScreenBounds GetCurrentMonitorWorkArea(BackendApi::IBackendWindowHelper* backendWindowHelper,
                                               BackendApi::WindowPointer window);

        // Will set the window size, after making sure it fits on the current screen
        void TrySetWindowSize(BackendApi::IBackendWindowHelper *backendWindowHelper, BackendApi::WindowPointer window, ImVec2 userWidgetsSize);

    private:
        int GetMonitorIndexFromWindowPosition(BackendApi::IBackendWindowHelper *backendWindowHelper, const ScreenPosition& windowPosition);
    };

    struct SearchForMonitorResult
    {
        int monitorIdx;
        std::optional<ScreenPosition> newPosition;
    };
    SearchForMonitorResult SearchForMonitor(const std::vector<ScreenBounds>& monitorsWorkAreas, const AppWindowParams& appWindowParams);

}