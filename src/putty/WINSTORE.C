/*
 * winstore.c: Windows-specific implementation of the interface
 * defined in storage.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <tchar.h>

#include "putty/putty.h"
#include "putty/storage.h"

#include <shlobj.h>
#ifndef CSIDL_APPDATA
#define CSIDL_APPDATA 0x001a
#endif
#ifndef CSIDL_LOCAL_APPDATA
#define CSIDL_LOCAL_APPDATA 0x001c
#endif

static int tried_shgetfolderpath = FALSE;
static HMODULE shell32_module = NULL;
typedef HRESULT (WINAPI *p_SHGetFolderPath_t)
    (HWND, int, HANDLE, DWORD, LPTSTR);
static p_SHGetFolderPath_t p_SHGetFolderPath = NULL;

/*
 * Open (or delete) the random seed file.
 */
enum { DEL, OPEN_R, OPEN_W };
static int try_random_seed(TCHAR const *path, int action, HANDLE *ret)
{
    if (action == DEL) {
	DeleteFile(path);
	*ret = INVALID_HANDLE_VALUE;
	return FALSE;		       /* so we'll do the next ones too */
    }

    *ret = CreateFile(path,
		      action == OPEN_W ? GENERIC_WRITE : GENERIC_READ,
		      action == OPEN_W ? 0 : (FILE_SHARE_READ |
					      FILE_SHARE_WRITE),
		      NULL,
		      action == OPEN_W ? CREATE_ALWAYS : OPEN_EXISTING,
		      action == OPEN_W ? FILE_ATTRIBUTE_NORMAL : 0,
		      NULL);

    return (*ret != INVALID_HANDLE_VALUE);
}


static HANDLE access_random_seed(int action)
{
    HKEY rkey;
    DWORD type, size;
    HANDLE rethandle;
    TCHAR seedpath[2 * MAX_PATH + 10] = TEXT("\0");

    /*
     * Iterate over a selection of possible random seed paths until
     * we find one that works.
     * 
     * We do this iteration separately for reading and writing,
     * meaning that we will automatically migrate random seed files
     * if a better location becomes available (by reading from the
     * best location in which we actually find one, and then
     * writing to the best location in which we can _create_ one).
     */

    /*
     * First, try the location specified by the user in the
     * Registry, if any.
     */
    size = sizeof(seedpath);
    if (RegOpenKey(HKEY_CURRENT_USER, TEXT(PUTTY_REG_POS), &rkey) ==
	ERROR_SUCCESS) {
	int ret = RegQueryValueEx(rkey, TEXT("RandSeedFile"),
				  0, &type, (LPBYTE) seedpath, &size);
	if (ret != ERROR_SUCCESS || type != REG_SZ)
	    seedpath[0] = '\0';
	RegCloseKey(rkey);

	if (*seedpath && try_random_seed(seedpath, action, &rethandle))
	    return rethandle;
    }

    /*
     * Next, try the user's local Application Data directory,
     * followed by their non-local one. This is found using the
     * SHGetFolderPath function, which won't be present on all
     * versions of Windows.
     */
    if (!tried_shgetfolderpath) {
	/* This is likely only to bear fruit on systems with IE5+
	 * installed, or WinMe/2K+. There is some faffing with
	 * SHFOLDER.DLL we could do to try to find an equivalent
	 * on older versions of Windows if we cared enough.
	 * However, the invocation below requires IE5+ anyway,
	 * so stuff that. */
	shell32_module = LoadLibrary(TEXT("SHELL32.DLL"));
	if (shell32_module) {
	    p_SHGetFolderPath = (p_SHGetFolderPath_t)
#if defined(UNICODE) || defined(_UNICODE)
		GetProcAddress(shell32_module, "SHGetFolderPathW");
#else
		GetProcAddress(shell32_module, "SHGetFolderPathA");
#endif
	}
    }
    if (p_SHGetFolderPath) {
	if (SUCCEEDED(p_SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA,
					NULL, SHGFP_TYPE_CURRENT, seedpath))) {
	    _tcscat(seedpath, TEXT("\\PUTTY.RND"));
	    if (try_random_seed(seedpath, action, &rethandle))
		return rethandle;
	}

	if (SUCCEEDED(p_SHGetFolderPath(NULL, CSIDL_APPDATA,
					NULL, SHGFP_TYPE_CURRENT, seedpath))) {
	    _tcscat(seedpath, TEXT("\\PUTTY.RND"));
	    if (try_random_seed(seedpath, action, &rethandle))
		return rethandle;
	}
    }

    /*
     * Failing that, try %HOMEDRIVE%%HOMEPATH% as a guess at the
     * user's home directory.
     */
    {
	int len, ret;

	len =
	    GetEnvironmentVariable(TEXT("HOMEDRIVE"), seedpath,
				   sizeof(seedpath));
	ret =
	    GetEnvironmentVariable(TEXT("HOMEPATH"), seedpath + len,
				   sizeof(seedpath) - len);
	if (ret != 0) {
	    _tcscat(seedpath, TEXT("\\PUTTY.RND"));
	    if (try_random_seed(seedpath, action, &rethandle))
		return rethandle;
	}
    }

    /*
     * And finally, fall back to C:\WINDOWS.
     */
    GetWindowsDirectory(seedpath, sizeof(seedpath));
    _tcscat(seedpath, TEXT("\\PUTTY.RND"));
    if (try_random_seed(seedpath, action, &rethandle))
	return rethandle;

    /*
     * If even that failed, give up.
     */
    return INVALID_HANDLE_VALUE;
}

void read_random_seed(noise_consumer_t consumer)
{
    HANDLE seedf = access_random_seed(OPEN_R);

    if (seedf != INVALID_HANDLE_VALUE) {
	while (1) {
	    char buf[1024];
	    DWORD len;

	    if (ReadFile(seedf, buf, sizeof(buf), &len, NULL) && len)
		consumer(buf, len);
	    else
		break;
	}
	CloseHandle(seedf);
    }
}

void write_random_seed(void *data, int len)
{
    HANDLE seedf = access_random_seed(OPEN_W);

    if (seedf != INVALID_HANDLE_VALUE) {
	DWORD lenwritten;

	WriteFile(seedf, data, len, &lenwritten, NULL);
	CloseHandle(seedf);
    }
}