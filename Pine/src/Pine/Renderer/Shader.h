#pragma once

#include <string>

namespace Pine
{
	class Shader
	{
	public:

		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Bind() const;
		void Unind() const;

	private:

		uint32_t m_RendererID;
	};
}