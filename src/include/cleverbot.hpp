/*
	cleverbot.hpp
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
*/
#include "./http.hpp"
#include "./json.hpp"

#include <string>
#include <sstream>

#include <stdexcept>

#define WRAPPER_NAME "cleverbot-cpp"
#define API_ROUTE    "https://www.cleverbot.com/getreply?"

class CleverbotInstance {



	std::string api_key;

	std::string cleverbot_state;

public:

	/*
		Constructor.

		@param _api_key the user's API Key.
		@param cs, an optional cleverbot state for picking up an old conversation.
	*/
	CleverbotInstance(const std::string& _api_key, const std::string& cs = "") : api_key(_api_key), cleverbot_state(cs) {}

	CleverbotInstance(CleverbotInstance&) = delete;
	CleverbotInstance& operator=(CleverbotInstance&) = delete;

	CleverbotInstance(CleverbotInstance&&) = default;
	CleverbotInstance& operator=(CleverbotInstance&&) = default;

	/*
		Interacts with Cleverbot.

		@param input the message to send to Cleverbot.
			Defaults to "".
		@return what Cleverbot responds to your input.
	*/
	std::string interact(const std::string& input = "") {
		std::stringstream link;
		link << API_ROUTE << "key=" << api_key;

		if (!input.empty())
			link << "&input=" << MyHTTP::uri_encode(input);
		if (!cleverbot_state.empty())
			link << "&cs=" << cleverbot_state;

		link << "&wrapper=" << WRAPPER_NAME;

		using json = nlohmann::json;

		try {
			json response = json::parse(MyHTTP::get(link.str()));

			if (cleverbot_state.empty())
				cleverbot_state = response["cs"].get<std::string>();

			return response["output"].get<std::string>();

		} catch (std::exception& e) {
			std::stringstream ss;
			ss << "Error in CleverbotInstance::interact: " << e.what();
			throw std::runtime_error(ss.str());
		}
	}

	void set_cleverbot_state(const std::string& cs) {
		cleverbot_state = cs;
	}

	std::string get_cleverbot_state(void) const {
		return cleverbot_state;
	}

};
