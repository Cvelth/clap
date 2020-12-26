#pragma once
#include "resource/configuration.hpp"

namespace clap::resource {
	// The function is non-blocking with identification itself done on a separate thread.
	// As such, it's recommended to call 'resource::wait()' function before actually using resources 
	// and, especially, configuration.
	void identify();
	void wait();
	bool were_identified();
}