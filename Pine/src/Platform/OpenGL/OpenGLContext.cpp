#include "pnpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Pine
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		PN_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		PN_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		PN_CORE_ASSERT(status, "Failed to initialize Glad!");
		
		PN_CORE_INFO("OpenGL Info:\n\tVendor: {0}\n\tRenderer: {1}\n\tVersion: {2}", 
			reinterpret_cast<const char*>(glGetString(GL_VENDOR)), 
			reinterpret_cast<const char*>(glGetString(GL_RENDERER)), 
			reinterpret_cast<const char*>(glGetString(GL_VERSION))
		);

		PN_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Pine requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		PN_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}
