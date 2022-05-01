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
			m_Win32 = std::make_unique<orangelie::Windows::Win32>();
			m_Win32->Intialize(WindowProcedure, maxScreenWidth, maxScreenHeight, isFullscreenMode);
		}

		void ZekrosEngine::Run() {
			MSG msg = {};

			for (;;) {
				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					DispatchMessageW(&msg);
					TranslateMessage(&msg);
				}

				if (msg.message == WM_QUIT) {
					break;
				}


			}
		}

		LRESULT ZekrosEngine::MessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
			switch (uMessage)
			{
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