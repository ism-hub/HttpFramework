#pragma once

#include <memory>

namespace Http {

/**
 * A generic serialization visitor.
 * Input: An acceptable object.
 * Output: Serialization string.
 */
template <class Serializer, class StringT>
class SerializationVisitor {
	std::shared_ptr<Serializer> serializer = nullptr;
public:
	SerializationVisitor(std::shared_ptr<Serializer> serializer) : serializer(serializer) {}
	virtual ~SerializationVisitor() {}

	template <class Model>
	std::shared_ptr<void> visit(Model& model) {
		std::shared_ptr<StringT> serialized = std::make_shared<StringT>();
		//serialized->reserve(2500);
		//if(!serialized->reserve(2500))//TODO:
		//	return nullptr;
			//Serial.println(F("___ERROR: failed to reserve 2500 bytes for the SerializationVisitor string"));
		serializer->serialize(model, *serialized);
		return serialized;
	}
};

} /* namespace Http */