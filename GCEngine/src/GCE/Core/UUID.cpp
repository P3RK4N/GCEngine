#include "GCEPCH.h"
#include "GCE/Core/UUID.h"

#include <random>
#include <unordered_map>

static std::unordered_map<uint64_t, std::string> m_Map;

static void addToMap()
{
	m_Map[GCE::UUID()] = "Perkan";
}

namespace GCE
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformIntDistribution;

	UUID::UUID()
		: m_UUID(s_UniformIntDistribution(s_Engine))
	{
		
	}

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{
		
	}

}

namespace std
{
	template<>
	struct hash<GCE::UUID>
	{
		std::size_t operator()(const GCE::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}