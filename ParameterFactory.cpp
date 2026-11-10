#include "pch.h"
#include "ParameterFactory.h"
#include "Read.h"

std::unique_ptr<Parameter> ParameterFactory::create(char short_name, const char* name)
{
    if (short_name)
    {
        switch (short_name)
        {
        case Read::short_name: return std::make_unique<Read>();
        }
    }

    if (name)
    {
        std::string_view sv{ name };

        if (sv == Read::name) return std::make_unique<Read>();
    }

    std::cerr << "Invalid parameter name was passed: " << (short_name ? std::string(short_name, 1) : std::string(name)) << '\n';
    exit(1);
    //do not throw exceptions, ai might not be able to handle them when using a debug binary.
    //throw std::logic_error("Invalid arguments passed to ParameterFactory::create");
}
