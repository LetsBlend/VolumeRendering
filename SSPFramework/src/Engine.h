#pragma once
class Engine
{
public:
	void StartUp()
	{

	}
	void ShutDown()
	{

	}

	void Run();

	inline static bool VSync = false;
	inline static int targetFrameRate = 120;

private:
	inline static bool UpdateApplication = true;
};

