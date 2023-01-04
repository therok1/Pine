#pragma once

#include "Pine/Renderer/Buffer.h"

namespace Pine
{
	class VertexArray
	{
	public:

		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};
}