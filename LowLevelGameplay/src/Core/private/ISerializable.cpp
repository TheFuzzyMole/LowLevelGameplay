#include <Core/ISerializable.h>

namespace YAML
{
	Node convert<LLGP::Vector2f>::encode(const LLGP::Vector2f rhs)
	{
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.SetStyle(EmitterStyle::Flow);
		return node;
	}

	bool convert<LLGP::Vector2f>::decode(const Node node, LLGP::Vector2f& rhs)
	{
		if (!node.IsSequence() || node.size() != 2)
			return false;

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		return true;
	}

	Node convert<LLGP::Vector2u>::encode(const LLGP::Vector2u rhs)
	{
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.SetStyle(EmitterStyle::Flow);
		return node;
	}

	bool convert<LLGP::Vector2u>::decode(const Node node, LLGP::Vector2u& rhs)
	{
		// Note: Is there no way to get type from type of types?
		if (!node.IsSequence() || node.size() != 2)
			return false;

		rhs.x = node[0].as<unsigned int>();
		rhs.y = node[1].as<unsigned int>();
		return true;
	}
}

namespace LLGP
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const LLGP::Vector2f& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const LLGP::Vector2u& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
}
