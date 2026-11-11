#pragma once
#include "Parameter.h"
class Help : public Parameter
{
public:
	static constexpr char short_name = 'h';
	static constexpr const char* name = "help";
	static constexpr int arg_count = 0;

	Help() : Parameter(short_name, name, arg_count) {}

	virtual void handler() override;
};

