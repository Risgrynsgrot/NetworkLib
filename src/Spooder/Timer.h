#pragma once

#include <chrono>

namespace Spooder
{
	class Timer
	{
	public:

		Timer(const Timer& aTimer) = delete;
		Timer& operator = (const Timer& aTimer) = delete;

		void Update();

		void SetIsPaused(bool aIsPausedFlag);
		float GetDeltaTime() const;
		double GetDeltaTimeDouble() const;
		double GetTotalTime() const;
	    int GetFps() const;

		static Timer* GetInstance();

		bool IsPaused() const { return myIsPaused; };

	private:
		Timer();
		static Timer* ourInstance;

		std::chrono::high_resolution_clock myClock;
		std::chrono::high_resolution_clock::time_point myStartTime;
		std::chrono::high_resolution_clock::time_point myFrameTime;
		std::chrono::high_resolution_clock::time_point myLastFrameTime;
	    bool myIsPaused = false;
	};
}