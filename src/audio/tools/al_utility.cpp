#include "al_utility.hpp"

namespace Zen {

bool check_al_errors (const std::string& filename, const std::uint_fast32_t line)
{
	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr
			<< "***ERROR*** (" << filename << ": " << line << ")"
			<< std::endl;
		switch (error)
		{
			case AL_INVALID_NAME:
				std::cerr << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
				break;
			case AL_INVALID_ENUM:
				std::cerr << "AL_INVALID_ENUM: a bad enum value was passed to an OpenAL function";
				break;
			case AL_INVALID_VALUE:
				std::cerr << "AL_INVALID_VALUE: a bad value was passed to an OpenAL function";
				break;
			case AL_INVALID_OPERATION:
				std::cerr << "AL_INVALID_OPERATION: the requested operation is not valid";
				break;
			case AL_OUT_OF_MEMORY:
				std::cerr << "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
				break;
			default:
				std::cerr << "UNKNOWN AL ERROR: " << error;
		}

		std::cerr << std::endl;
		return false;
	}
	return true;
}

}	// namespace Zen
