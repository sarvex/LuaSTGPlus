/**
 * @file
 * @date 2022/3/11
 * @author 9chu
 * 这个文件是 LuaSTGPlus 项目的一部分，请在项目所定义之授权许可范围内合规使用。
 */
#include <lstg/Core/Subsystem/DebugGUI/FrameTimeMonitor.hpp>

#include <imgui.h>
#include <implot.h>
#include <lstg/Core/Subsystem/ProfileSystem.hpp>

using namespace std;
using namespace lstg;
using namespace lstg::Subsystem::DebugGUI;

static const DebugWindowFlags kWindowStyle = DebugWindowFlags::NoSavedSettings | DebugWindowFlags::AlwaysAutoResize;

FrameTimeMonitor::FrameTimeMonitor()
    : Window("FrameTimeMonitor", "Frame Time Monitor", kWindowStyle)
{
    // 收集 200 个采样点
    m_stFrames.resize(200);
}

void FrameTimeMonitor::OnPrepareWindow() noexcept
{
    ImGui::SetNextWindowPos(ImVec2(5.f, 60.f), ImGuiCond_FirstUseEver);
}

void FrameTimeMonitor::OnUpdate(double elapsedTime) noexcept
{
    const auto& profiler = ProfileSystem::GetInstance();

    // 收集帧信息
    double total = profiler.GetLastFrameElapsedTime();
    double eventDispatchTime = profiler.GetPerformanceCounter(PerformanceCounterTypes::PerFrame, "EventDispatchTime");
    double updateTime = profiler.GetPerformanceCounter(PerformanceCounterTypes::PerFrame, "UpdateTime");
    double renderTime = profiler.GetPerformanceCounter(PerformanceCounterTypes::PerFrame, "RenderTime");

    FrameTime ft;
    ft.Total = total;
    ft.EventDispatchTimeStack = eventDispatchTime + updateTime + renderTime;
    ft.UpdateTimeStack = updateTime + renderTime;
    ft.RenderTimeStack = renderTime;

    m_stFrames.erase(m_stFrames.begin());
    m_stFrames.push_back(ft);
}

void FrameTimeMonitor::OnRender() noexcept
{
    if (ImPlot::BeginPlot("##NoTitle", ImVec2(300.f, 150.f)))
    {
        auto totalSamples = static_cast<int>(m_stFrames.size());
        auto beginOfTotalTime = reinterpret_cast<double*>(reinterpret_cast<uint8_t*>(m_stFrames.data()) + offsetof(FrameTime, Total));
        auto beginOfEventDispatchTime = reinterpret_cast<double*>(reinterpret_cast<uint8_t*>(m_stFrames.data()) +
            offsetof(FrameTime, EventDispatchTimeStack));
        auto beginOfEventUpdateTime = reinterpret_cast<double*>(reinterpret_cast<uint8_t*>(m_stFrames.data()) +
            offsetof(FrameTime, UpdateTimeStack));
        auto beginOfEventRenderTime = reinterpret_cast<double*>(reinterpret_cast<uint8_t*>(m_stFrames.data()) +
            offsetof(FrameTime, RenderTimeStack));

        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_Lock,
            ImPlotAxisFlags_AutoFit);
        ImPlot::SetupAxesLimits(0, totalSamples, 0, 5.0);
        ImPlot::PlotLine("Frame", beginOfTotalTime, totalSamples, 1, 0, 0, sizeof(FrameTime));
        ImPlot::PlotLine("EventDispatch", beginOfEventDispatchTime, totalSamples, 1, 0, 0, sizeof(FrameTime));
        ImPlot::PlotLine("Update", beginOfEventUpdateTime, totalSamples, 1, 0, 0, sizeof(FrameTime));
        ImPlot::PlotLine("Render", beginOfEventRenderTime, totalSamples, 1, 0, 0, sizeof(FrameTime));
        ImPlot::EndPlot();
    }
}
