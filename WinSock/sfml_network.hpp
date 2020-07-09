#ifndef SFML_NETWORK
#define SFML_NETWORK



#include <iostream>


#define SFML_STATIC
#include <SFML/Network.hpp>

#ifndef NDEBUG
#pragma comment(lib, "winmm")
#pragma comment(lib, "sfml-system-s-d")

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "sfml-network-s-d")
#else
#pragma comment(lib, "winmm")
#pragma comment(lib, "sfml-system-s")

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "sfml-network-s")
#endif



#ifndef MY_SFML_NETWORK_TEST

int test_sfml_network(const std::string& host)
{
	sf::Http http;
	http.setHost(host);

	// fill the request...
	sf::Http::Request request;
	//request.setMethod(sf::Http::Request::Post);
	request.setUri("/index.html");
	request.setHttpVersion(1, 1); // HTTP 1.1
	//request.setField("From", "me");
	//request.setField("Content-Type", "application/x-www-form-urlencoded");
	//request.setBody("para1=value1&param2=value2");

	sf::Http::Response resp = http.sendRequest(request);

	if (resp.getStatus() != resp.InvalidResponse)
	{
		std::cout << "status: " << resp.getStatus() << std::endl;
		std::cout << "HTTP version: " << resp.getMajorHttpVersion() << "." << resp.getMinorHttpVersion() << std::endl;
		std::cout << "Content-Type header:" << resp.getField("Content-Type") << std::endl;
		std::cout << "body: " << resp.getBody() << std::endl;

		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

#endif // !MY_SFML_NETWORK_TEST






#endif // !SFML_NETWORK


