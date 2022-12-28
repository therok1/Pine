#include <Pine.h>

class Sandbox : public Pine::Application
{
public:

	Sandbox()
	{

	}

	~Sandbox()
	{

	}

};

Pine::Application* Pine::CreateApplication()
{
	return new Sandbox();
}