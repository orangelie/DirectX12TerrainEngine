/*
*
* < InterfaceD3D12.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "../../../HEADER/PUBLIC/DxInterface/InterfaceD3D12.h"

namespace orangelie {

	namespace DxInterface {

		InterfaceD3D12::InterfaceD3D12() {
		}

		InterfaceD3D12::~InterfaceD3D12() {
		}

		void InterfaceD3D12::CreateDevice(IDXGIFactory4* DxgiFactory) {
			LRESULT lResult =  D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(m_Device.GetAddressOf()));

			// Software Adapter
			if (FAILED(lResult)) {
				ComPtr<IDXGIAdapter> warpAdapter;
				HR(DxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(warpAdapter.GetAddressOf())));
				D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(m_Device.GetAddressOf()));
			}
		}

		ID3D12Device* InterfaceD3D12::GetDevice() const {
			return m_Device.Get();
		}
	}
}