#pragma once
#include "Parameter.h"
class ModuleBase : public Parameter
{
public:
	static constexpr char short_name = 'b';
	static constexpr const char* name = "base";
	static constexpr int arg_count = 2;

	ModuleBase() : Parameter(short_name, name, arg_count) {}

	virtual void handler() override;
};

