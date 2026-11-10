#pragma once
#include "Parameter.h"

namespace ParameterFactory
{
	std::unique_ptr<Parameter> create(char short_name, const char* name = nullptr);
};

