
#pragma once

#include <memory>
#include <vector>
#include "IHandlerExecutionChain.hpp"
#include "../model/HttpServletRequest.hpp"
#include "../model/HttpServletResponse.hpp"


namespace Http {

template<class StringT>
class DispatcherServlet {
	std::shared_ptr<IHandlerExecutionChain<StringT>> invalidChain = nullptr;
public:
	std::vector<std::shared_ptr<IHandlerExecutionChain<StringT>>> _handlerExecutionChains;

	DispatcherServlet(std::vector<std::shared_ptr<IHandlerExecutionChain<StringT>>> exeChains = {}) : _handlerExecutionChains(exeChains) {
	}

	DispatcherServlet(std::initializer_list<std::shared_ptr<IHandlerExecutionChain<StringT>>> exeChains) : _handlerExecutionChains(exeChains) {
	}

	 ~DispatcherServlet() {
	}

	std::shared_ptr<IHandlerExecutionChain<StringT>>& getHandlerExecutionChain(HttpServletRequest<StringT>& request) {
		for(auto& exeChain : _handlerExecutionChains){
			if(exeChain->canHandle(request))
				return exeChain;
		}
		return invalidChain;
	}

	std::shared_ptr<HttpServletResponse<StringT>> dispatch(HttpServletRequest<StringT>& request) {
		std::shared_ptr<IHandlerExecutionChain<StringT>>& executionChain = getHandlerExecutionChain(request);
		if(executionChain == nullptr) {
			std::shared_ptr<HttpServletResponse<StringT>> resp = std::make_shared<HttpServletResponse<StringT>>();
			resp->_httpCode = HttpCode::SC_NOT_FOUND;
			return resp;
		}
		std::shared_ptr<HttpServletResponse<StringT>> response = executionChain->execute(request);
		return response;
	}

};

} /* namespace Http */
