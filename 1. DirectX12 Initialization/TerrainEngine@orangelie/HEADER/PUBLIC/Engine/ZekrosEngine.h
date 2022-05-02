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
#include "../Time/GameTimer.h"
#include "../DxInterface/InterfeceDxgi.h"
#include "../DxInterface/InterfaceD3D12.h"

namespace orangelie {
	namespace Engine {
		class ZekrosEngine {
		public:
			ZekrosEngine();
			ZekrosEngine(const ZekrosEngine&) = delete;
			ZekrosEngine& operator=(const ZekrosEngine&) = delete;
			~ZekrosEngine();

			void Initialize(int maxScreenWidth, int maxScreenHeight, bool isFullscreenMode);
			void Run();

			static ZekrosEngine* gZekrosEngine;
			virtual LRESULT MessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

		protected:
			virtual void update(float dt) = 0;
			virtual void draw(float dt) = 0;

			virtual void MouseDown(WPARAM btnState, int x, int y) = 0;
			virtual void MouseUp(WPARAM btnState, int x, int y) = 0;
			virtual void MouseMove(WPARAM btnState, int x, int y) = 0;

			int m_ClientWidth;
			int m_ClientHeight;

		private:
			std::unique_ptr<orangelie::Windows::Win32> m_Win32;
			std::unique_ptr<orangelie::Time::GameTimer> m_GameTimer = nullptr;
			std::unique_ptr<orangelie::DxInterface::InterfaceDxgi> m_DxgiInterface;
			std::unique_ptr<orangelie::DxInterface::InterfaceD3D12> m_D3D12Interface;

			bool m_IsEnginePaused = false;
			bool m_IsSizeMinimized = false;
			bool m_IsSizeMaximized = false;

		};
	}
}

LRESULT _stdcall WindowProcedure(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);