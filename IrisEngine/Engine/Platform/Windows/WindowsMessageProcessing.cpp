#include "WindowsMessageProcessing.h"
#include "../../Component/Input/Input.h"
#include <WindowsX.h>

//��Ϣ����
//window ��������Ӧ�Ļص��ἤ��ú�����Ȼ��ִ���м��input.h�ж���õ��������
//�����ϲ��ҵ��(�磬InputComponent)����input.h�ļ���ע����Ҫ�Ĵ���󣬱�ɰ��߼�ִ�е����ϲ�ҵ����
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

	//����windows�Լ��������
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
