#pragma once

#include <string>

// Uses Win32 Sockets.

class Mail
{
public:
	void send (std::string const & text, std::string const & fromAddress, std::string const & toAddress, std::string const & smtpServerAddress);
};

