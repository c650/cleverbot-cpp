/*
	http.cpp

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

	This file and the corresponding http.hpp
	were taken from my other project:
	https://github.com/c650/irc-bot-plugins
*/
#include <iostream>
#include <curl/curl.h>
#include <cstring>
#include <stdexcept>

#include "./include/http.hpp"

namespace MyHTTP {

/*
	HELPER
	Prototypes.
*/
	/* to store result of GET/POST in stream */
	static size_t write_to_string(void *ptr, size_t size, size_t nmemb, std::string& stream);

/* ------------ */

	/* see .hpp for docs */
	std::string get(const std::string& url) {

		#ifdef DEBUG
			std::cout << "MyHTTP::get( " << url << " )\n";
		#endif

		std::string response;

		CURLHandle curl_handle;
		CURLcode res = curl_handle.do_setup(url, write_to_string, response).perform();
		if (res != CURLE_OK)
			throw std::runtime_error("HTTP GET of " + url + " failed with CURLcode: " + std::to_string(res));

		return response;
	}

	std::string uri_encode(const std::string& unformatted) {

		std::string result = "";

		CURL *curl = curl_easy_init();
		if (curl) {
			char *unformatted_raw = strdup(unformatted.data());

			char *escaped = curl_easy_escape(curl, unformatted_raw, strlen(unformatted_raw));

			result = escaped;

			curl_free(escaped);

			free(unformatted_raw);
			curl_easy_cleanup(curl);
		}

		return result;
	}

/* -----------------*/
	/* HELPERS */

	static size_t write_to_string(void *ptr, size_t size, size_t nmemb, std::string& stream) {
		size_t realsize = size * nmemb;
		std::string temp(static_cast<const char*>(ptr), realsize);
		stream.append(temp);
		return realsize;
	}

	/* CURLHandle Implementation */

	CURLHandle::CURLHandle() : CURLHandle(nullptr) {}
	CURLHandle::CURLHandle(CURL* _curl_ptr) : curl_ptr(_curl_ptr) {}

	CURLHandle::~CURLHandle() {
		if (this->curl_ptr != nullptr)
			curl_easy_cleanup(this->curl_ptr);
	}

	CURLcode CURLHandle::perform(void) {
		if (this->curl_ptr == nullptr)
			throw std::logic_error("Cannot perform the CURL action on a nullptr!");
		return curl_easy_perform(this->curl_ptr);
	}

};
