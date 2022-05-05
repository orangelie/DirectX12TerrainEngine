#include "HEADER/PUBLIC/Engine/ZekrosEngine.h"

static const int gNumFramesDirty = 3;

struct RenderItem {
	RenderItem() = default;

	XMFLOAT4X4 World = orangelie::Utility::Tools::Identity();

	int NumFramesDirty = gNumFramesDirty;

	orangelie::Mesh::MeshGeometry* MeshGeo = nullptr;

	D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology;

	UINT ObjCBIndex = 0;

	UINT IndexCount;
	UINT StartIndexLocation;
	UINT BaseVertexLocation;
};

struct Vertex {
	XMFLOAT3 Position;
	XMFLOAT4 Color;
};

class GridAndCameraMovement : public orangelie::Engine::ZekrosEngine {
private:
	void BuildRootSignature() {
		CD3DX12_ROOT_PARAMETER rootParameters[2];

		rootParameters[0].InitAsConstantBufferView(0);
		rootParameters[1].InitAsConstantBufferView(1);

		CD3DX12_ROOT_SIGNATURE_DESC signatureDesc(2, rootParameters, 0, nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


		ComPtr<ID3DBlob> ppCode = nullptr, ppErrorMsgs = nullptr;
		HRESULT hResult;

		hResult = D3D12SerializeRootSignature(&signatureDesc, D3D_ROOT_SIGNATURE_VERSION_1,
			ppCode.GetAddressOf(), ppErrorMsgs.GetAddressOf());

		if (ppErrorMsgs != nullptr) {
			MessageBoxA(0, (char*)ppErrorMsgs->GetBufferPointer(), "Rootsignature Error", MB_ICONWARNING);
		}
		HR(hResult);

		HR(m_Device->CreateRootSignature(0,
			ppCode->GetBufferPointer(), ppCode->GetBufferSize(),
			IID_PPV_ARGS(m_RootSignature.GetAddressOf())));
	}

	void BuildShaderSystem() {
		orangelie::Shader::ShaderRasterizeObjects2 RasterizeObj;
		std::vector<D3D_SHADER_MACRO> shaderMacro = { { NULL, NULL } };

		RasterizeObj.VS = { L"./Shader/Default.hlsl", "vs_5_1", "VS", shaderMacro, true };
		RasterizeObj.PS = { L"./Shader/Default.hlsl", "ps_5_1", "PS", shaderMacro, true };

		m_ShaderSys->ShaderCompileFromFile("sha1", RasterizeObj);

		m_InputElementDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
			0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputElementDesc.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
			0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	}

	void BuildBoxGeometry() {
		std::array<Vertex, 8> vertices =
		{
			Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), orangelie::Mesh::MeshColor::Cyan }),
			Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), orangelie::Mesh::MeshColor::Magenta }),
			Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), orangelie::Mesh::MeshColor::Yellow }),
			Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), orangelie::Mesh::MeshColor::Red }),
			Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), orangelie::Mesh::MeshColor::Green }),
			Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), orangelie::Mesh::MeshColor::Blue }),
			Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), orangelie::Mesh::MeshColor::Black }),
			Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), orangelie::Mesh::MeshColor::White })
		};

		std::array<std::uint16_t, 36> indices =
		{
			// front face
			0, 1, 2,
			0, 2, 3,

			// back face
			4, 6, 5,
			4, 7, 6,

			// left face
			4, 5, 1,
			4, 1, 0,

			// right face
			3, 2, 6,
			3, 6, 7,

			// top face
			1, 5, 6,
			1, 6, 2,

			// bottom face
			4, 0, 3,
			4, 3, 7
		};

		const UINT vertexBufferByteSize = (UINT)vertices.size() * sizeof(Vertex);
		const UINT indexBufferByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

		auto meshGeo = std::make_unique<orangelie::Mesh::MeshGeometry>();
		meshGeo->Name = "shapeGeo";

		HR(D3DCreateBlob(vertexBufferByteSize, &meshGeo->CpuVertexBuffer));
		CopyMemory(meshGeo->CpuVertexBuffer->GetBufferPointer(), vertices.data(), vertexBufferByteSize);
		HR(D3DCreateBlob(indexBufferByteSize, &meshGeo->CpuIndexBuffer));
		CopyMemory(meshGeo->CpuIndexBuffer->GetBufferPointer(), indices.data(), indexBufferByteSize);

		meshGeo->GpuVertexBuffer = orangelie::Utility::Tools::CreateDefaultBuffer(m_Device, m_CommandList.Get(),
			vertices.data(), vertexBufferByteSize,
			meshGeo->GpuVertexUploader);
		meshGeo->GpuIndexBuffer = orangelie::Utility::Tools::CreateDefaultBuffer(m_Device, m_CommandList.Get(),
			indices.data(), indexBufferByteSize,
			meshGeo->GpuIndexUploader);

		meshGeo->VertexBufferByteSize = vertexBufferByteSize;
		meshGeo->VertexByteStride = sizeof(Vertex);

		meshGeo->IndexBufferByteSize = indexBufferByteSize;
		meshGeo->IndexFormat = DXGI_FORMAT_R16_UINT;

		orangelie::Mesh::SubmeshGeometry Submesh;
		Submesh.IndexCount = (UINT)indices.size();
		Submesh.BaseVertexLocation = 0;
		Submesh.StartIndexLocation = 0;

		meshGeo->Submeshes["cube"] = Submesh;
		m_Geometrics[meshGeo->Name] = std::move(meshGeo);
	}

	void BuildRenderItems() {
		auto CubeRitem = std::make_unique<RenderItem>();
		CubeRitem->ObjCBIndex = 0;
		CubeRitem->World = orangelie::Utility::Tools::Identity();
		CubeRitem->MeshGeo = m_Geometrics["shapeGeo"].get();
		CubeRitem->IndexCount = CubeRitem->MeshGeo->Submeshes["cube"].IndexCount;
		CubeRitem->StartIndexLocation = CubeRitem->MeshGeo->Submeshes["cube"].StartIndexLocation;
		CubeRitem->BaseVertexLocation = CubeRitem->MeshGeo->Submeshes["cube"].BaseVertexLocation;
		CubeRitem->PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		CubeRitem->NumFramesDirty = 3;

		m_AllRitems.push_back(std::move(CubeRitem));
	}

	void BuildFrameResources() {
		for (size_t i = 0; i < gFrameResourceCount; ++i) {
			m_FrameResources.push_back(std::make_unique<orangelie::FrameResource>(m_Device, (UINT)m_AllRitems.size(), 1));
		}
	}

	void BuildPSOs() {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC GraphicsPSODesc = {};
		GraphicsPSODesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		GraphicsPSODesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		GraphicsPSODesc.DSVFormat = m_DepthStencilFormat;
		GraphicsPSODesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		GraphicsPSODesc.InputLayout = {
			m_InputElementDesc.data(),
			(UINT)m_InputElementDesc.size(),
		};
		GraphicsPSODesc.NodeMask = 0;
		GraphicsPSODesc.NumRenderTargets = 1;
		GraphicsPSODesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		GraphicsPSODesc.pRootSignature = m_RootSignature.Get();
		GraphicsPSODesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		GraphicsPSODesc.RTVFormats[0] = m_BackBufferFormat;
		GraphicsPSODesc.SampleDesc.Count = 1;
		GraphicsPSODesc.SampleDesc.Quality = 0;
		GraphicsPSODesc.SampleMask = UINT_MAX;

		auto shaderBin = m_ShaderSys->Resource("sha1");

		GraphicsPSODesc.VS = {
			reinterpret_cast<BYTE*>(shaderBin.VSbinCode->GetBufferPointer()),
			shaderBin.VSbinCode->GetBufferSize()
		};
		GraphicsPSODesc.PS = {
			reinterpret_cast<BYTE*>(shaderBin.PSbinCode->GetBufferPointer()),
			shaderBin.PSbinCode->GetBufferSize()
		};

		HR(m_Device->CreateGraphicsPipelineState(&GraphicsPSODesc, IID_PPV_ARGS(m_PSOs["opaque"].GetAddressOf())));
	}

	void DrawRenderItems() {
		auto ObjConst = m_CurrFrameResource->m_ObjConstants->Resource();
		const UINT objSize = orangelie::Utility::Tools::CalcConstantBufferByteSize(sizeof(orangelie::ObjConstants));

		for (auto& r : m_AllRitems) {
			D3D12_GPU_VIRTUAL_ADDRESS objAddress = ObjConst->GetGPUVirtualAddress() + objSize * r->ObjCBIndex;
			m_CommandList->SetGraphicsRootConstantBufferView(0, objAddress);

			m_CommandList->IASetVertexBuffers(0, 1, &orangelie::CppStdUtil::unmove(r->MeshGeo->VertexBufferView()));
			m_CommandList->IASetIndexBuffer(&orangelie::CppStdUtil::unmove(r->MeshGeo->IndexBufferView()));
			m_CommandList->IASetPrimitiveTopology(r->PrimitiveTopology);

			m_CommandList->DrawIndexedInstanced(r->IndexCount, 1, r->StartIndexLocation, r->BaseVertexLocation, 0);
		}
	}

	void UpdateObjectCBs() {
		auto objCB = m_CurrFrameResource->m_ObjConstants.get();

		for (auto& e : m_AllRitems) {
			if (e->NumFramesDirty > 0) {
				XMMATRIX world = XMLoadFloat4x4(&e->World);

				orangelie::ObjConstants objConstants;
				XMStoreFloat4x4(&objConstants.gWorld, XMMatrixTranspose(world));

				objCB->CopyData(e->ObjCBIndex, objConstants);

				--(e->NumFramesDirty);
			}
		}
	}

	void UpdatePassCBs() {
		auto passCB = m_CurrFrameResource->m_PassConstants.get();

		XMMATRIX P = XMLoadFloat4x4(&orangelie::CppStdUtil::unmove(m_Camera.GetProjectionMatrix()));
		XMMATRIX V = XMLoadFloat4x4(&orangelie::CppStdUtil::unmove(m_Camera.GetViewMatrix()));

		orangelie::PassConstants passConstants;
		XMStoreFloat4x4(&passConstants.gProj, XMMatrixTranspose(P));
		XMStoreFloat4x4(&passConstants.gView, XMMatrixTranspose(V));
		XMStoreFloat4x4(&passConstants.gViewProj, XMMatrixTranspose(XMMatrixMultiply(V, P)));

		passCB->CopyData(0, passConstants);
	}

	void OnKeyboardInput(float dt) {
		const float speed = 5.0f;

		if ((GetAsyncKeyState('W') & 0x8000))
			m_Camera.Walk(speed * dt);
		if ((GetAsyncKeyState('S') & 0x8000))
			m_Camera.Walk(-speed * dt);
		if ((GetAsyncKeyState('A') & 0x8000))
			m_Camera.Strafe(-speed * dt);
		if ((GetAsyncKeyState('D') & 0x8000))
			m_Camera.Strafe(speed * dt);

		m_Camera.UpdateViewMatrix();
	}

protected:
	virtual void OnResize() {
		ZekrosEngine::OnResize();

		// TODO: Something here...
		m_Camera.SetLens(0.25f * XM_PI, ((float)m_ClientWidth / m_ClientHeight), 1.0f, 1000.0f);
	}

	virtual void init() {
		m_Camera.SetPosition(0.0f, 1.0f, -5.0f);

		HR(m_CommandList->Reset(m_CommandAllocator.Get(), nullptr));

		// TODO: Something here...
		BuildRootSignature();
		BuildShaderSystem();
		BuildBoxGeometry();
		BuildRenderItems();
		BuildFrameResources();
		BuildPSOs();


		SubmitCommandList();
		FlushCommandQueue();
	}

	virtual void update(float dt) override {
		OnKeyboardInput(dt);

		m_CurrFrameResourceIndex = (m_CurrFrameResourceIndex + 1) % gFrameResourceCount;
		m_CurrFrameResource = m_FrameResources[m_CurrFrameResourceIndex].get();
		m_CurrFrameResource->m_Fence = m_CurrentFenceCount;

		if (m_Fence->GetCompletedValue() < m_CurrFrameResource->m_Fence && m_CurrFrameResource->m_Fence != 0) {
			HANDLE hEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
			m_Fence->SetEventOnCompletion(m_CurrFrameResource->m_Fence, hEvent);
			WaitForSingleObject(hEvent, 0xffffffff);
			CloseHandle(hEvent);
		}

		UpdateObjectCBs();
		UpdatePassCBs();
	}

	virtual void draw(float dt) override {
		HR(m_CurrFrameResource->m_CommandAllocator->Reset());
		HR(m_CommandList->Reset(m_CurrFrameResource->m_CommandAllocator.Get(), m_PSOs["opaque"].Get()));

		auto rtvHandle = CurrentBackBufferView();
		auto dsvHandle = DepthStencilView();

		using orangelie::CppStdUtil::unmove;
		m_CommandList->ResourceBarrier(1, &unmove(CD3DX12_RESOURCE_BARRIER::Transition(
			SwapChainResource(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET)));

		const FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_CommandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);
		m_CommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		m_CommandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);

		m_CommandList->RSSetViewports(1, &m_Viewport);
		m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());

		auto PassConst = m_CurrFrameResource->m_PassConstants->Resource();
		m_CommandList->SetGraphicsRootConstantBufferView(1, PassConst->GetGPUVirtualAddress());

		DrawRenderItems();

		m_CommandList->ResourceBarrier(1, &unmove(CD3DX12_RESOURCE_BARRIER::Transition(
			SwapChainResource(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT)));

		SubmitCommandList();
		PresentSwapChain();

		m_CurrentSwapBufferIndex = (m_CurrentSwapBufferIndex + 1) % gBackBufferCount;

		// Only without frame resources.
		// FlushCommandQueue(); 

		m_CurrFrameResource->m_Fence = ++m_CurrentFenceCount;
		HR(m_CommandQueue->Signal(m_Fence, m_CurrFrameResource->m_Fence));
	}

	virtual void MouseDown(WPARAM btnState, int x, int y) override {
		m_LastPrevPoint.x = x;
		m_LastPrevPoint.y = y;

		SetCapture(m_hWnd);
	}

	virtual void MouseUp(WPARAM btnState, int x, int y) override {
		ReleaseCapture();
	}

	virtual void MouseMove(WPARAM btnState, int x, int y) override {
		if ((btnState & MK_RBUTTON) != 0) {

		}

		else if ((btnState & MK_LBUTTON) != 0) {
			float dx = XMConvertToRadians(0.25f * (static_cast<float>(x - m_LastPrevPoint.x)));
			float dy = XMConvertToRadians(0.25f * (static_cast<float>(y - m_LastPrevPoint.y)));

			m_Camera.Pitch(dy);
			m_Camera.RotateY(dx);
		}

		m_LastPrevPoint.x = x;
		m_LastPrevPoint.y = y;
	}

private:
	ComPtr<ID3D12RootSignature> m_RootSignature;
	std::unique_ptr<orangelie::Shader::ShaderSystem> m_ShaderSys = std::make_unique<orangelie::Shader::ShaderSystem>();
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputElementDesc;

	std::vector<std::unique_ptr<RenderItem>> m_AllRitems;
	std::unordered_map<std::string, std::unique_ptr<orangelie::Mesh::MeshGeometry>> m_Geometrics;
	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> m_PSOs;

	POINT m_LastPrevPoint;

	orangelie::Camera::DefaultCamera m_Camera;

};