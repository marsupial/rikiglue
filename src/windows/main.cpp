/*
	main.cpp
*/

#include <windows.h>
#include <deque>

#include "windows/resource.h"


typedef std::deque<HWND>  list_t;

static const TCHAR
	kWindowTitle[] = L"glue",
	kWindowClass[] = L"gabba",
	kBitmapProperty[] = L"gabbaBitmap";

static ATOM
registerClass( HINSTANCE     hInstance );

static HWND
initInstance( HINSTANCE    instance,
              int          show )
{
	HWND window = ::CreateWindow(kWindowClass, kWindowTitle, WS_OVERLAPPEDWINDOW,
	                             CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, instance, NULL);

	if ( window != NULL )
	{
		::ShowWindow(window, show);
		::UpdateWindow(window);
	}

	return ( window );
}

int APIENTRY
WinMain( HINSTANCE    instance,
         HINSTANCE    prevInstance,
         LPSTR        cmdLine,
         int          show )
{
	registerClass(instance);

	// Perform application initialization:
	HWND window = initInstance(instance, show);
	if ( window == NULL )
		return FALSE;

	::SetTimer(window, 1, 1000/25, NULL);

	MSG    msg;
	HACCEL accelTable = ::LoadAccelerators(instance, MAKEINTRESOURCE(IDC_APP));

	// Main message loop:
	while ( ::GetMessage(&msg, NULL, 0, 0) )
	{
		if ( !::TranslateAccelerator(msg.hwnd, accelTable, &msg) )
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	return ( msg.wParam );
}

class Gabba
{
public:

	Gabba( HWND    window,
	       HDC     dc ) :
		mWindow(window),
		mDC(NULL),
		mBitmap(NULL),
		mOldObject(NULL)
	{
		::GetClientRect(window, &mRect);
	
		LPPOINT pts = (LPPOINT) &mRect;
		::ClientToScreen(window, pts++);
		::ClientToScreen(window, pts);

		//This is the best stretch mode
		mDC = ::CreateCompatibleDC(dc);

		dc = GetDC(NULL);
		mBitmap = ::CreateCompatibleBitmap(dc, mRect.right-mRect.left, mRect.bottom-mRect.top);
		ReleaseDC(NULL, dc);

		mOldObject = ::SelectObject(mDC, mBitmap);
		::SetStretchBltMode(mDC, HALFTONE);
	}

	~Gabba()
	{
		if ( mBitmap )
		{
			::SelectObject(mDC, mOldObject);
			::DeleteObject(mBitmap);
		}
		::ReleaseDC(mWindow, mDC);
	}

	void
	operator () ( HWND    window )
	{
		if ( window == mWindow )
			return;

		RECT srcRect, iRect;
		::GetWindowRect(window, &srcRect);
		if ( !IntersectRect(&iRect, &mRect, &srcRect) )
			return;
		
		RECT dstRect = iRect;
		srcRect = iRect;

		::MapWindowPoints(HWND_DESKTOP, mWindow, (LPPOINT) &dstRect, 2);
		::MapWindowPoints(HWND_DESKTOP, window, (LPPOINT) &srcRect, 2);

		HDC winDC = ::GetDC(window);

		::StretchBlt( mDC, dstRect.left, dstRect.top, dstRect.right-dstRect.left, dstRect.bottom-dstRect.top, 
		              winDC, srcRect.left, srcRect.top, srcRect.right-srcRect.left, srcRect.bottom-srcRect.top,
		              SRCCOPY );

		::ReleaseDC(window, winDC);
	}

	HBITMAP
	bitmap()
	{
		if ( HBITMAP bitmap = mBitmap )
		{
			::SelectObject(mDC, mOldObject);
			mBitmap = NULL;
			return ( bitmap );
		}

		return ( NULL );
	}

	void
	saveBitmap( const TCHAR    *filePath )
	{
		BITMAP bmpScreen;
		// Get the BITMAP from the HBITMAP
		GetObject(mBitmap, sizeof(BITMAP), &bmpScreen);
		 
		BITMAPFILEHEADER   bmfHeader;    
		BITMAPINFOHEADER   bi;
		 
		bi.biSize = sizeof(BITMAPINFOHEADER);    
		bi.biWidth = bmpScreen.bmWidth;    
		bi.biHeight = bmpScreen.bmHeight;  
		bi.biPlanes = 1;    
		bi.biBitCount = 32;    
		bi.biCompression = BI_RGB;    
		bi.biSizeImage = 0;  
		bi.biXPelsPerMeter = 0;    
		bi.biYPelsPerMeter = 0;    
		bi.biClrUsed = 0;    
		bi.biClrImportant = 0;

		DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

		// Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
		// call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
		// have greater overhead than HeapAlloc.
		HANDLE hDIB = GlobalAlloc(GHND,dwBmpSize); 
		char *lpbitmap = (char *)GlobalLock(hDIB);    

		// Gets the "bits" from the bitmap and copies them into a buffer 
		// which is pointed to by lpbitmap.
		GetDIBits(mDC, mBitmap, 0, (UINT)bmpScreen.bmHeight, lpbitmap, (BITMAPINFO *)&bi, DIB_RGB_COLORS);

		// A file is created, this is where we will save the screen capture.
		HANDLE hFile = CreateFile(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);   

		// Add the size of the headers to the size of the bitmap to get the total file size
		DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		//Offset to where the actual bitmap bits start.
		bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER); 

		//Size of the file
		bmfHeader.bfSize = dwSizeofDIB; 

		//bfType must always be BM for Bitmaps
		bmfHeader.bfType = 0x4D42; //BM   

		DWORD dwBytesWritten = 0;
		WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
		WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
		WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

		//Unlock and Free the DIB from the heap
		GlobalUnlock(hDIB);    
		GlobalFree(hDIB);

		//Close the handle for the file that was created
		CloseHandle(hFile);

	}

	void
	blit( HDC    dstDC )
	{
		::BitBlt(dstDC, 0,0, mRect.right-mRect.left, mRect.bottom-mRect.top, mDC, 0,0, SRCCOPY);
	}

private:

	HWND     mWindow;
	HBITMAP  mBitmap;
	HGDIOBJ  mOldObject;
	HDC      mDC;
	RECT     mRect;
};


static BOOL CALLBACK
enumWindowProc( HWND    window,
                LPARAM  lParam )
{
	if ( IsWindowVisible(window) )
		reinterpret_cast<list_t*>(lParam)->push_back(window);

	return ( TRUE );
}

static void
saveScreen( HWND    window )
{
	list_t windows;
	::EnumWindows(enumWindowProc, reinterpret_cast<LPARAM>(&windows));

	HDC windowDC = ::GetDC(window);
	if ( windows.size() )
	{
		Gabba gabba(window, windowDC);
		list_t::reverse_iterator  itr = windows.rbegin(),
		                          end = windows.rend();
		while ( itr != end )
		{
			gabba( *itr );
			++itr;
		}

		HBITMAP newBitmap = gabba.bitmap();
		HANDLE oldBitmap = ::GetProp(window, kBitmapProperty);
		::SetProp(window, kBitmapProperty, newBitmap);
		if ( oldBitmap )
			::DeleteObject(oldBitmap);
	}
	::ReleaseDC(window, windowDC);
}

static INT_PTR CALLBACK
aboutProc( HWND      window,
           UINT      message,
           WPARAM    wParam,
           LPARAM    lParam )
{
	switch ( message )
	{
		case WM_INITDIALOG:
			return ( TRUE  );

		case WM_COMMAND:
			if ( LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL )
			{
				::EndDialog(window, LOWORD(wParam));
				return ( TRUE  );
			}
			break;
	}
	return ( FALSE  );
}

static LRESULT CALLBACK
windowProc( HWND      window,
            UINT      message,
            WPARAM    wParam,
            LPARAM    lParam )
{
	switch ( message )
	{
		case WM_COMMAND:
			switch ( LOWORD(wParam) )
			{
			case IDM_ABOUT:
				::DialogBox( reinterpret_cast<HINSTANCE>(::GetWindowLong(window, GWL_HINSTANCE)),
				             MAKEINTRESOURCE(IDD_ABOUTBOX), window, aboutProc );
				break;
			case IDM_EXIT:
				::DestroyWindow(window);
				break;
			default:
				return ( ::DefWindowProc(window, message, wParam, lParam) );
			}
			break;

		case WM_TIMER:
			saveScreen(window);

		case WM_MOVE:
		case WM_SIZE:
			::RedrawWindow(window, NULL, NULL, RDW_INVALIDATE);
			break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(window, &ps);
			if ( HGDIOBJ bitmap = ::GetProp(window, kBitmapProperty) )
			{
				HDC bdc = CreateCompatibleDC(hdc);
				HGDIOBJ obj = ::SelectObject(bdc, bitmap);
			
				RECT rect;
				::GetClientRect(window, &rect);
			
				::BitBlt(hdc, 0, 0, rect.right, rect.bottom, bdc, 0, 0, SRCCOPY);
				
				::SelectObject(bdc, obj);
				::DeleteDC(bdc);
			}
			::EndPaint(window, &ps);
			}
			break;

		case WM_DESTROY:
			::PostQuitMessage(0);
			break;

		default:
			return ( ::DefWindowProc( window, message, wParam, lParam) );
	}

	return ( 0 );
}

static ATOM
registerClass( HINSTANCE     hInstance )
{
	WNDCLASSEX wcex;

	wcex.cbSize         = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= windowProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor		= ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_APP);
	wcex.lpszClassName	= kWindowClass;
	wcex.hIconSm		= ::LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return ( ::RegisterClassEx(&wcex) );
}