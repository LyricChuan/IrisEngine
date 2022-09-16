#pragma once

#include "../simple_library/public/simple_library.h"
#include "Debug/Log/SimpleLog.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include <wrl.h>//提供了智能指针，事件等，方便创建window应用平台程序
#include <dxgi1_4.h>//提供dxgi适配器，dxgi工厂，交换链等接口
//#include <d3d12.h>//d3d的api
#include "../Rendering/DirectX12/d3dx12.h"//d3d的api拓展
#include <d3dcompiler.h>//提供用于编译作色代码
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>//碰撞相关，AABB，OBB，胶囊体等
#include "Debug/EngineDebug.h"

using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

extern class FEngine* Engine;