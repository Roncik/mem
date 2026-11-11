#pragma once
#include "Parameter.h"

class Write : public Parameter
{
public:
	static constexpr char short_name = 'w';
	static constexpr const char* name = "write";
	static constexpr int arg_count = 4;

	Write() : Parameter(short_name, name, arg_count) {}

	virtual void handler() override;
};

