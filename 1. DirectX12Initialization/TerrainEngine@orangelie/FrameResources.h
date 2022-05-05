/*
*
* < FrameResources.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include "HEADER/PUBLIC/Utility/DxgiUtil.h"

namespace orangelie {

	class FrameResource {
	public:
		FrameResource(ID3D12Device* Device);
		FrameResource(const FrameResource&) = delete;
		FrameResource& operator=(const FrameResource&) = delete;
		~FrameResource();

		ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
		UINT64 m_Fence;

	};
}