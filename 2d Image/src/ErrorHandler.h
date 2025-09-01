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
	static ErrorHandler& getInstance();

	void log(type pType, std::string_view pMsg, 
			 std::string_view pFileName, int32_t pLine) const;

protected:
	ErrorHandler() = default;
	~ErrorHandler();

private:
	ErrorHandler(const ErrorHandler&) = delete;
	ErrorHandler& operator=(const ErrorHandler&) = delete;
	ErrorHandler(ErrorHandler&&) = delete;
	ErrorHandler& operator=(ErrorHandler&&) = delete;

	[[noreturn]] constexpr std::string_view to_string(type pType) const;

private:
	static ErrorHandler* mInstance;
};

#define LOG(pType, pMsg) ErrorHandler::getInstance().log(pType, pMsg, __FILE__, __LINE__) 
