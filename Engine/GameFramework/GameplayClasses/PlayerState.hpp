// PlayerState.hpp
// Last update 12/8/2020 by Stanislav Vasilev(Madman10K)
#pragma once
#include <Core.hpp>
#include "../Actors/APawn.hpp"

namespace UVK
{
    UVK_API class PlayerState
    {
    public:
        virtual void BeginPlay() = 0;
        virtual void Tick(float DeltaTime) = 0;
        virtual void EndPlay() = 0;
    };
}