#pragma once
#include <Core/Vector2.h>
#include <yaml-cpp/yaml.h>
#include <functional>

namespace YAML
{
	template <>
	struct convert<LLGP::Vector2f>
	{
		static Node encode(const LLGP::Vector2f rhs);
		
		static bool decode(const Node node, LLGP::Vector2f& rhs);
		
	};

	template <>
	struct convert<LLGP::Vector2u>
	{
		static Node encode(const LLGP::Vector2u rhs);
		
		static bool decode(const Node node, LLGP::Vector2u& rhs);
		
	};
}

namespace LLGP
{
	class Component;
	extern YAML::Emitter& operator<<(YAML::Emitter& out, const LLGP::Vector2f& v);
	
	extern YAML::Emitter& operator<<(YAML::Emitter& out, const LLGP::Vector2u& v);
	
	struct LinkRequest
	{
	public:
		LinkRequest(std::function<void(LLGP::Component*)> _delegate, uint64_t _token)
			: linkDelegate(_delegate), linkToken(_token) {}
		std::function<void(LLGP::Component*)> linkDelegate;
		uint64_t linkToken;
	};

	class ISerializable
	{
	public:
		virtual void Serialize(YAML::Emitter& out) = 0;
		virtual bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) = 0;
	};
}
