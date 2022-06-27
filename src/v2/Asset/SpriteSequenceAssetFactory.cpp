/**
* @file
* @date 2022/6/8
* @author 9chu
* 这个文件是 LuaSTGPlus 项目的一部分，请在项目所定义之授权许可范围内合规使用。
*/
#include <lstg/v2/Asset/SpriteSequenceAssetFactory.hpp>

#include <lstg/Core/Logging.hpp>
#include <lstg/Core/Subsystem/AssetSystem.hpp>
#include <lstg/Core/Subsystem/Asset/AssetError.hpp>
#include <lstg/Core/Subsystem/Asset/ArgumentHelper.hpp>
#include <lstg/v2/Asset/SpriteSequenceAsset.hpp>
#include <lstg/v2/Asset/SpriteSequenceAssetLoader.hpp>

using namespace std;
using namespace lstg;
using namespace lstg::v2::Asset;

LSTG_DEF_LOG_CATEGORY(SpriteSequenceAssetFactory);

std::string_view SpriteSequenceAssetFactory::GetAssetTypeName() const noexcept
{
    return "SpriteSequence";
}

Subsystem::Asset::AssetTypeId SpriteSequenceAssetFactory::GetAssetTypeId() const noexcept
{
    return SpriteSequenceAsset::GetAssetTypeIdStatic();
}

Result<Subsystem::Asset::CreateAssetResult> SpriteSequenceAssetFactory::CreateAsset(Subsystem::AssetSystem& assetSystem,
    Subsystem::Asset::AssetPoolPtr pool, std::string_view name, const nlohmann::json& arguments) noexcept
{
    auto textureName = Subsystem::Asset::ReadArgument<string>(arguments, "/texture");
    if (!textureName)
        return make_error_code(Subsystem::Asset::AssetError::MissingRequiredArgument);
    auto sequencesX = Subsystem::Asset::ReadArgument<double>(arguments, "/left");
    auto sequencesY = Subsystem::Asset::ReadArgument<double>(arguments, "/top");
    auto frameW = Subsystem::Asset::ReadArgument<double>(arguments, "/frameWidth");
    auto frameH = Subsystem::Asset::ReadArgument<double>(arguments, "/frameHeight");
    auto row = Subsystem::Asset::ReadArgument<int32_t>(arguments, "/row");
    auto column = Subsystem::Asset::ReadArgument<int32_t>(arguments, "/column");
    auto interval = Subsystem::Asset::ReadArgument<int32_t>(arguments, "/interval");

    if (!sequencesX || !sequencesY || !frameW || !frameH || !row || !column || !interval)
        return make_error_code(Subsystem::Asset::AssetError::MissingRequiredArgument);
    auto colliderHalfSizeX = Subsystem::Asset::ReadArgument<double>(arguments, "/colliderHalfSizeX", 0);
    auto colliderHalfSizeY = Subsystem::Asset::ReadArgument<double>(arguments, "/colliderHalfSizeY", 0);
    auto colliderIsRect = Subsystem::Asset::ReadArgument<bool>(arguments, "/colliderIsRect", false);

    try
    {
        // 找到依赖的纹理
        auto texture = static_pointer_cast<TextureAsset>(pool->GetAsset(*textureName));
        if (!texture)
        {
            LSTG_LOG_ERROR_CAT(SpriteSequenceAssetFactory, "Texture \"{}\" not found", *textureName);
            return make_error_code(Subsystem::Asset::AssetError::DependentAssetNotFound);
        }

        // 拆分构造 Frames
        std::vector<UVRectangle> frames;
        for (int j = 0; j < *row; ++j)  // 行
        {
            for (int i = 0; i < *column; ++i)  // 列
            {
                frames.emplace_back(
                    *sequencesX + *frameW * i,  // left
                    *sequencesY + *frameH * j,  // top
                    *frameW,  // width
                    *frameH  // height
                );
            }
        }
        if (frames.empty())
            return make_error_code(std::errc::invalid_argument);

        // 创建 Collider
        ColliderShape collider;
        if (colliderIsRect)
        {
            Math::Collider2D::OBBShape<double> obbShape;
            obbShape.HalfSize = Vec2(colliderHalfSizeX, colliderHalfSizeY);
            collider = obbShape;
        }
        else if (colliderHalfSizeX == colliderHalfSizeY)
        {
            Math::Collider2D::CircleShape<double> circleShape;
            circleShape.Radius = colliderHalfSizeX;
            collider = circleShape;
        }
        else
        {
            Math::Collider2D::EllipseShape<double> ellipseShape;
            ellipseShape.A = colliderHalfSizeX;
            ellipseShape.B = colliderHalfSizeY;
            collider = ellipseShape;
        }

        auto asset = make_shared<SpriteSequenceAsset>(string{name}, std::move(texture), std::move(frames), collider);
        auto loader = make_shared<SpriteSequenceAssetLoader>(asset);
        return Subsystem::Asset::CreateAssetResult {
            static_pointer_cast<Subsystem::Asset::Asset>(asset),
            static_pointer_cast<Subsystem::Asset::AssetLoader>(loader)
        };
    }
    catch (const std::system_error& ex)
    {
        return ex.code();
    }
    catch (...)
    {
        return make_error_code(errc::not_enough_memory);
    }
}
