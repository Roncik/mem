#pragma once
class Parameter
{
protected:
	char short_name{};
	std::string name{};
	int num_of_arguments{};
	std::vector<const char*> arguments{}; //arguments here are strings not literal bytes

	Parameter(char short_name, const char* name, int num_of_arguments) : short_name{ short_name }, name{ name }, num_of_arguments{ num_of_arguments } {}
	Parameter(char short_name, const char* name) : Parameter(short_name, name, 0) {}
	Parameter(char short_name) : Parameter(short_name, nullptr) {}

public:
	void add_argument(const char* arg) { arguments.push_back(arg); }
	int get_num_of_arguments() { return num_of_arguments; }

	virtual void handler() = 0;
};

//When I add parameters I will have to implement a factory that will create the parameter objects based on the short_name or name


