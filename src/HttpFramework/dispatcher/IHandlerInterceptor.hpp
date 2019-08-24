#pragma once

#include "IController.hpp"
#include "../model/HttpServletRequest.hpp"
#include "../model/HttpServletResponse.hpp"

namespace Http {

/**
 * Interceptors are part of the execution chain.
 * Before/After the controller handles the request the intercept intercept the response/request. 
 */
template <class AcceptableType>
class IHandlerInterceptor {
public:
	IHandlerInterceptor() {
	}
	virtual ~IHandlerInterceptor() {
	}

	virtual bool preHandle(HttpServletRequest& request, HttpServletResponse& response, IController<AcceptableType>& handler) = 0;

	virtual void postHandle(HttpServletRequest& request, HttpServletResponse& response, IController<AcceptableType>& handler, AcceptableType& model) = 0;
};

} /* namespace Http */
