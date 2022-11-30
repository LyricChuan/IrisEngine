#include "ObjectTransformation.h"

FObjectTransformation::FObjectTransformation()
    :World(EngineMath::IdentityMatrix4x4())
    ,TextureTransformation(EngineMath::IdentityMatrix4x4())
    ,MaterialIndex(0)
    ,Padding1(0)
    ,Padding2(0)
    ,Padding3(0)
{

}
