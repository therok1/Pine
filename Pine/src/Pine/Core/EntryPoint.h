#pragma once

#ifdef PN_PLATFORM_WINDOWS

extern Pine::Application* Pine::CreateApplication();

int main(int argc, char** argv)
{
	Pine::Log::Init();

	PN_PROFILE_BEGIN_SESSION("Startup", "PineProfile-Startup.json");
	auto app = Pine::CreateApplication();
	PN_PROFILE_END_SESSION();

	PN_PROFILE_BEGIN_SESSION("Runtime", "PineProfile-Runtime.json");
	app->Run();
	PN_PROFILE_END_SESSION();

	PN_PROFILE_BEGIN_SESSION("Shutdown", "PineProfile-Shutdown.json");
	delete app;
	PN_PROFILE_END_SESSION();
}

#endif
