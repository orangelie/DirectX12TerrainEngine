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

		private:
			void CreateCommandObjects();

		protected:
			ID3D12Resource* SwapChainResource() const;
			D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
			D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

			void PresentSwapChain();
			void SubmitCommandList();
			void FlushCommandQueue();

			virtual void OnResize();

			virtual void update(float dt) = 0;
			virtual void draw(float dt) = 0;

			virtual void MouseDown(WPARAM btnState, int x, int y) = 0;
			virtual void MouseUp(WPARAM btnState, int x, int y) = 0;
			virtual void MouseMove(WPARAM btnState, int x, int y) = 0;

			int m_ClientWidth;
			int m_ClientHeight;

			ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
			ComPtr<ID3D12GraphicsCommandList> m_CommandList;
			ComPtr<ID3D12CommandQueue> m_CommandQueue;

			D3D12_VIEWPORT m_Viewport;
			RECT m_ScissorRect;


			int m_CurrentSwapBufferIndex = 0;
			static const int gBackBufferCount = 2;

		private:
			ID3D12Device* m_Device = nullptr;

			std::unique_ptr<orangelie::Windows::Win32> m_Win32 = nullptr;
			orangelie::Time::GameTimer m_GameTimer;
			std::unique_ptr<orangelie::DxInterface::InterfaceDxgi> m_DxgiInterface = nullptr;
			std::unique_ptr<orangelie::DxInterface::InterfaceD3D12> m_D3D12Interface = nullptr;

			bool m_IsEnginePaused = false;
			bool m_IsSizeMinimized = false;
			bool m_IsSizeMaximized = false;

			UINT64 m_CurrentFenceCount = 0;

			const DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			const DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

			ComPtr<ID3D12Resource> m_SwapChainBuffer[gBackBufferCount];
			ComPtr<ID3D12Resource> m_DepthStencilBuffer = nullptr;
		};
	}
}

LRESULT _stdcall WindowProcedure(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);