#include "window.hpp"

int main() {
	window w("rrl", window_mode::windowed, 1280, 720);
	return w.loop();
}