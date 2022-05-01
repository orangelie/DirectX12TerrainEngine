#pragma once
/*
*
* < ZekrosEngine.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include "../Windows/Windows.h"

namespace orangelie {
	namespace Engine {
		class ZekrosEngine sealed {
		public:
			ZekrosEngine();
			ZekrosEngine(const ZekrosEngine&) = delete;
			ZekrosEngine& operator=(const ZekrosEngine&) = delete;
			~ZekrosEngine();

			void Initialize(int maxScreenWidth, int maxScreenHeight, bool isFullscreenMode);
			void Run();

			static ZekrosEngine* gZekrosEngine;
			static LRESULT MessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

		private:


		private:
			std::unique_ptr<orangelie::Windows::Win32> m_Win32;

		};
	}
}

LRESULT _stdcall WindowProcedure(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);