#include "WindowsMessageProcessing.h"
#include "../../Component/Input/Input.h"
#include <WindowsX.h>

//消息队列
//window 鼠标键盘响应的回调会激活该函数，然后执行中间层input.h中定义好的输入代理，
//当更上层的业务(如，InputComponent)包含input.h文件并注册需要的代理后，便可把逻辑执行到该上层业务中
LRESULT CALLBACK EngineWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_RBUTTONDOWN:
		MouseDownDelegate.Broadcast(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		return 0;
	case WM_RBUTTONUP:
		MouseUpDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		MouseMoveDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEWHEEL:
		MousesWheelsDelegate.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam),(short)HIWORD(wParam));
		return 0;
	}

	//交给windows自己处理完成
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
