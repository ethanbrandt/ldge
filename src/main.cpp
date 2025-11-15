#include <iostream>
#include <thread>
#include <chrono>

const float FRAMES_PER_SECOND = 60;

int main()
{
	int testFrames = 4000;
	const int frameDuration = int(1000 / FRAMES_PER_SECOND);

	for (int i = 0; i < testFrames; ++i)
	{
		std::cout << "Frame " << i + 1 << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(frameDuration));
	}

	return 0;
}