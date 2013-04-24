#define WIN32_LEAN_AND_MEAN

#include "StringUtil.h"
#include "mail.h"
#include <cstdio>
#include <windows.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define CRLF "\r\n"                 // carriage-return/line feed pair

// Basic error checking for send() and recv() functions
void check (int iStatus, char const * szFunction)
{
	if (iStatus != SOCKET_ERROR && iStatus)
		return;
	throw std::string("Error during call to ") + szFunction + ": " + toString(iStatus) + " - " + toString((int)GetLastError()) + ". ";
}

void Mail::send (std::string const & text, std::string const & fromAddress, std::string const & toAddress, std::string const & smtpServerAddress)
{
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
		throw "Error opening Winsock 1.1. ";

	// Lookup email server's IP address.
	struct hostent * pHostEnt = gethostbyname(smtpServerAddress.c_str());
	if (pHostEnt == 0)
		throw "Could not resolve SMTP server IP address. ";

	// Create a TCP/IP socket, no specific protocol
	SOCKET hServer = socket(PF_INET, SOCK_STREAM, 0);
	if(hServer == INVALID_SOCKET)
		throw "Could not open a socket. ";

	// Get the mail service port
	struct servent * pServEnt = getservbyname("mail", 0);

	// Use the SMTP default port if no other port is specified
	u_short iProtocolPort;
	if (pServEnt == 0)
		iProtocolPort = htons(IPPORT_SMTP);
	else
		iProtocolPort = pServEnt->s_port;
	iProtocolPort = 465;

	// Setup a Socket Address structure
	SOCKADDR_IN socketAddress;
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port   = iProtocolPort;
	socketAddress.sin_addr   = *((LPIN_ADDR)*pHostEnt->h_addr_list);

	// Connect the Socket
	if (connect(hServer, (PSOCKADDR)&socketAddress, sizeof(socketAddress)) != 0)
		throw "Could not connect to SMTP server. ";

	// Receive initial response from SMTP server
	char szBuffer[4096];
	char szMsgLine[255];
	check(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() Reply");

	// Send HELO server.com
	sprintf(szMsgLine, "HELO %s%s", smtpServerAddress.c_str(), CRLF);
	check(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() HELO");
	check(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() HELO");

	// Send MAIL FROM: <sender@mydomain.com>
	sprintf(szMsgLine, "MAIL FROM:<%s>%s", fromAddress.c_str(), CRLF);
	check(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() MAIL FROM");
	check(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() MAIL FROM");

	// Send RCPT TO: <receiver@domain.com>
	sprintf(szMsgLine, "RCPT TO:<%s>%s", toAddress.c_str(), CRLF);
	check(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() RCPT TO");
	check(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() RCPT TO");

	// Send DATA
	sprintf(szMsgLine, "DATA%s", CRLF);
	check(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() DATA");
	check(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() DATA");

	check(send(hServer, text.c_str(), strlen(text.c_str()), 0), "send() message-line");

	// Send blank line and a period
	sprintf(szMsgLine, "%s.%s", CRLF, CRLF);
	check(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() end-message");
	check(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() end-message");

	// Send QUIT
	sprintf(szMsgLine, "QUIT%s", CRLF);
	check(send(hServer, szMsgLine, strlen(szMsgLine), 0), "send() QUIT");
	check(recv(hServer, szBuffer, sizeof(szBuffer), 0), "recv() QUIT");

	// Close server socket and prepare to exit.
	closesocket(hServer);

	WSACleanup();
}

