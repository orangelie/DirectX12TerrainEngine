/*
*
* < InterfaceD3D12.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include "../Utility/DxgiUtil.h"
#include "../Windows/Windows.h"

namespace orangelie {

	namespace DxInterface {

		class InterfaceD3D12 {
		public:
			InterfaceD3D12();
			InterfaceD3D12(const InterfaceD3D12&) = delete;
			InterfaceD3D12& operator=(const InterfaceD3D12&) = delete;
			~InterfaceD3D12();

			void CreateDevice(IDXGIFactory4* DxgiFactory);

			ID3D12Device* GetDevice() const;

		private:

		private:
			ComPtr<ID3D12Device> m_Device;

		};
	}
}