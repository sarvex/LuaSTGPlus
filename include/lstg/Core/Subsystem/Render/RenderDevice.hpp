/**
 * @file
 * @date 2022/3/8
 * @author 9chu
 * 此文件为 LuaSTGPlus 项目的一部分，版权与许可声明详见 COPYRIGHT.txt。
 */
#pragma once
#include <lstg/Core/Exception.hpp>

namespace Diligent
{
    struct IRenderDevice;
    struct IDeviceContext;
    struct ISwapChain;
}

namespace lstg::Subsystem::Render
{
    LSTG_DEFINE_EXCEPTION(RenderDeviceInitializeFailedException);

    /**
     * 渲染设备
     * 由 DiligentEngine 完成抽象，不对应用暴露 DiligentEngine。
     */
    class RenderDevice
    {
    protected:
        RenderDevice() noexcept = default;
        virtual ~RenderDevice() noexcept;

    public:
        /**
         * 获取关联的渲染设备
         */
        [[nodiscard]] Diligent::IRenderDevice* GetDevice() const noexcept;

        /**
         * 获取关联的渲染上下文
         */
        [[nodiscard]] Diligent::IDeviceContext* GetImmediateContext() const noexcept;

        /**
         * 获取关联的交换链
         */
        [[nodiscard]] Diligent::ISwapChain* GetSwapChain() const noexcept;

        /**
         * 是否启用垂直同步
         */
        [[nodiscard]] virtual bool IsVerticalSyncEnabled() const noexcept;

        /**
         * 设置是否启用垂直同步
         */
        virtual void SetVerticalSyncEnabled(bool enable) noexcept;

        /**
         * 获取渲染画面宽度
         * 转发到 SwapChain.GetDesc().Width
         */
        [[nodiscard]] uint32_t GetRenderOutputWidth() const noexcept;

        /**
         * 获取渲染画面高度
         * 转发到 SwapChain.GetDesc().Height
         */
        [[nodiscard]] uint32_t GetRenderOutputHeight() const noexcept;

        /**
         * 获取已渲染的画面数量
         */
        [[nodiscard]] uint32_t GetPresentedFrameCount() const noexcept { return m_uPresentedCount; }

        /**
         * 呈现
         * 默认转发到 SwapChain.Present()，不同平台存在特定行为可以通过覆写该方法实现。
         */
        virtual void Present() noexcept;

    protected:
        Diligent::IRenderDevice* m_pRenderDevice = nullptr;
        Diligent::IDeviceContext* m_pRenderContext = nullptr;
        Diligent::ISwapChain* m_pSwapChain = nullptr;
        uint32_t m_uPresentedCount = 0;
        bool m_bVerticalSync = false;
    };

    using RenderDevicePtr = std::shared_ptr<RenderDevice>;
}
