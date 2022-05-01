/*
* 
* < cppStdUtil.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
* 
*/





#pragma once

#include <iostream>
#include <fstream>

#include <exception>
#include <memory>

#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>

#define THROW(e) { \
	std::string __err_message = std::string(__FILE__) + ": " + e; \
	throw std::runtime_error(__err_message); \
} \

#define HR(n) if(n)