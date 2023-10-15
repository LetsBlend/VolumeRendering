#include "pch.h"
#include "ImGui.h"
#include "SSPGui.h"
#include "imgui.h"
#include "ImGui/ImGuiDXRenderer.h"
#include "ImGui/ImGuiWinRenderer.h"
#include "Window/Window.h"
#include "Renderer/D3D11.h"
#include "Renderer/Camera.h"

ImGuiIO& SSPGui::StartUp()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(Window::GetWindowHandle());
	ImGui_ImplDX11_Init(D3D11::_device.Get(), D3D11::_deviceContext.Get());
	return io;
}

void SSPGui::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void SSPGui::Draw(Camera& cam)
{
	ImGui::Render();
	D3D11::_deviceContext->OMSetRenderTargets(1, cam.m_renderTargetView.GetAddressOf(), nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void SSPGui::ShutDown()
{
}
