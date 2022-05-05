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
#include "HEADER/PUBLIC/Gpu/UploadBuffer.h"

namespace orangelie {

	struct ObjConstants {
		XMFLOAT4X4 gWorld = orangelie::Utility::Tools::Identity();
	};

	struct PassConstants {
		XMFLOAT4X4 gProj = orangelie::Utility::Tools::Identity();;
		XMFLOAT4X4 gView = orangelie::Utility::Tools::Identity();;
		XMFLOAT4X4 gViewProj = orangelie::Utility::Tools::Identity();;
	};

	class FrameResource {
	public:
		FrameResource(ID3D12Device* Device, UINT objCount, UINT passCount);
		FrameResource(const FrameResource&) = delete;
		FrameResource& operator=(const FrameResource&) = delete;
		~FrameResource();

		ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
		UINT64 m_Fence;

		std::unique_ptr<orangelie::Gpu::UploadBuffer<ObjConstants>> m_ObjConstants = nullptr;
		std::unique_ptr<orangelie::Gpu::UploadBuffer<PassConstants>> m_PassConstants = nullptr;

	};
}