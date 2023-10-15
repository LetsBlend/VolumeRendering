#pragma once
#include "pch.h"
#include "Engine.h"

int main()
{
	auto app = new Engine;
	app->StartUp();

	app->Run();

	app->ShutDown();
	delete app;
}