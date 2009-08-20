//
// SMTPClientSession.h
//
// $Id: //poco/1.3/Net/include/Poco/Net/SMTPClientSession.h#4 $
//
// Library: Net
// Package: Mail
// Module:  SMTPClientSession
//
// Definition of the SMTPClientSession class.
//
// Copyright (c) 2005-2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Net_SMTPClientSession_INCLUDED
#define Net_SMTPClientSession_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/DialogSocket.h"
#include "Poco/Timespan.h"


namespace Poco {
namespace Net {


class MailMessage;


class Net_API SMTPClientSession
	/// This class implements an Simple Mail
	/// Transfer Procotol (SMTP, RFC 2821)
	/// client for sending e-mail messages.
{
public:
	enum
	{
		SMTP_PORT = 25
	};

	enum LoginMethod
	{
		AUTH_NONE,
		AUTH_CRAM_MD5,
		AUTH_LOGIN
	};

	explicit SMTPClientSession(const StreamSocket& socket);
		/// Creates the SMTPClientSession using
		/// the given socket, which must be connected
		/// to a SMTP server.

	SMTPClientSession(const std::string& host, Poco::UInt16 port = SMTP_PORT);
		/// Creates the SMTPClientSession using a socket connected
		/// to the given host and port.

	virtual ~SMTPClientSession();
		/// Destroys the SMTPClientSession.

	void setTimeout(const Poco::Timespan& timeout);
		/// Sets the timeout for socket read operations.
		
	Poco::Timespan getTimeout() const;
		/// Returns the timeout for socket read operations.

	void login(const std::string& hostname);
		/// Greets the SMTP server by sending a EHLO command
		/// with the given hostname as argument.
		///
		/// If the server does not understand the EHLO command,
		/// a HELO command is sent instead.
		///
		/// Throws a SMTPException in case of a SMTP-specific error, or a
		/// NetException in case of a general network communication failure.

	void login();
		/// Calls login(hostname) with the current host name.

	void login(LoginMethod loginMethod, const std::string& username, const std::string& password);
		/// Logs in to the SMTP server using the given authentication method and the given
		/// credentials.
		
	void open();
		/// Reads the initial response from the SMTP server.
		///
		/// Usually called implicitly through login(), but can
		/// also be called explicitly to implement different forms
		/// of SMTP authentication.
		///
		/// Does nothing if called more than once.

	void close();
		/// Sends a QUIT command and closes the connection to the server.	
		///
		/// Throws a SMTPException in case of a SMTP-specific error, or a
		/// NetException in case of a general network communication failure.

	void sendMessage(const MailMessage& message);
		/// Sends the given mail message by sending a MAIL FROM command,
		/// a RCPT TO command for every recipient, and a DATA command with
		/// the message headers and content.
		///
		/// Throws a SMTPException in case of a SMTP-specific error, or a
		/// NetException in case of a general network communication failure.

	int sendCommand(const std::string& command, std::string& response);
		/// Sends the given command verbatim to the server
		/// and waits for a response.
		///
		/// Throws a SMTPException in case of a SMTP-specific error, or a
		/// NetException in case of a general network communication failure.

	int sendCommand(const std::string& command, const std::string& arg, std::string& response);
		/// Sends the given command verbatim to the server
		/// and waits for a response.
		///
		/// Throws a SMTPException in case of a SMTP-specific error, or a
		/// NetException in case of a general network communication failure.

protected:
	enum StatusClass
	{
		SMTP_POSITIVE_COMPLETION   = 2,
		SMTP_POSITIVE_INTERMEDIATE = 3,
		SMTP_TRANSIENT_NEGATIVE    = 4,
		SMTP_PERMANENT_NEGATIVE    = 5
	};
	enum
	{
		DEFAULT_TIMEOUT = 30000000 // 30 seconds default timeout for socket operations	
	};

	static bool isPositiveCompletion(int status);
	static bool isPositiveIntermediate(int status);
	static bool isTransientNegative(int status);
	static bool isPermanentNegative(int status);

	void login(const std::string& hostname, std::string& response);
	void loginUsingCRAM_MD5(const std::string& username, const std::string& password);
	void loginUsingLogin(const std::string& username, const std::string& password);
	void loginUsingPlain(const std::string& username, const std::string& password);

private:
	DialogSocket _socket;
	bool         _isOpen;
};


//
// inlines
//
inline bool SMTPClientSession::isPositiveCompletion(int status)
{
	return status/100 == SMTP_POSITIVE_COMPLETION;
}


inline bool SMTPClientSession::isPositiveIntermediate(int status)
{
	return status/100 == SMTP_POSITIVE_INTERMEDIATE;
}


inline bool SMTPClientSession::isTransientNegative(int status)
{
	return status/100 == SMTP_TRANSIENT_NEGATIVE;
}


inline bool SMTPClientSession::isPermanentNegative(int status)
{
	return status/100 == SMTP_PERMANENT_NEGATIVE;
}


} } // namespace Poco::Net


#endif // Net_SMTPClientSession_INCLUDED
