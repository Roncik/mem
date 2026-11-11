#pragma once
#include "Parameter.h"
class Patternscan : public Parameter
{
public:
	static constexpr char short_name = 'p';
	static constexpr const char* name = "pattern";
	static constexpr int arg_count = 4;

	Patternscan() : Parameter(short_name, name, arg_count) {}

	virtual void handler() override;
};

