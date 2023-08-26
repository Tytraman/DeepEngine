#include <DE/error.hpp>

#include <stdio.h>

const char *de::DError::Error = "Error";
const char *de::DError::Warning = "Warning";
const char *de::DError::Success = "Success";

void de::DError::printError(ErrorStatus status) {
	const char *error;

	DE_ERROR_SW(status,
		default: error = "Unknown"; break;
		DE_ERROR_CASE(NoError);
		DE_ERROR_CASE(InitSDL);
		DE_ERROR_CASE(CreateWindowSDL);
		DE_ERROR_CASE(CreateRendererSDL);
		DE_ERROR_CASE(GLCreateContext);
	)

	printf("[%s] %s\n", DError::Error, error);
}
