/*
*
* < InterfaceDxgi.h >
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

		class InterfaceDxgi {
		public:
			InterfaceDxgi();
			InterfaceDxgi(const InterfaceDxgi&) = delete;
			InterfaceDxgi& operator=(const InterfaceDxgi&) = delete;
			~InterfaceDxgi();

			void BuildDxgiFactory();

			IDXGIFactory4* GetFactory4() const;

		private:

		private:
			ComPtr<IDXGIFactory4> m_DxgiFactory;

		};
	}
}