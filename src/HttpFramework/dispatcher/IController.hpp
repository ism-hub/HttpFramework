#pragma once

#include "../model/HttpServletRequest.hpp"
#include "../model/HttpServletResponse.hpp"

namespace Http {

template <class AcceptableType, class StringT>
class IController {
public:

	IController(){
	}
	virtual ~IController(){
	}

	virtual bool canHandle(HttpServletRequest<StringT>& req)=0;
	virtual std::shared_ptr<AcceptableType> handle(HttpServletRequest<StringT>& req, HttpServletResponse<StringT>& response)=0;
};

} /* namespace Http */
