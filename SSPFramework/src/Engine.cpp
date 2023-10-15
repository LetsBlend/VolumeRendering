#include "pch.h"
#include "SSPFramework.h"

Window gWindow;
D3D11 gD3D11;
Time gTime;
SSPGui gSSPGui;

void Engine::Run()
{
	Math::vec2 vec = Math::vec2(2, 3);
	std::cout << vec.x << ", " << vec.y << std::endl;

	gWindow.StartUp(1920, 1080, &UpdateApplication);

	gD3D11.StartUp();

	ImGuiIO& io = gSSPGui.StartUp();

	Material mat{ L"Assets/Shaders/Main.vs.hlsl", L"Assets/Shaders/Main.ps.hlsl" };

	MeshRenderer mesh{ &mat, "Assets/Models/Ducky.fbx" };
	Transform meshTrans{};
	meshTrans.SetRotation(Math::vec3(-90, 0, 0));

	Camera cam{};
	Transform camTrans{};

	bool show_demo_window = true;
	bool show_another_window = true;
	while (UpdateApplication)
	{
		gTime.UpdateStartTime();

		gWindow.UpdateEvents(gD3D11, cam);

		cam.Clear();

		gSSPGui.Update();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello FFPFramework!");  // Create a window called "Hello, world!" and append into it.
			ImGui::Text("I like.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::Checkbox("VSync", &Engine::VSync);
			float frameRate = (float)Engine::targetFrameRate;
			ImGui::SliderFloat("Framerate", &frameRate, 10, 255, "%f");
			Engine::targetFrameRate = (int)frameRate;

			ImGui::End();
		}

		{
			ImGui::Begin("Cube Window!"); // Create a window called "Hello, world!" and append into it.
			ImGui::DragFloat3("Position", (float*)&meshTrans.m_position, .1f);
			ImGui::DragFloat3("Rotation", (float*)&meshTrans.m_pitchYawRoll, .1f);
			ImGui::DragFloat3("Scale", (float*)&meshTrans.m_scale, .1f);
			meshTrans.SetPosition(meshTrans.m_position);
			meshTrans.SetRotation(meshTrans.m_pitchYawRoll);
			meshTrans.SetScale(meshTrans.m_scale);

			ImGui::End();
		}

		// Camera Window
		{
			ImGui::Begin("Camera Window!");                          // Create a window called "Hello, world!" and append into it.
			ImGui::Text("TODO: Add Stuff my guy");
			ImGui::DragFloat3("Position", (float*)&camTrans.m_position, .1f);
			ImGui::DragFloat3("Rotation", (float*)&camTrans.m_pitchYawRoll, 1);

			camTrans.SetRotation(camTrans.m_pitchYawRoll);
			ImGui::End();
		}

		float moveSpeed = 1;
		Math::vec3 dirR = Math::vec3(camTrans.GetRight().x * moveSpeed, 0, camTrans.GetRight().z * moveSpeed);

		Math::vec3 up = Math::vec3{ 0, 1, 0 };
		Math::vec3 forw = Math::cross(dirR, up);

		if (Input::GetKey(KeyCode::W))
			camTrans.Translate(Math::vec3{ forw.x * (float)Time::deltaTime, 0, forw.z * (float)Time::deltaTime });
		if (Input::GetKey(KeyCode::S))
			camTrans.Translate(Math::vec3{ -forw.x * (float)Time::deltaTime, 0, -forw.z * (float)Time::deltaTime });
		if (Input::GetKey(KeyCode::A))
			camTrans.Translate(Math::vec3{ -dirR.x * (float)Time::deltaTime, 0, -dirR.z * (float)Time::deltaTime });
		if (Input::GetKey(KeyCode::D))
			camTrans.Translate(Math::vec3{ dirR.x * (float)Time::deltaTime, 0, dirR.z * (float)Time::deltaTime });

		if (Input::GetKey(KeyCode::Space))
			camTrans.Translate(Math::vec3(0, moveSpeed * (float)Time::deltaTime, 0));
		if (Input::GetKey(KeyCode::LShift))
			camTrans.Translate(Math::vec3(0, -moveSpeed * (float)Time::deltaTime, 0));

		float rotSpeed = .1f;
		if (Input::GetButton(Button::RButton))
		{
			Cursor::cursorMode = CursorMode::LockedAndHidden;
			camTrans.Rotate(Math::vec3(Events::mouseDelta.y * rotSpeed, Events::mouseDelta.x * rotSpeed, 0));
		}
		else
			Cursor::cursorMode = CursorMode::None;

		cam.Update(camTrans);

		mesh.Update(meshTrans);

		mesh.Render(cam);

		gSSPGui.Draw(cam);

		gD3D11.Present();

		gWindow.HandleEventBuffer();

		gTime.UpdateEndTime();
	}

	cam.Flush();
	mesh.Flush();
	mat.Flush();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	gD3D11.ShutDown();
	gWindow.ShutDown();
}
