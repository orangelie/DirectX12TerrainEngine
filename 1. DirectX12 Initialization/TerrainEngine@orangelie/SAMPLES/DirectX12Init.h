#include "../HEADER/PUBLIC/Engine/ZekrosEngine.h"

class DirectX12Init : public orangelie::Engine::ZekrosEngine {
protected:
	void update(float dt) override {

	}

	void draw(float dt) override {
		HR(m_CommandAllocator->Reset());
		HR(m_CommandList->Reset(m_CommandAllocator.Get(), nullptr));

		auto rtvHandle = CurrentBackBufferView();
		auto dsvHandle = DepthStencilView();

		using orangelie::CppStdUtil::unmove;
		m_CommandList->ResourceBarrier(1, &unmove(CD3DX12_RESOURCE_BARRIER::Transition(
			SwapChainResource(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET)));

		const FLOAT color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_CommandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);
		m_CommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		m_CommandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);

		m_CommandList->RSSetViewports(1, &m_Viewport);
		m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

		m_CommandList->ResourceBarrier(1, &unmove(CD3DX12_RESOURCE_BARRIER::Transition(
			SwapChainResource(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT)));

		SubmitCommandList();
		PresentSwapChain();

		m_CurrentSwapBufferIndex = (m_CurrentSwapBufferIndex + 1) % gBackBufferCount;
		FlushCommandQueue();
	}

	void MouseDown(WPARAM btnState, int x, int y) override {

	}

	void MouseUp(WPARAM btnState, int x, int y) override {

	}

	void MouseMove(WPARAM btnState, int x, int y) override {

	}

private:

};