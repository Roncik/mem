#include "pch.h"
#include "ParameterFactory.h"
#include "Help.h"


namespace
{
	const char* gCurrentPath{};
}

// todo:
// dump game module
// class informer?
// persistent history?

// pattern scanning should have an option to only scan pages that are executable
// there should be an option to select operation on game module only or on all modules loaded in the process




int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        Help().handler();
        return 1;
    }
    
    gCurrentPath = argv[0]; // first argument is always current executable path
    int arg_counter{ 1 };
    for (; arg_counter < argc;)
    {
        const auto& parameter_arg{ argv[arg_counter] }; // always a parameter (- or --)

        if (parameter_arg[0] != '-' || strlen(parameter_arg) < 2)
        {
            std::cerr << "Parsing error at argument " << arg_counter << ". Argument value: " << parameter_arg << '\n';
            return 1;
        }

        std::unique_ptr<Parameter> parameter{};

        if (parameter_arg[1] != '-')
        {
            //short name
            const char& short_name = parameter_arg[1];
            parameter = ParameterFactory::create(short_name);
        }
        else
        {
            // long name
            std::string_view long_name(&parameter_arg[2]);
            parameter = ParameterFactory::create(NULL, long_name.data());
        }

        //parse additional arguments(add them to the parameter's argument list)
        ++arg_counter;
        const auto param_arg_count{ parameter.get()->get_num_of_arguments() };
        for (size_t i = 0; i < param_arg_count; ++i, ++arg_counter)
        {
            const auto& curr_arg{ argv[arg_counter] };
            if (curr_arg[0] != '-')
                parameter.get()->add_argument(curr_arg);
            else
                break;
        }

        parameter.get()->handler();
    }
    
    return 0;
}

