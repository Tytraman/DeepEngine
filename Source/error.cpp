#include <DE/error.hpp>

#include <stdio.h>

const char *deep::error::Error = "Error";
const char *deep::error::Warning = "Warning";
const char *deep::error::Success = "Success";

void deep::error::printError(error_status status) {
	const char *error;

	DE_ERROR_SW(status,
		default: error = "Unknown"; break;
		DE_ERROR_CASE(error_status::NoError);
		DE_ERROR_CASE(error_status::InitSDL);
		DE_ERROR_CASE(error_status::CreateWindowSDL);
		DE_ERROR_CASE(error_status::CreateRendererSDL);
		DE_ERROR_CASE(error_status::GLCreateContext);
	)

	printf("[%s] %s\n", error::Error, error);
}
