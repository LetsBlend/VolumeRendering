#pragma once
class Camera;

class SSPGui
{
public:
	ImGuiIO& StartUp();
	void Update();
	void Draw(Camera& cam);
	void ShutDown();
};

