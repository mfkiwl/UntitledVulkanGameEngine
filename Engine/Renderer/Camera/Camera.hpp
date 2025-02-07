#pragma once
#include "Projection.hpp"
#include <Core/Interfaces/WindowInterface.hpp>
#include <GameFramework/Components/Components/CoreComponent.hpp>

namespace UVK
{

    /**
     * @brief A cross-renderer camera abstraction
     */
    class UVK_PUBLIC_API Camera
    {
    public:
        Camera() = default;
        /**
         * @brief Constructs and initializes a camera for a Pawn
         * @param coreComponent A reference to the core component of the actor the Pawn controls
         * @param translation Translation offset of the camera from the parent
         * @param rot Rotation offset of the camera from the parent
         * @param up What direction is the up direction
         */
        Camera(const CoreComponent& coreComponent, FVector translation, FVector rotation, FVector up) noexcept;

        /**
         * @brief Constructs and initializes a camera for a Pawn
         * @param owningPawn A pointer to the pawn that owns the given camera
         * @param translation Translation offset of the camera from the parent
         * @param rot Rotation offset of the camera from the parent
         * @param up What direction is the up direction
         */
        void init(const CoreComponent& coreComponent, FVector translation, FVector rotation, FVector up) noexcept;

        [[nodiscard]] glm::mat4 calculateViewMatrix() const noexcept;

        void recalculate() noexcept;

        Projection& projection() noexcept;

        /**
         * @brief Automatically constructs a camera and it's corresponding view matrix, should be used when initially
         * creating the camera
         * @param owningPawn A pointer to the pawn that owns the given camera
         * @param translation Translation offset of the camera from the parent
         * @param rot Rotation offset of the camera from the parent
         * @param up What direction is the up direction
         * @param planes A 2D vector representing the near and far planes for the perspective projection
         * @param fov Describes the horizontal field of view of the perspective projection
         * @param aspectRatio The aspect ratio of the screen/viewport for the perspective projection
         */
        static Camera makeCamera(const CoreComponent& coreComponent, float aspectRatio = Window::aspectRatio(), FVector translation = FVector(0.0f, 0.0f, 0.0f), FVector rotation = FVector(0.0f, 0.0f, 0.0f), FVector up = FVector(0.0f, 1.0f, 0.0f), FVector2 planes = FVector2(0.1f, 100.0f), float fov = 90.0f) noexcept;

        FVector front{};
        FVector up{};
        FVector right{};
        FVector worldUp{};
        FVector translationOffset{};
        FVector rotationOffset{};
    private:
        const CoreComponent* core = nullptr;
        Projection proj;
    };
}