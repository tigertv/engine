/**
 * @file
 */

#pragma once

#include "animation/chr/CharacterSkeleton.h"
#include "animation/Animation.h"

namespace animation {
namespace chr {
namespace tool {
extern void update(float animTime, ToolAnimationType animation, CharacterSkeleton& skeleton, const CharacterSkeletonAttribute& skeletonAttr);
}
}
}
