/**
 * @file
 * @date 2022/3/14
 * @author 9chu
 * 这个文件是 LuaSTGPlus 项目的一部分，请在项目所定义之授权许可范围内合规使用。
 */
#include <lstg/Core/Subsystem/Render/Effect/EffectPassDefinition.hpp>

using namespace std;
using namespace lstg;
using namespace lstg::Subsystem::Render::Effect;

#define SMART_PTR_ELEMENT_EQUAL(X) \
    (X && rhs.X ? *X == *rhs.X : static_cast<bool>(X) == static_cast<bool>(rhs.X))

bool EffectPassDefinition::BlendStateDesc::operator==(const BlendStateDesc& rhs) const noexcept
{
    return Enable == rhs.Enable &&
        SourceBlend == rhs.SourceBlend &&
        DestBlend == rhs.DestBlend &&
        BlendOperation == rhs.BlendOperation &&
        SourceAlphaBlend == rhs.SourceAlphaBlend &&
        DestAlphaBlend == rhs.DestAlphaBlend &&
        AlphaBlendOperation == rhs.AlphaBlendOperation &&
        WriteMask == rhs.WriteMask;
}

size_t EffectPassDefinition::BlendStateDesc::GetHashCode() const noexcept
{
    auto ret = std::hash<std::string_view>{}("PassBlendState");
    ret ^= std::hash<bool>{}(Enable);
    ret ^= std::hash<BlendFactors>{}(SourceBlend);
    ret ^= std::hash<BlendFactors>{}(DestBlend);
    ret ^= std::hash<BlendOperations>{}(BlendOperation);
    ret ^= std::hash<BlendFactors>{}(SourceAlphaBlend);
    ret ^= std::hash<BlendFactors>{}(DestAlphaBlend);
    ret ^= std::hash<BlendOperations>{}(AlphaBlendOperation);
    ret ^= std::hash<ColorWriteMask>{}(WriteMask);
    return ret;
}

bool EffectPassDefinition::RasterizerStateDesc::operator==(const RasterizerStateDesc& rhs) const noexcept
{
    return FillMode == rhs.FillMode &&
        CullMode == rhs.CullMode;
}

size_t EffectPassDefinition::RasterizerStateDesc::GetHashCode() const noexcept
{
    auto ret = std::hash<std::string_view>{}("PassRasterizerState");
    ret ^= std::hash<FillModes>{}(FillMode);
    ret ^= std::hash<CullModes>{}(CullMode);
    return ret;
}

bool EffectPassDefinition::DepthStencilStateDesc::operator==(const DepthStencilStateDesc& rhs) const noexcept
{
    return DepthEnable == rhs.DepthEnable &&
        DepthWriteEnable == rhs.DepthWriteEnable &&
        DepthFunction == rhs.DepthFunction;
}

size_t EffectPassDefinition::DepthStencilStateDesc::GetHashCode() const noexcept
{
    auto ret = std::hash<std::string_view>{}("DepthStencilState");
    ret ^= std::hash<bool>{}(DepthEnable);
    ret ^= std::hash<bool>{}(DepthWriteEnable);
    ret ^= std::hash<ComparisionFunctions>{}(DepthFunction);
    return ret;
}

bool EffectPassDefinition::operator==(const EffectPassDefinition& rhs) const noexcept
{
    // Name & States
    auto ret = (m_stName == rhs.m_stName &&
        m_stBlendState == rhs.m_stBlendState &&
        m_stRasterizerState == rhs.m_stRasterizerState &&
        m_stDepthStencilState == rhs.m_stDepthStencilState);
    if (!ret)
        return false;

    // Vertex Shader
    if (!SMART_PTR_ELEMENT_EQUAL(m_pVertexShader))
        return false;

    // Pixel Shader
    if (!SMART_PTR_ELEMENT_EQUAL(m_pPixelShader))
        return false;
    return true;
}

size_t EffectPassDefinition::GetHashCode() const noexcept
{
    auto ret = std::hash<std::string_view>{}("EffectPass");
    ret ^= std::hash<string>{}(m_stName);
    ret ^= m_stBlendState.GetHashCode();
    ret ^= m_stRasterizerState.GetHashCode();
    ret ^= m_stDepthStencilState.GetHashCode();
    if (m_pVertexShader)
        ret ^= m_pVertexShader->GetHashCode();
    if (m_pPixelShader)
        ret ^= m_pPixelShader->GetHashCode();
    return ret;
}
