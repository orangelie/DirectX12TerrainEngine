/*
*
* < ZekrosEngine.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "../../../HEADER/PUBLIC/Engine/ZekrosEngine.h"

namespace orangelie {

	namespace Engine {

		ZekrosEngine::ZekrosEngine() {
			gZekrosEngine = this;
		}

		ZekrosEngine::~ZekrosEngine() {
		}

		void ZekrosEngine::Initialize(int maxScreenWidth, int maxScreenHeight, bool isFullscreenMode) {
			// Win32
			m_Win32 = std::make_unique<orangelie::Windows::Win32>();
			m_Win32->Intialize(WindowProcedure, maxScreenWidth, maxScreenHeight, isFullscreenMode);

			// Dxgi
			m_DxgiInterface = std::make_unique<orangelie::DxInterface::InterfaceDxgi>();
			m_DxgiInterface->BuildDxgiFactory();

			// GameTimer
			m_GameTimer = std::make_unique<orangelie::Time::GameTimer>();

			// D3D12
			m_D3D12Interface = std::make_unique<orangelie::DxInterface::InterfaceD3D12>();
			m_D3D12Interface->CreateDevice(m_DxgiInterface->GetFactory4());
		}

		void ZekrosEngine::Run() {
			MSG msg = {};

			m_GameTimer->Reset();

			for (;;) {
				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					DispatchMessageW(&msg);
					TranslateMessage(&msg);
				}

				if (msg.message == WM_QUIT) {
					break;
				}

				if (m_IsEnginePaused) continue;

				update(m_GameTimer->DeltaTime());
				draw(m_GameTimer->DeltaTime());

				m_GameTimer->Tick();
			}
		}

		LRESULT ZekrosEngine::MessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
			switch (uMessage) {
			case WM_ACTIVATE:
				if (LOWORD(wParam) == WA_INACTIVE) {
					if(m_GameTimer.get() != nullptr) m_GameTimer->Stop();
					m_IsEnginePaused = true;
				}
				else {
					if (m_GameTimer.get() != nullptr) m_GameTimer->Start();
					m_IsEnginePaused = false;
				}
				return 0;

			case WM_SIZE:
				m_ClientWidth = LOWORD(lParam);
				m_ClientHeight = HIWORD(lParam);

				if (wParam == SIZE_MINIMIZED) {
					m_IsSizeMinimized = true;
					m_IsSizeMaximized = false;
					m_IsEnginePaused = true;
				}

				else if (wParam == SIZE_MAXIMIZED) {
					m_IsSizeMinimized = false;
					m_IsSizeMaximized = true;
					m_IsEnginePaused = false;
				}

				else if (wParam == SIZE_RESTORED) {
					if (m_IsSizeMinimized) {
						m_IsSizeMinimized = false;
						m_IsEnginePaused = false;
					}
					if (m_IsSizeMaximized) {
						m_IsSizeMaximized = false;
						m_IsEnginePaused = false;
					}
				}
				return 0;

			case WM_ENTERSIZEMOVE:
				m_IsEnginePaused = true;
				if (m_GameTimer.get() != nullptr) m_GameTimer->Stop();
				return 0;
			case WM_EXITSIZEMOVE:
				m_IsEnginePaused = false;
				if (m_GameTimer.get() != nullptr) m_GameTimer->Start();
				return 0;

			case WM_MOUSEMOVE:
				MouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;

			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_LBUTTONDOWN:
				MouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;

			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			case WM_LBUTTONUP:
				MouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;

			case WM_GETMINMAXINFO:
				((MINMAXINFO*)lParam)->ptMinTrackSize = POINT(200, 200);
				return 0;

			case WM_MENUCHAR:
				MAKELRESULT(0, MNC_CLOSE);
				return 0;

			case WM_KEYDOWN:
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					PostQuitMessage(0);
				}
				return 0;

			case WM_DESTROY:
			case WM_CLOSE:
				PostQuitMessage(0);
				return 0;
			}

			return DefWindowProcW(hWnd, uMessage, wParam, lParam);
		}
	}

}

orangelie::Engine::ZekrosEngine* orangelie::Engine::ZekrosEngine::gZekrosEngine = nullptr;
LRESULT _stdcall WindowProcedure(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	return orangelie::Engine::ZekrosEngine::gZekrosEngine->MessageHandler(hWnd, uMessage, wParam, lParam);
}