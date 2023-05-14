#include <capture.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

WSADATA		g_data;
SOCKET		g_sock = INVALID_SOCKET;
SOCKADDR_IN	g_addr;

/**
 * Sends a message along with the last error that
 * WSA (Windows Socket API) had gotten, also cleaning
 * up used memory.
 * 
 * @param	msg: Extra little detail of the current step it halted
 * @param	cleanup: Should it cleanup WSA or not
 * 
 * @return	False
 * */
static bool network_failure(const std::string& msg, const bool cleanup = true)
{
	std::cerr << msg << ": " << WSAGetLastError() << std::endl;
	if (g_sock != INVALID_SOCKET)
		closesocket(g_sock);
	if (cleanup)
		WSACleanup();
	return false;
}

/**
 * Prepares a networking socket to send off data
 * to a given distant server
 * 
 * @param	ip: IP of distant server
 * @param	port: Port of distant server
 * 
 * @return	True or False if socket was successfully created
 * */
bool new_connection(const std::string& ip, const int& port)
{
	if (g_sock != INVALID_SOCKET)
		stop_connection();

	if (WSAStartup(MAKEWORD(2, 2), &g_data) != 0)
		return network_failure("WSA Startup failed", false);

	g_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (g_sock == INVALID_SOCKET)
		return network_failure("Socket creation failed");

	g_addr.sin_family = AF_INET;
	g_addr.sin_port = htons(8080);
	if (inet_pton(AF_INET, "127.0.0.1", &g_addr.sin_addr) <= 0)
		return network_failure("Invalid address provided");

	if (connect(g_sock, (SOCKADDR *) &g_addr, sizeof(g_addr)) == SOCKET_ERROR)
		return network_failure("Couldnt connect to server");
	return true;
}

/**
 * Halts the connection if it does exists, otherwise
 * it does nothing except return True
 * 
 * @return	True
 * */
bool stop_connection()
{
	if (g_sock != INVALID_SOCKET)
		closesocket(g_sock);
	WSACleanup();
	g_sock = INVALID_SOCKET;
	return true;
}

/**
 * Send over data to the distant server
 * 
 * @param	buffer: Buffer to send over
 * @param	buffer_size: Size of the given buffer
 * 
 * @return	True or False if sending was successfull
 * */
bool send_data(const std::string &buffer)
{
	if (g_sock == INVALID_SOCKET)
	{
		std::cerr << "Connection is not initialized!" << std::endl;
		return false;
	}

	if (send(g_sock, buffer.c_str(), (int) buffer.size(), 0) == SOCKET_ERROR)
	{
		std::cerr << "Failed to send data over distant server: " << WSAGetLastError() << std::endl;
		return false;
	}
	return true;
}