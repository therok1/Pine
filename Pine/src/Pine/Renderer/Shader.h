#pragma once

#include <glm/glm.hpp>

namespace Pine
{
	class Shader
	{
	public:

		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Bind() const;
		void Unind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:

		uint32_t m_RendererID;
	};
}