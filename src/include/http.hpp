/*
	http.hpp
	MIT License

	Copyright (c) 2017 Charles

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

	This file and the corresponding http.cpp
	were taken from my other project:
	https://github.com/c650/irc-bot-plugins
*/
#ifndef _MY_HTTP_H
#define _MY_HTTP_H

#include <iostream>
#include <curl/curl.h>

namespace MyHTTP {

	/*
		Uses Libcurl to perform a get request to some URL.

		@param url the url to GET with HTTP

		@return the response data.
	*/
	std::string get(const std::string& url);

	/*
		Encodes a string with URI compliance.

		@param unformatted the string to encode.
		@return the encoded string
	*/
	std::string uri_encode(const std::string& unformatted);

	/* CURL Handle struct for RAII*/
	struct CURLHandle {

		CURL* curl_ptr;

		CURLHandle();
		CURLHandle(CURL* _curl_ptr);

		~CURLHandle();

		/*
			Internally calls curl_easy_perform.
			Throws std::logic_error if curl_ptr is nullptr.
		*/
		CURLcode perform(void);

		/*
			Does basic setup stuff.
		*/
		template <typename WriteFunc>
		CURLHandle& do_setup(const std::string& url, WriteFunc func, std::string& response_string)  {
			if (curl_ptr == nullptr)
				curl_ptr = curl_easy_init();

			if (curl_ptr) {
				curl_easy_setopt(curl_ptr, CURLOPT_URL, url.data());
				curl_easy_setopt(curl_ptr, CURLOPT_WRITEFUNCTION, func);
				curl_easy_setopt(curl_ptr, CURLOPT_WRITEDATA, response_string);

				// curl_easy_setopt(curl_ptr, CURLOPT_USERAGENT, "");
				return *this;
			}
			throw std::runtime_error("Failed to initialize the CURL*");
		}
	};
};

#endif
