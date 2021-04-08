#include "Timer.h"

namespace Spooder
{
Timer* Timer::ourInstance = nullptr;

Timer::Timer()
{
	ourInstance     = this;
	myStartTime     = myClock.now();
	myFrameTime     = myStartTime;
	myLastFrameTime = myStartTime;
	myIsPaused      = false;
}

void Timer::Update()
{
	myLastFrameTime = myFrameTime;
	myFrameTime     = myClock.now();
}

void Timer::SetIsPaused(bool aIsPausedFlag)
{
	myIsPaused = aIsPausedFlag;
}

float Timer::GetDeltaTime() const
{
	if (myIsPaused)
	{
		return 0.0f;
	}
	return std::chrono::duration<float>(myFrameTime - myLastFrameTime).count();
}

double Timer::GetDeltaTimeDouble() const
{
	if (myIsPaused)
	{
		return 0.0;
	}
	return std::chrono::duration<double>(myFrameTime - myLastFrameTime).count();
}

double Timer::GetTotalTime() const
{
	return std::chrono::duration<double>(myClock.now() - myStartTime).count();
}
Timer* Timer::GetInstance()
{
	if (ourInstance == nullptr)
	{
		ourInstance = new Timer();
	}
	return ourInstance;
}
int Timer::GetFps() const
{
	if (static_cast<int>(1.0f / GetDeltaTime()) > 0)
	{
		return static_cast<int>((1.0f / GetDeltaTime()));
	}
	return 0;
}
}  // namespace Spooder