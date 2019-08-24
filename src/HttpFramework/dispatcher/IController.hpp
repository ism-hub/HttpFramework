#pragma once

#include "../model/HttpServletRequest.hpp"
#include "../model/HttpServletResponse.hpp"

namespace Http {

template <class AcceptableType>
class IController {
public:

	IController(){
	}
	virtual ~IController(){
	}

	virtual bool canHandle(HttpServletRequest& req)=0;
	virtual std::shared_ptr<AcceptableType> handle(HttpServletRequest& req, HttpServletResponse& response)=0;
};

} /* namespace Http */
