#pragma once
#include "Parameter.h"

class Read : public Parameter
{
public:
	static constexpr char short_name = 'r';
	static constexpr const char* name = "read";
	static constexpr int arg_name = 3;

	Read() : Parameter(short_name, name, arg_name) {}

	virtual void handler() override;
};

