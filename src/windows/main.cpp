/*
	main.cpp
*/

#include <windows.h>
#include <deque>
#include <vector>

#include "windows/resource.h"
#include "frame/frame.h"
#include "threads/threads.h"

namespace rikiGlue
{
	typedef std::deque<HWND>  list_t;

	static const TCHAR
		kWindowTitle[] = L"glue",
		kWindowClass[] = L"gabba",
		kBitmapProperty[] = L"gabbaBitmap";

	static DecodeThread
		*sDecodeThread = NULL;

	static LRESULT CALLBACK
	windowProc( HWND      window,
				UINT      message,
				WPARAM    wParam,
				LPARAM    lParam );
}

static HWND
initInstance( HINSTANCE    instance,
              int          show )
{
	using namespace rikiGlue;

	WNDCLASSEX wcex;
	wcex.cbSize         = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= rikiGlue::windowProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= instance;
	wcex.hIcon			= ::LoadIcon(instance, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor		= ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_APP);
	wcex.lpszClassName	= kWindowClass;
	wcex.hIconSm		= ::LoadIcon(instance, MAKEINTRESOURCE(IDI_SMALL));

	if ( ::RegisterClassEx(&wcex) == 0 )
		return ( NULL );

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
	rikiGlue::DecodeThread  thread;
	if ( thread.create() == 0 )
		rikiGlue::sDecodeThread = &thread;

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

namespace rikiGlue
{

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

	bool
	operator () ( HWND    window )
	{
		if ( window == mWindow )
			return ( false );

		RECT srcRect, iRect;
		::GetWindowRect(window, &srcRect);
		if ( !IntersectRect(&iRect, &mRect, &srcRect) )
			return ( true );
		
		RECT dstRect = iRect;
		srcRect = iRect;

		::MapWindowPoints(HWND_DESKTOP, mWindow, (LPPOINT) &dstRect, 2);
		::MapWindowPoints(HWND_DESKTOP, window, (LPPOINT) &srcRect, 2);

		HDC winDC = ::GetDC(window);

		::StretchBlt( mDC, dstRect.left, dstRect.top, dstRect.right-dstRect.left, dstRect.bottom-dstRect.top, 
		              winDC, srcRect.left, srcRect.top, srcRect.right-srcRect.left, srcRect.bottom-srcRect.top,
		              SRCCOPY );

		::ReleaseDC(window, winDC);
		return ( true );
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
		editBitmap( rikiGlue::DecodeThread    *thread )
	{
		BITMAP bitmap;
		::GetObject(mBitmap, sizeof(bitmap), &bitmap);
 
		BITMAPINFOHEADER   bi;

		bi.biSize = sizeof(BITMAPINFOHEADER);    
		bi.biWidth = bitmap.bmWidth;    
		bi.biHeight = bitmap.bmHeight;  
		bi.biPlanes = 1;    
		bi.biBitCount = 32;    
		bi.biCompression = BI_RGB;    
		bi.biSizeImage = 0;  
		bi.biXPelsPerMeter = 0;    
		bi.biYPelsPerMeter = 0;    
		bi.biClrUsed = 0;    
		bi.biClrImportant = 0;

		const size_t rowBytes = bitmap.bmWidth * 4, //((bitmap.bmWidth * bi.biBitCount + 23) / 24) * 3,
		             dwBmpSize = rowBytes * bitmap.bmHeight;

		typedef std::vector<uint8_t> buffer_t;

		buffer_t bytes(dwBmpSize);
		
		::GetDIBits(mDC, mBitmap, 0, bitmap.bmHeight, &bytes[0], (BITMAPINFO*) &bi, DIB_RGB_COLORS);

		Frame   frame(&bytes[0], rowBytes, bitmap.bmWidth, bitmap.bmHeight, Frame::kFormatBGRA);
		Frame::operation_t ops[] = { lutDecrypt, bChannel };
		frame.operate(ops, 2, thread);

		::SetDIBits(mDC, mBitmap, 0, bitmap.bmHeight, &bytes[0], (BITMAPINFO*) &bi, DIB_RGB_COLORS);
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
saveScreen( HWND            window,
            DecodeThread    *thread )
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
			if ( gabba( *itr ) == false )
				break;
			++itr;
		}

		gabba.editBitmap(thread);

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
			saveScreen(window, sDecodeThread);

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

} /* namespace rikiGlue */
