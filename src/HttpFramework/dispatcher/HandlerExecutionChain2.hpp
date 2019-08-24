#pragma once

#include <vector>
#include <memory>
#include "IController.hpp"
#include "IHandlerExecutionChain.hpp"
#include "IHandlerInterceptor.hpp"

namespace Http {

/**
 * Normally an execution chain is composed of an controller and interceptors,
 * The controller contains the user of the framework code (his domain of objects).
 * The interceptors are usually provided by the framework and manages things like serialization of the user domain or checking security token.
 * The interceptors run on the request befor the controller (user code) and after.
 */
template <class AcceptableType>
class HandlerExecutionChain2 : public IHandlerExecutionChain {
protected:
	std::shared_ptr<IController<AcceptableType>> _controller;
	std::vector<std::shared_ptr<IHandlerInterceptor<AcceptableType>>> _handlerInterceptors;

public:
	HandlerExecutionChain2(std::shared_ptr<IController<AcceptableType>> controller) : _controller(controller) {}
	virtual ~HandlerExecutionChain2() {}

	void addInterceptor(std::shared_ptr<IHandlerInterceptor<AcceptableType>> interceptor){
		_handlerInterceptors.push_back(interceptor);
	}

	bool canHandle(HttpServletRequest& req) override {
		return _controller->canHandle(req);
	}

	std::shared_ptr<HttpServletResponse> execute(HttpServletRequest& req) override {
		std::shared_ptr<HttpServletResponse> response = std::make_shared<HttpServletResponse>();
		//HandlerInterceptors before handle (every one return bool if returned true than we need to stop the execution)
		for (unsigned int i = 0; i < _handlerInterceptors.size(); i++) {
			if (!_handlerInterceptors[i]->preHandle(req, *response, *_controller)) // if false then the Interceptor handled the response
				return response;
		}
		// call the controller (the user code)
		std::shared_ptr<AcceptableType> acceptable = _controller->handle(req, *response);
		if (acceptable == nullptr) { //error happened inside the controller, and the controller prepared the right httPresponse with the error properties
			return response;
		}
		//post interceptors 
		for (size_t i = 0; i < _handlerInterceptors.size(); i++) {
			_handlerInterceptors[i]->postHandle(req, *response, *_controller,*acceptable);
		}
		return response;
	}
};

} /* namespace Http */

