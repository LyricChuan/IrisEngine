#pragma once

#include "../simple_library/public/simple_library.h"
#include "Debug/Log/SimpleLog.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include <wrl.h>//�ṩ������ָ�룬�¼��ȣ����㴴��windowӦ��ƽ̨����
#include <dxgi1_4.h>//�ṩdxgi��������dxgi�������������Ƚӿ�
//#include <d3d12.h>//d3d��api
#include "../Rendering/DirectX12/d3dx12.h"//d3d��api��չ
#include <d3dcompiler.h>//�ṩ���ڱ�����ɫ����
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>//��ײ��أ�AABB��OBB���������
#include "Debug/EngineDebug.h"

using namespace Microsoft::WRL;
using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

extern class FEngine* Engine;