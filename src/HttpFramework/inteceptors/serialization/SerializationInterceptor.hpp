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
template <class Visitor, class Acceptable, class StringT>
class SerializationInterceptor : public IHandlerInterceptor<Acceptable, StringT> {
public:
	std::shared_ptr<Visitor>	visitor;
	SerializationInterceptor(std::shared_ptr<Visitor> visitor) :  visitor(visitor) {}
	virtual ~SerializationInterceptor() {}

	bool preHandle(HttpServletRequest<StringT>& , HttpServletResponse<StringT>& , IController<Acceptable, StringT>&){
		return true;
	}

	void postHandle(HttpServletRequest<StringT>& , HttpServletResponse<StringT>& response, IController<Acceptable, StringT>&, Acceptable& model) {
		//convert the object into json string
		StringT serialized = std::move(*std::static_pointer_cast<StringT>(model.accept(*visitor)));
		response.content = serialized;
		response.content_type = StringT(HTTP_CONTENT_TYPE_JSON);//TODO: try to optimize
		response._httpCode = HttpCode::SC_OK;
	}
};

} /* namespace Http */
