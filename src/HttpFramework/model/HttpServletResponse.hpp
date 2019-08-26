#pragma once

//#include <WString.h>
#include "HttpEnums.hpp"
//#include <WString.h>

//#include <HardwareSerial.h>

namespace Http {

template<class StringT>
class HttpServletResponse {
public:
	StringT content_type;
	StringT content;
	HttpCode _httpCode;
	HttpServletResponse() :
		 _httpCode(HttpCode::SC_NOT_IMPL) {
	}
	HttpServletResponse(StringT contentType, StringT content, HttpCode httpCode) :
		content_type(std::move(contentType)), content(std::move(content)), _httpCode(httpCode) {
	}

	~HttpServletResponse() {
	}
};


//const const char* HttpServletResponse::CONTENT_TYPE_JSON = "application/json";

} /* namespace Http */
