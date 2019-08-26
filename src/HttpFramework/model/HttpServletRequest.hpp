#pragma once

//#include <WString.h>
#include <map>
#include <vector>
#include <stdio.h>
#include "HttpEnums.hpp"

#include <string>

//TODO: this whole thing handles strings revisit it. 
namespace Http {

/**
 * Represents an HTTP request.
 * StringT - need to have 'char operator[]' and 'length()' 
 * 
 */
template<class StringT>
class HttpServletRequest {
public:

	typedef StringT	string_type;

	class Token {
		const StringT& _source;
		int _startIndx;
		int _endIndx;
	public:
		Token(const StringT& source, int startIndx, int endIndx) 
			: _source(source),  _startIndx(startIndx), _endIndx(endIndx) {}

		int getStartIndx() const {
			return _startIndx;
		}

		int getEndIndex() const {
			return _endIndx;
		}

		const StringT& getSource() const {
			return _source;
		}

		/**
		 * Can compare with different type of strings (operator[] and length() are needed)
		 * There is a special case comparing to const chat*
		 */
		template<class StringT2>
		bool operator==(const StringT2& other) const{
			if(other.length() != _endIndx - _startIndx + 1)
				return false;
			for(int i = _startIndx, io = 0; i <= _endIndx; i++, io++)
				if(_source[i] != other[io])
					return false;
			return true;
		}

		/*template<>
		bool eq(const char* other) {
			int io = 0;
			for(int i = origin._startIndx; i <= origin._endIndx; i++, io++){
				if(other[io] == '\0')//making sure 'other' isn't shorter
					return false;
				if(origin._source[i] != other[io])
					return false;
			}
			if(other[io] != '\0')//making sure 'other' isn't longer
				return false;
			return true;
		}*/

		/**
		 * trying to cast the token to an int 
		 * isValidInt is an output - if true the token was int.
		 */
		int stoi(bool* isValidInt = 0) const {
			int num = 0;
			int minusSign = 1;
			int i = _startIndx;
			if(_source[i] == '-'){
				minusSign = -1;
				i++;
			}

			for(; i <= _endIndx; i++){
				if(_source[i] < '0' || _source[i] > '9'){
					if(isValidInt != 0)
						*isValidInt = false;
					return -1;
				}
				int digit = _source[i] - '0';
				num = num*10 + digit;
			}
			if(isValidInt != 0)
				*isValidInt = true;
			return num*minusSign;
		}
	};


	const StringT& url;
	const StringT& requestBody;
	Http::HTTPMethod httPMethod;
	std::vector<Token> urlTokens;

	/**
	 * Careful not sending rvalue to here..
	 */
	HttpServletRequest(const StringT& requestBody, Http::HTTPMethod httPMethod, const StringT& url) :
			requestBody(requestBody), httPMethod(httPMethod), url(url) {
		addURLTokens(url);
	}

	~HttpServletRequest() {
	}

private:

	/**
	 * the index of the first time ch apppears in str when starting to search from startIndx.
	 * if didnt find return -1.
	 * if  0 > startIndx >= str.length() return -1
	 */
	int findNextChar(const StringT& str, char ch, int startIndx) const{
		if(startIndx < 0)
			return -1;
		int loc = startIndx;
		while(loc < str.length()){
			if(str[loc] == ch)
				return loc;
			loc++;
		}
		return -1;
	}

	void addURLTokens(const StringT& url) {
		if(url.length() == 0 || url.length() == 1)
			return;
		int nextTokenStartPlace = 1;//skipping the first '/'
		int indx = findNextChar( url, '/', nextTokenStartPlace );
		while (indx != -1) {
			urlTokens.emplace_back(url, nextTokenStartPlace, indx - 1);
			nextTokenStartPlace = indx + 1;
			indx = findNextChar( url, '/', nextTokenStartPlace );
		}
		if(nextTokenStartPlace < url.length())
			urlTokens.emplace_back(url, nextTokenStartPlace, url.length() - 1);
	}
};

} /* namespace Http */
