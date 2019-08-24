
#pragma once

#include <memory>
#include <vector>
#include "IHandlerExecutionChain.hpp"
#include "../model/HttpServletRequest.hpp"
#include "../model/HttpServletResponse.hpp"


namespace Http {

class DispatcherServlet {
	std::shared_ptr<IHandlerExecutionChain> invalidChain = nullptr;
public:
	std::vector<std::shared_ptr<IHandlerExecutionChain>> _handlerExecutionChains;

	DispatcherServlet(std::vector<std::shared_ptr<IHandlerExecutionChain>> exeChains = {}) : _handlerExecutionChains(exeChains) {
	}

	DispatcherServlet(std::initializer_list<std::shared_ptr<IHandlerExecutionChain>> exeChains) : _handlerExecutionChains(exeChains) {
	}

	 ~DispatcherServlet() {
	}

	std::shared_ptr<IHandlerExecutionChain>& getHandlerExecutionChain(HttpServletRequest& request) {
		for(auto& exeChain : _handlerExecutionChains){
			if(exeChain->canHandle(request))
				return exeChain;
		}
		return invalidChain;
	}

	std::shared_ptr<HttpServletResponse> dispatch(HttpServletRequest& request) {
		std::shared_ptr<IHandlerExecutionChain>& executionChain = getHandlerExecutionChain(request);
		if(executionChain == nullptr) {
			std::shared_ptr<HttpServletResponse> resp = std::make_shared<HttpServletResponse>();
			resp->_httpCode = HttpCode::SC_NOT_FOUND;
			return resp;
		}
		std::shared_ptr<HttpServletResponse> response = executionChain->execute(request);
		return response;
	}

};

} /* namespace Http */
