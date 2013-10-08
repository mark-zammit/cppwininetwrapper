#ifndef _FTP_H_
#define _FTP_H_

/*
Coder	: Tokivena -- Elie Louis
Contact : elie_louis@hotmail.com
*/

/*
Contributor : Mark Zammit
Contact     : markzammit@me.com
*/

#include <Windows.h>
#include <WinInet.h>
#include <tchar.h>
#include <iostream>
#include <string>
#pragma comment (lib, "wininet.lib")

/* UNICODE SUPPORT */
#if !defined(TSTR)
	#if defined(UNICODE) || defined(_UNICODE_)
		#define TSTR	std::wstring
	#else
		#define	TSTR    std::string
	#endif
#endif


#define INVALID_FILE    -1

/** NOTE: Use GetLastError() to examine the @false result from any method */

class FTP
{
	public:
	    /** Creates an internet connection and sets up local variables */
		FTP(void);
		/** Disconnects session on scope end if not already disconnected */
		~FTP(void);

		/// CONNECTION METHODS ///
		/** bool connect(TSTR, TSTR, TSTR)
		 *  Connects to FTP server.
		 *      @lpszServerName : Fully qualified domain name or IP Address
		 *      @lpszUser       : User id
		 *      @lpszPassword   : User password
		 *  Returns : @true on connect of @false on failure
		 */
		 /** bool disconect(void)
		 *  Disconnects from active FTP server.
		 *
		 *  Returns : @true on successful disconnect or @false if already disconnected
		 */
		bool connect(TSTR lpszServerName, TSTR lpszUser, TSTR lpszPassword);
		bool disconnect(void);

		/// GET/PUSH METHODS ///
		/** Uploads a file from client location to server location.
		 *      @lpszLocation   : Client relative file location
		 *      @lpszRemFile    : File name to be saved on server, leave blank for original name
		 *  Returns : @true on successful upload or @false on failed upload, could be
		 *              because file already exists on server or because there is no active connection.
		 */
		 /** Downloads a file from server location to client location.
		 *      @lpszLocation    : Server relative file loaction
		 *      @lpszRemName     : File name to be saved on client, leave blank for original name
		 *  Returns : @true on successful upload or @false on failed upload, could be
		 *              because file already exists on client or because there is no active connection.
		 */
		bool upload(TSTR lpszLocation, TSTR lpszRemFile);
		bool download(TSTR lpszLocation, TSTR lpszRemName);

		/// DIRECTORY METHODS ///
		/** bool cd(TSTR)
		 *  Change Directory to either a fully qualified path or relative path.
		 *      @lpszDirectory  : Directory on server to change to
		 *  Returns : @true on succcessful change or @false if no directory exists or
		 *              if there is no active connection.
		 */
		 /** bool mkdir(TSTR)
		 *  Creates Directory for either a fully qualified path or relative path.
		 *      @lpszDirectory  : Directory on server to create
		 *  Returns : @true on succcessful directory creation or @false if create failed or
		 *              if there is no active connection.
		 */
		 /** bool rmdir(TSTR)
		 *  Removes Directory for either a fully qualified path or relative path.
		 *      @lpszDirectory  : Directory on server to remove
		 *  Returns : @true on succcessful removal or @false if directory doesn't exist or
		 *              if there is no active connection.
		 */
		 /** TSTR getcd(void)
		 *  Retrieves current directory path
		 *
		 *  Returns : currently pointed to server directory as a TSTR for unicode support
		 */
		bool cd(TSTR lpszDirectory);
		bool mkdir(TSTR lpszDirectory);
		bool rmdir(TSTR lpszDirectory);
		TSTR getcd(void);

        /// FILE HANDLING METHODS ///
        /** bool remove(TSTR)
         *  Removes a file from FTP server.
         *      @lpszFileName   : Name of file to be deleted on server.
         *  Returns : @true of file was successfully removed or @false if no file exists or
         *              if there is no active connection.
         */
         /** bool rename(TSTR, TSTR)
         *  Renames a file on FTP server.
         *      @lpszFileName   : Name of file to be renamed on server.
         *  Returns : @true of file was successfully renamed or @false if no file exists or
         *              if there is no active connection.
         */
         /** LONGLONG getfilesize(TSTR)
         *  File size of existing server file.
         *      @lpszFileName   : Name of file to retrieve size of on server.
         *  Returns : File size as a long long or @INVALID_FILE if file does not exist or
         *              if there is no active connection.
         */
		bool     remove(TSTR lpszFileName);
		bool     rename(TSTR lpszOldFileName, TSTR lpszNewFileName);
		LONGLONG getfilesize(TSTR lpszFileName);

        /// MISCELLANEOUS METHODS ///
        /** HINTERNET gethandle(void)
         *  Gets handle to an open FTP Session for cross-API use
         *
         *  Returns : HINTERNET handle to FTP Sessions or NULL if there is no connection
         *              NB: NULL is returned so that deallocated (bad handles) aren't returned.
         */
         /** bool command(TSTR)
         *  Executes a user defined FTP Command
         *
         *  Returns : @true if command was successfully exeucted or @false if not or
         *              if there is no active connection.
         */
		HINTERNET   gethandle(void);
		bool        command(TSTR lpszCommand);

	private:
		bool        m_connected;
		HINTERNET   m_hInternet;
		HINTERNET   m_hFtpSession;
};



#endif
