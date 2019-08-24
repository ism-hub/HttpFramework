#pragma once

//#include <WString.h>
#include <map>
#include <vector>
#include <stdio.h>
#include "HttpEnums.hpp"

#include <string>

//TODO: this whole thing handles strings revisit it. 
namespace Http {


class HttpServletRequest {
public:
	std::string requestBody;
	Http::HTTPMethod httPMethod;
	std::string url;
	std::vector<std::string> urlTokens;

	HttpServletRequest(std::string requestBody, Http::HTTPMethod httPMethod, std::string url) :
			requestBody(requestBody), httPMethod(httPMethod), url(url) {
		addURLTokens(url);
	}

	~HttpServletRequest() {
	}

private:
	void addURLTokens(std::string url) {
		if(url.length() == 0 || url.length() == 1)
			return;
		int nextTokenStartPlace = 1;
		//int indx = url.indexOf('/', nextTokenStartPlace);
		std::string::size_type indx = url.find( '/', nextTokenStartPlace );

		while (indx != std::string::npos) {
			//String tok = url.substring(nextTokenStartPlace, indx);
			std::string tok = url.substr(nextTokenStartPlace, indx - nextTokenStartPlace);
			urlTokens.push_back(tok);
			nextTokenStartPlace = indx + 1;
			indx = url.find('/', nextTokenStartPlace);//if nextTokenStartPlace is invalid indx find will return npos (not found indx)
		}
		if(nextTokenStartPlace < url.length())
			urlTokens.push_back(url.substr(nextTokenStartPlace));
	}
};

} /* namespace Http */
