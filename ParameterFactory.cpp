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
}
