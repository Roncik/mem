#pragma once
#include "Parameter.h"

class Read : public Parameter
{
public:
	static constexpr char short_name = 'r';
	static constexpr const char* name = "read";
	static constexpr int arg_count = 3;

	Read() : Parameter(short_name, name, arg_count) {}

	virtual void handler() override;
};

