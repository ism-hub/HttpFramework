#pragma once

#include "../model/HttpServletRequest.hpp"
#include "../model/HttpServletResponse.hpp"

namespace Http {

/**
 * A massage arrives to the dispatcher and the dispatcher asks the chain if it can handle the request.
 * If so the dispatcher can ask the chain to handle the request by calling the 'execute' method.
 */
class IHandlerExecutionChain {
public:
	IHandlerExecutionChain() {}
	virtual ~IHandlerExecutionChain() {}

	/**
	 * If this execution chain can handle the request.
	 * (reduced to if the controller can handle it)
	 */
	virtual bool canHandle(HttpServletRequest& req) = 0;

	/**
	 * Handles the request.
	 * pre: canHandle on the request returned true.
	 */
	virtual std::shared_ptr<HttpServletResponse> execute(HttpServletRequest& req)  = 0;
};

} /* namespace Http */

