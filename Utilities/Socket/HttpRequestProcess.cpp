
#include "stdafx.h"
#include "HttpRequestProcess.h"


//////////////////////////////////////////////////////////////////////////////
// HttpRequestProcess


HttpRequestProcess::HttpRequestProcess ( LPTSTR buffer, long length ) :

	_resp(),
	_processState(REQ_REQUEST),
	_buffer(""),
	_length(0)

{
	create(buffer,length);	
}

HttpRequestProcess::HttpRequestProcess () :

	_resp(),
	_processState(REQ_REQUEST),
	_buffer(""),
	_length(0)

{}

HttpRequestProcess::~HttpRequestProcess ()
{
	release();
}

bool HttpRequestProcess::create ( LPTSTR buffer, long length )
{
	// set state
	_processState = REQ_REQUEST;
	_buffer		  = buffer;
	_length		  = length;

	return true;
}

void HttpRequestProcess::release ()
{
	// release request and response
	_request.release();
	_resp.release();

	// reset state
	_processState = REQ_REQUEST;
	_buffer.erase();
	_length = 0;
	_line.erase();
}

bool HttpRequestProcess::process ( LPTSTR buffer, long length )
{
	// if create fails stop
	if ( !create(buffer,length) )
		return false;

	// process request
	return process();
}

bool HttpRequestProcess::process ()
{

	// start at first of buffer
	int index = 0;

	// set process state
	_processState = REQ_REQUEST;

	switch ( _processState )
	{
		case REQ_REQUEST:
		case REQ_HEADER:
		{
			while( getLine( index ) == true )
			{
				if ( !_line.empty() )
					processLine();
				else
				{
					_processState = REQ_BODY;
					break;
				}
			}

			// #DGH
			// need to check logic. if request is
			// finished getLine returns false
			// then process state is not set
			// to REQ_BODY in above

			// break if we're not looking for the body
			//if ( _processState != REQ_BODY )
			//	break;

			// stop if no body sent
			// if no content-length then body not
			// sent
			if ( !bodySent() )
			{
				_processState = REQ_DONE;
				break;
			}
		}

		// else fall through
		case REQ_BODY:
		{
			addToBody( _length, index );
			break;
		}
	}

	// if process request
	if ( _processState == REQ_DONE )
	{
		// get request information
		_resp.setState(_request);

		// process it
		_resp.startResponse();

		return true;

		//if ( !resp.startResponse(*this) )
		//	AsyncSelect( FD_WRITE | FD_CLOSE );
	}

	return false;
}


bool HttpRequestProcess::getLine ( int & index )
{
	// show did not find line
	bool foundLine = false;

	// reset line
	_line = "";

	// continue til find line end
	// might need to do string search
	while ( foundLine == false && index < _length )
	{
		char ch = _buffer[index];
		switch( ch )
		{
			// ignore
			case '\r': 
				break;

			// end-of-line
			case '\n': 
				foundLine = true;
				break;

			// get char
			default:   // other
				_line += ch;
				break;
		}
		++index;
	}
	return foundLine;
}


string decode ( const string & str, bool query = false )
{
	int index;
	string strDecoded = str;

	// special processing or query strings
	if ( query )
	{
		// change all '+' to ' '
		while( (index = strDecoded.find('+')) != -1 )
			strDecoded = StringUtil::left(strDecoded, index) + ' ' + StringUtil::mid(strDecoded, index+1);
	}

	// first see if there are any %s to decode
	if ( strDecoded.find( '%' ) != -1 )
	{
		// iterate through the string, changing %dd to special char
		for( index=0; index < strDecoded.size(); index++ )
		{
			char ch = strDecoded[index];
			if ( ch == '%' )
			{
				if ( strDecoded[index+1] == '%' )
				{
					// keep one percent sign
					strDecoded = StringUtil::left(strDecoded,index) + StringUtil::mid(strDecoded,index+1);
				}
				else
				{
					// assume we have a hex value
					char ch1 = strDecoded[index+1];
					char ch2 = strDecoded[index+2];
					ch1 = ch1 >= 'A' ? (ch1&0xdf)-'A' : ch1-'0';
					ch2 = ch2 >= 'A' ? (ch2&0xdf)-'A' : ch2-'0';

					// replace the escape sequence with the char
					strDecoded = StringUtil::left(strDecoded,index)
						+ (char)(ch1*16 + ch2)
						+ StringUtil::mid(strDecoded,index+3);
				}
			}
		}
	}

	return strDecoded;
}



void HttpRequestProcess::processLine ()
{
	int index;
	switch( _processState )
	{
		case REQ_REQUEST:
		{
			index = _line.find( ' ' );
			if ( index != -1 )
			{
				// get method
				_request.setMethod( StringUtil::left( _line, index ) );

				// remove processed portion and cleanup
				_line.erase( 0, index );

				StringUtil::trimLeft( _line );

				index = _line.find( ' ' );
				if ( index == -1 )
				{
					_request.setUrl( _line );
					StringUtil::trimRight( _request.getUrl() );

					_processState = REQ_SIMPLE;
				}
				else
				{
					_request.setUrl( StringUtil::left( _line, index ) );
					_request.setVersion( StringUtil::mid( _line, index+1 ) );

					StringUtil::trimLeft( _request.getVersion() );
				}

				// check for execution arguments
				index = _request.getUrl().find( '?' );

				if ( index != -1 )
				{
					// save the args
					_request.setArgs( StringUtil::mid( _request.getUrl(), index+1 ) );


					// strip from file name
					_request.setUrl( StringUtil::left( _request.getUrl(), index ) );


					//request.m_dwExecute = idIsapiExecute;
				}

				// change any "%xx"s to the appropriate char
				_request.setUrl( decode( _request.getUrl() ) );
			}
			_processState = REQ_HEADER;
			break;
		}

		case REQ_HEADER:
		{
			index = _line.find( ':' );
			if ( index != -1 )
			{
				string strName  = StringUtil::left( _line, index );
				string strValue = StringUtil::mid( _line, index+1 );


				StringUtil::makeLower( strName );
				StringUtil::trimLeft( strValue );

				// add param value pair
				_request.addHeaderValue( strName, strValue );
			}

			break;
		}
	}

	// clear line
	_line.erase();
}




bool HttpRequestProcess::bodySent ()
{
	bool bSent = false;

	// get content length
	string strValue = _request.getHeaderValue( idParamContentLength );

	// if found set size
	if ( !strValue.empty() )
	{
		long length = atoi( strValue.c_str() );
		_request.setLength( length );
		bSent = true;
	}

	return bSent;
}

void HttpRequestProcess::addToBody( int nBytes, int index )
{
	// #DGH
	// come back to 
	
	/*
	// save the buffer size
	int oldSize = _buffer.size();

	// get rid of old stuff, append to body data
	_buffer.erase( 0, index );
	_buffer.resize( nBytes - index );

	// add to request body
	_body += _buffer;

	// restore the buffer size
	_buffer.resize( oldSize );

	// see if we're done
	if ( _body.size() >= getLength() )
	{
		_body.resize( getLength() );
		_processState = REQ_DONE;
	}

	*/
}


