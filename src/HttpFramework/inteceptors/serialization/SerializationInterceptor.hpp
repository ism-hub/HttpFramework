#pragma once

#include <memory>
#include "../../model/HttpServletRequest.hpp"
#include "../../model/HttpServletResponse.hpp"
#include "../../dispatcher/IController.hpp"
#include "../../dispatcher/IHandlerInterceptor.hpp"

namespace Http {

/**
 * A generic serialization interceptor that works with a visitor as its engine (delegates all the calls to the visitor)
 */
template <class Visitor, class Acceptable>
class SerializationInterceptor : public IHandlerInterceptor<Acceptable> {
public:
	std::shared_ptr<Visitor>	visitor;
	SerializationInterceptor(std::shared_ptr<Visitor> visitor) :  visitor(visitor) {}
	virtual ~SerializationInterceptor() {}

	bool preHandle(HttpServletRequest& , HttpServletResponse& , IController<Acceptable>&){
		return true;
	}

	void postHandle(HttpServletRequest& , HttpServletResponse& response, IController<Acceptable>&, Acceptable& model) {
		//convert the object into json string
		std::string serialized = std::move(*std::static_pointer_cast<std::string>(model.accept(*visitor)));
		response.content = serialized;
		response.content_type = HttpServletResponse::CONTENT_TYPE_JSON;
		response._httpCode = HttpCode::SC_OK;
	}
};

} /* namespace Http */
