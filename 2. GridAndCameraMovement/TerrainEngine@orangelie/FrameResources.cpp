/*
*
* < FrameResource.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "FrameResources.h"

namespace orangelie {

	FrameResource::FrameResource(ID3D12Device* Device, UINT objCount, UINT passCount) {
		HR(Device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(m_CommandAllocator.GetAddressOf())));
		m_Fence = 0;

		m_ObjConstants = std::make_unique<orangelie::Gpu::UploadBuffer<ObjConstants>>(Device, objCount, true);
		m_PassConstants = std::make_unique<orangelie::Gpu::UploadBuffer<PassConstants>>(Device, passCount, true);
	}

	FrameResource::~FrameResource() {
	}
}