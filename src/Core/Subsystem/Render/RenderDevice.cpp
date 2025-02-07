/**
 * @file
 * @date 2022/3/9
 * @author 9chu
 * 此文件为 LuaSTGPlus 项目的一部分，版权与许可声明详见 COPYRIGHT.txt。
 */
#include <lstg/Core/Subsystem/Render/RenderDevice.hpp>

#include <cassert>
#include <RenderDevice.h>
#include <DeviceContext.h>
#include <SwapChain.h>

using namespace std;
using namespace lstg;
using namespace lstg::Subsystem::Render;

RenderDevice::~RenderDevice() noexcept
{
    if (m_pSwapChain)
        m_pSwapChain->Release();
    if (m_pRenderContext)
        m_pRenderContext->Release();
    if (m_pRenderDevice)
        m_pRenderDevice->Release();
}

Diligent::IRenderDevice* RenderDevice::GetDevice() const noexcept
{
    return m_pRenderDevice;
}

Diligent::IDeviceContext* RenderDevice::GetImmediateContext() const noexcept
{
    return m_pRenderContext;
}

Diligent::ISwapChain* RenderDevice::GetSwapChain() const noexcept
{
    return m_pSwapChain;
}

bool RenderDevice::IsVerticalSyncEnabled() const noexcept
{
    return m_bVerticalSync;
}

void RenderDevice::SetVerticalSyncEnabled(bool enable) noexcept
{
    m_bVerticalSync = enable;
}

uint32_t RenderDevice::GetRenderOutputWidth() const noexcept
{
    assert(m_pSwapChain);
    return m_pSwapChain->GetDesc().Width;
}

uint32_t RenderDevice::GetRenderOutputHeight() const noexcept
{
    assert(m_pSwapChain);
    return m_pSwapChain->GetDesc().Height;
}

void RenderDevice::Present() noexcept
{
    // 执行 Present
    m_pSwapChain->Present(m_bVerticalSync ? 1 : 0);
    ++m_uPresentedCount;
}
