/*
*
* < InterfaceDxgi.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "../../../HEADER/PUBLIC/DxInterface/InterfeceDxgi.h"

namespace orangelie {

	namespace DxInterface {

		InterfaceDxgi::InterfaceDxgi() {
		}

		InterfaceDxgi::~InterfaceDxgi() {
		}

		void InterfaceDxgi::BuildDxgiFactory() {
			HR(CreateDXGIFactory1(IID_PPV_ARGS(m_DxgiFactory.GetAddressOf())));
		}

		IDXGIFactory4* InterfaceDxgi::GetFactory4() const {
			return m_DxgiFactory.Get();
		}
	}
}