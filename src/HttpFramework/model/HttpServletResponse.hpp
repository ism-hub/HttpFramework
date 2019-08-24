#pragma once

//#include <WString.h>
#include "HttpEnums.hpp"
//#include <WString.h>

//#include <HardwareSerial.h>

namespace Http {

enum class HttpCode {SC_OK=200, SC_NOT_FOUND = 404, SC_ACCEPTED = 202, SC_NOT_IMPL = 501};


class HttpServletResponse {
public:
	static const std::string CONTENT_TYPE_JSON;
	std::string content_type;
	std::string content;
	HttpCode _httpCode;
	HttpServletResponse() :
		content_type(""), content(""), _httpCode(HttpCode::SC_NOT_IMPL) {
	}
	HttpServletResponse(std::string contentType, const char* content, HttpCode httpCode) :
		content_type(contentType), content(content), _httpCode(httpCode) {
	}

	~HttpServletResponse() {
	}
};

const std::string HttpServletResponse::CONTENT_TYPE_JSON = "application/json";

} /* namespace Http */
