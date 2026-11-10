#pragma once
#include "Parameter.h"

namespace ParameterFactory
{
	std::unique_ptr<Parameter> create(char short_name = 0, const char* name = nullptr);
};

