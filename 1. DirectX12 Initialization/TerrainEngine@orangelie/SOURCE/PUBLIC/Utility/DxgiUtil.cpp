/*
*
* < DxgiUtil.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "../../../HEADER/PUBLIC/Utility/DxgiUtil.h"

namespace orangelie {

	namespace Utility {

		UINT CalcConstantBufferByteSize(UINT cbSize) {
			return (cbSize + 255) & ~255;
		}

		ComPtr<ID3D12Resource> CreateDefaultBuffer(
			ID3D12Device* Device,
			ID3D12GraphicsCommandList* CommandList,
			const void* data,
			UINT64 size,
			ComPtr<ID3D12Resource>& Uploader) {
			using orangelie::CppStdUtil::unmove;

			ComPtr<ID3D12Resource> defaultBuffer;

			HR(Device->CreateCommittedResource(
				&unmove(CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT)),
				D3D12_HEAP_FLAG_NONE,
				&unmove(CD3DX12_RESOURCE_DESC::Buffer(size)),
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(defaultBuffer.GetAddressOf())));

			HR(Device->CreateCommittedResource(
				&unmove(CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD)),
				D3D12_HEAP_FLAG_NONE,
				&unmove(CD3DX12_RESOURCE_DESC::Buffer(size)),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(Uploader.GetAddressOf())));

			// resource barrier
			// updateSubresources
			// resource barrier
		}
	}
}