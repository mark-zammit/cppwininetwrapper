#include "ftp.h"

FTP::FTP()
{
	m_hInternet = InternetOpen(NULL,
                               INTERNET_OPEN_TYPE_DIRECT,
                               NULL,
                               NULL,
                               0);
	m_connected = false;
}

FTP::~FTP()
{
    if(m_connected)
    {
        InternetCloseHandle(m_hFtpSession);
        InternetCloseHandle(m_hInternet);
    }
}


bool FTP::connect(TSTR lpszServerName, TSTR lpszUser, TSTR lpszPassword)
{
	m_hFtpSession = InternetConnect(m_hInternet,
                                    (LPTSTR)lpszServerName.c_str(),
                                    INTERNET_DEFAULT_FTP_PORT,
                                    (LPTSTR)lpszUser.c_str(),
                                    (LPTSTR)lpszPassword.c_str(),
                                    INTERNET_SERVICE_FTP,
                                    0,
                                    0);
	if (!m_hFtpSession)
        return false;

	return (m_connected = true);
}

bool FTP::disconnect()
{
    if(m_connected)
    {
        InternetCloseHandle(m_hFtpSession);
        InternetCloseHandle(m_hInternet);
        m_connected = false;
    }

    return !m_connected;
}

bool FTP::upload(TSTR lpszLocation, TSTR lpszRemFile)
{
	return !m_connected ? false : FtpPutFile(m_hFtpSession,
                                             (LPTSTR)lpszLocation.c_str(),
                                             lpszRemFile.empty() ?
                                                (LPTSTR)lpszLocation.c_str()
                                                : (LPTSTR)lpszRemFile.c_str(),
                                             FTP_TRANSFER_TYPE_BINARY,
                                             0) == TRUE;
}

bool FTP::download(TSTR lpszLocation, TSTR lpszRemFile)
{
	return !m_connected ?
        false
        : FtpGetFile(m_hFtpSession,
                     (LPTSTR)lpszLocation.c_str(),
                     lpszRemFile.empty() ?
                        (LPTSTR)lpszLocation.c_str()
                        : (LPTSTR)lpszRemFile.c_str(),
                     0,
                     0,
                     FTP_TRANSFER_TYPE_BINARY,
                     0) == TRUE;
}

bool FTP::mkdir(TSTR lpszDirectory)
{
	return !m_connected ?
        false
        : FtpCreateDirectory(m_hFtpSession,
                             (LPTSTR)lpszDirectory.c_str()) == TRUE;
}

bool FTP::cd(TSTR lpszDirectory)
{
	return !m_connected ?
        false
        : FtpSetCurrentDirectory(m_hFtpSession,
                                 (LPTSTR)lpszDirectory.c_str()) == TRUE;
}

bool FTP::remove(TSTR lpszFileName)
{
	return !m_connected ?
        false
        : FtpDeleteFile(m_hFtpSession,
                        (LPTSTR)lpszFileName.c_str()) == TRUE;
}

bool FTP::rename(TSTR lpszOldFileName, TSTR lpszNewFileName)
{
	return !m_connected ?
        false
        : FtpRenameFile(m_hFtpSession,
                        (LPTSTR)lpszOldFileName.c_str(),
                        (LPTSTR)lpszNewFileName.c_str()) == TRUE;
}

bool FTP::rmdir(TSTR lpszDirectory)
{
	return !m_connected ?
        false
        : FtpRemoveDirectory(m_hFtpSession,
                             (LPTSTR)lpszDirectory.c_str()) == TRUE;
}

HINTERNET FTP::gethandle()
{
	return m_connected ? m_hFtpSession : NULL;
}

TSTR FTP::getcd()
{
	LPTSTR lpszCurrentDirectory = (LPTSTR)GlobalAlloc(GPTR, MAX_PATH + 1);
	DWORD lpdwCurrentDirectory  = MAX_PATH;
	TSTR strCurrentDirectory;

	if (!FtpGetCurrentDirectory(m_hFtpSession,
                                lpszCurrentDirectory,
                                &lpdwCurrentDirectory))
	{
		strCurrentDirectory = _T("ERROR-1");
		return strCurrentDirectory;
	}

	int len = (int)lpdwCurrentDirectory;
	for (int i = 0; i < len; i++)
	{
		strCurrentDirectory += lpszCurrentDirectory[i];
	}
	GlobalFree((HANDLE)lpszCurrentDirectory);

	return strCurrentDirectory;
}

LONGLONG FTP::getfilesize(TSTR lpszFileName)
{
    if(!m_connected)
        return INVALID_FILE;

    HINTERNET fHandle = FtpOpenFile(m_hFtpSession,
                                    (LPTSTR)lpszFileName.c_str(),
                                    GENERIC_READ,
                                    INTERNET_FLAG_TRANSFER_BINARY,
                                    0);
	LARGE_INTEGER llFileSize;
	DWORD dwFileSizeLow = INVALID_FILE;
	DWORD dwFileSizeHigh = INVALID_FILE;

	if(fHandle)
    {
        dwFileSizeLow = FtpGetFileSize(fHandle,
                                       &dwFileSizeHigh);


        InternetCloseHandle(fHandle);
    }

    llFileSize.LowPart = dwFileSizeLow;
    llFileSize.HighPart = dwFileSizeHigh;

	return llFileSize.QuadPart;
}

bool FTP::command(TSTR lpszCommand)
{
    return !m_connected ?
        false
        : FtpCommand(m_hFtpSession,
                     FALSE,
                     FTP_TRANSFER_TYPE_BINARY,
                     (LPTSTR)lpszCommand.c_str(),
                     0,
                     NULL) == TRUE;
}


