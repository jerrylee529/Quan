#if !defined(HttpSocketHandler_H)
#define HttpSocketHandler_H


#include "HttpServerInfo.h"
#include "HttpRequestProcess.h"
#include "SocketHandler.h"
#include "TransferStatus.h"


////////////////////////////////////////////////////////////////////////////////////
// HttpSocketHandler
// 
// Purpose:		handler for http socket. socket handler creates the socket and
//				the handler notifies us of socket events


class HttpSocketHandler : 
	public SocketHandler
{
	TransferStatus		_status;			// controls transfer
	HttpServerInfo &	_server;			// ref to server info
	HttpRequestProcess	_processReq;		// used to process requests
	bool				_keepAlive;			// shows if connection left open after
											// request processed

public:

	HttpSocketHandler ( HttpServerInfo & server ) : 
		
		SocketHandler(),

		_status(),
		_server( server ),
		_processReq(),
		_keepAlive(false)

	{}

	virtual ~HttpSocketHandler ()
	{
		SocketHandler::release();
	}

	// socket event handlers
	bool onClose ( int error )
	{
		// close the socket
		Socket & sock = getSocket();
		sock.close();

		// remove handler
		delete this;

		return true;
	}

	bool onSend ( int error )
	{
		// get socket
		Socket & sock = getSocket();

		// get buffer to send
		LPTSTR buffer = _processReq.data();
		long   length = _processReq.size();

		// send response
		long noSent = sock.send( buffer, length );

		// update transfer status
		_status.incNoSent( noSent );

		// if done and dont keep open close socket
		// this will delete the handler 
		if ( _status.finished() && !_keepAlive )
		{
			sock.close();
		}

		return true;
	}

	bool onReceive ( int error )
	{
		// get socket
		Socket & sock = getSocket();

		// reset transfer status
		_status.reset();

		// get no of bytes
		long length = sock.getRecieveSize();
		if ( length == 0 )
		{
			return false;
		}


		// recieve data
		char buffer[1024];

		// terminate buffer
		buffer[length] = '\0';

		// recieve 
		length = sock.recieve( buffer, 1024 );
		if ( length == 0 )
		{
			return false;
		}

		printf("%s",buffer);

		// process request
		if ( _processReq.process( buffer, length ) )
		{
			// setup transfer info
			long noToSend = _processReq.size();
			_status.setSizeToSend( noToSend );

			//string body = _processReq.getBody();

		}

		return true;
	}



	// run method
	//long run ();
};


#endif