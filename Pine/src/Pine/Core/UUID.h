#pragma once

#include <xhash>

namespace Pine
{
	class UUID
	{
	public:

		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }

	private:

		uint64_t m_UUID;

	};
}

namespace std
{
	template<>
	struct hash<Pine::UUID>
	{
		std::size_t operator()(const Pine::UUID& uuid) const
		{
			return hash<uint64_t>()(static_cast<uint64_t>(uuid));
		}
	};
}
