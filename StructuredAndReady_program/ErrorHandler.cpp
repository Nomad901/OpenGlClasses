#include "ErrorHandler.h"

void ErrorHandler::log(type pType, std::string_view pMsg, 
					   std::string_view pFileName, int32_t pLine) const
{
	std::cout << std::format("[{}] {}, [FILENAME] {}, [LINE] {}\n", to_string(pType), pMsg, pFileName, pLine);
}

[[noreturn]] constexpr std::string_view ErrorHandler::to_string(type pType) const
{
	switch (pType)
	{
	case type::INFO:
		return "INFO";
		break;
	case type::ERROR:
		return "ERROR";
		break;
	case type::CRITICAL_ERROR:
		return "CRITICAL_ERROR";
		break;
	}
	std::terminate();
}
