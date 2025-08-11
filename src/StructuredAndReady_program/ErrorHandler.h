#pragma once
#include <iostream>
#include <format>
#include <string>
#include <unordered_map>

enum class type 
{
	INFO = 0,
	ERROR = 1,
	CRITICAL_ERROR = 2
};

class ErrorHandler
{
public:
	void log(type pType, std::string_view pMsg, 
			 std::string_view pFileName, int32_t pLine) const;

private:
	[[noreturn]] constexpr std::string_view to_string(type pType) const;

};

