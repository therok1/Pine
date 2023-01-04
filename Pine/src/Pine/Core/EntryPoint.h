#pragma once

#ifdef PN_PLATFORM_WINDOWS

extern Pine::Application* Pine::CreateApplication();

int main(int argc, char** argv)
{
	Pine::Log::Init();
	PN_CORE_TRACE("Initialized log!");
	PN_INFO("Initialized log!");

	auto app = Pine::CreateApplication();
	app->Run();
	delete app;
}

#endif
