#pragma once

#include "IController.hpp"
#include "../model/HttpServletRequest.hpp"
#include "../model/HttpServletResponse.hpp"

namespace Http {

/**
 * Interceptors are part of the execution chain.
 * Before/After the controller handles the request the intercept intercept the response/request. 
 */
template <class AcceptableType, class StringT>
class IHandlerInterceptor {
public:
	IHandlerInterceptor() {
	}
	virtual ~IHandlerInterceptor() {
	}

	virtual bool preHandle(HttpServletRequest<StringT>& request, HttpServletResponse<StringT>& response, IController<AcceptableType, StringT>& handler) = 0;

	virtual void postHandle(HttpServletRequest<StringT>& request, HttpServletResponse<StringT>& response, IController<AcceptableType, StringT>& handler, AcceptableType& model) = 0;
};

} /* namespace Http */
