#pragma once

#ifdef PN_PLATFORM_WINDOWS

extern Pine::Application* Pine::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Pine::CreateApplication();
	app->Run();
	delete app;
}

#endif
