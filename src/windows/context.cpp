/*
   context.cpp
*/

#include <windows.h>
#include "commands/context.h"
#include "common/application.h"
#include "frame/frame.h"
#include "frame/operations.h"

namespace rikiGlue
{

using namespace Gdiplus;
#define mGDIContext (mContext.mGraphics)

void
Context::context_t::transform()
{
	mGraphics.TranslateTransform(0.f, mHeight);
	mGraphics.ScaleTransform(1.f, -1.f);

	const Context::Rect &rect = Application::instance().transform();
	mGraphics.TranslateTransform(rect.origin.x, rect.origin.y);
	mGraphics.ScaleTransform(rect.width, rect.height);
}

Context::Context( HDC         hdc,
                  HBITMAP     image,
                  const Rect  &bounds ) :	
mContext(hdc, bounds.height),
	mImage(image)
{
	mContext.transform();
}

Context::~Context()
{

}

void
Context::setStrokeColor( float_t   r,
		                 float_t   g,
		                 float_t   b,
		                 float_t   a )
{
	mContext.mStroke = Color( static_cast<BYTE>(a*255.f), static_cast<BYTE>(r*255.f),
	                          static_cast<BYTE>(g*255.f), static_cast<BYTE>(b*255.f) );
}

void
Context::setFillColor( float_t   r,
		               float_t   g,
		               float_t   b,
		               float_t   a )
{
	mContext.mFill = Color( static_cast<BYTE>(a*255.f), static_cast<BYTE>(r*255.f),
	                        static_cast<BYTE>(g*255.f), static_cast<BYTE>(b*255.f) );
}

void
stringToWString( const std::string   &fromStr,
				 std::wstring        &toStr )
{
	const int sz = static_cast<int>(fromStr.length());
	toStr.resize(sz);
	::MultiByteToWideChar(CP_ACP, 0, fromStr.data(), sz, &(*toStr.begin()), sz);
}

void
Context::drawText( const std::string  &str,
                   const Point        &loc,
                   const float_t      fontSize,
                   const std::string  &fontName )
{
	std::wstring wstr, wname;
	stringToWString(str, wstr);
	stringToWString(fontName, wname);
	
	GraphicsState state = mGDIContext.Save();

	Matrix mx1;
	Matrix mx2(1.f, 0.f, 0.f, -1.f, 0.f, fontSize+fontSize/2.f);

	mGDIContext.GetTransform(&mx1);
	mx1.Multiply(&mx2);
	mGDIContext.SetTransform(&mx1);
	
	SolidBrush brush( mContext.mFill );
	Font       font(wname.data(), fontSize);
	PointF     pt(loc.x, -loc.y);

	mGDIContext.DrawString(wstr.data(), wstr.length(), &font, pt, &brush);
	mGDIContext.Restore(state);
}

void
Context::strokeRect( const Rect    &rect )
{
	Pen pen( mContext.mStroke );
	RectF gdiRect(rect.origin.x, rect.origin.y, rect.width, rect.height);
	mGDIContext.DrawRectangles(&pen, &gdiRect, 1);
}

void
Context::fillRect( const Rect    &rect )
{
	SolidBrush brush( mContext.mFill );
	RectF gdiRect(rect.origin.x, rect.origin.y, rect.width, rect.height);
	mGDIContext.FillRectangle(&brush, gdiRect);
}

bool
Context::blockOperation( const Rect         &inrect,
                         Block::operation_t op )
{
	if ( !mImage )
		return ( false );

	PointF points[2] = { PointF(inrect.origin.x, inrect.origin.y),
	                     PointF(inrect.origin.x+inrect.width, inrect.origin.y+inrect.height) };
	Matrix mx1;
	mGDIContext.GetTransform(&mx1);
	mx1.TransformPoints(points, 2);
	Gdiplus::Rect rect(points[0].X, points[1].Y, points[1].X-points[0].X, points[0].Y-points[1].Y);

	HDC gdc = mGDIContext.GetHDC();
	HDC mDC = ::CreateCompatibleDC(gdc);
	HDC dc = GetDC(NULL);
	HBITMAP hbitmap = ::CreateCompatibleBitmap(dc, rect.Width, rect.Height);
	ReleaseDC(NULL, dc);

	HGDIOBJ  mOldObject = ::SelectObject(mDC, hbitmap);
	::BitBlt(mDC, 0, 0, rect.Width, rect.Height, gdc, rect.X, rect.Y, SRCCOPY);

	BITMAP bitmap;
	::GetObject(hbitmap, sizeof(bitmap), &bitmap);

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

	const size_t rowBytes = bitmap.bmWidth * 4,
	             dwBmpSize = rowBytes * bitmap.bmHeight;

	std::vector<uint8_t> bgra(dwBmpSize);
	::GetDIBits(mDC, hbitmap, 0, bitmap.bmHeight, &bgra[0], (BITMAPINFO*) &bi, DIB_RGB_COLORS);

	Frame frame(bitmap.bmWidth, bitmap.bmHeight, Application::instance().decoderThread());

	const Frame::operation_t ops[3] = { bgraToRGB, op, rgbToBGRA };
	frame.operate(&ops[0], 1, &bgra[0], rowBytes);
	frame.operate(&ops[1], 1, frame.bytes(), frame.rowBytes());
	frame.operate(&ops[2], 1, frame.bytes(), frame.rowBytes(), &bgra[0], rowBytes);

	::SetDIBits(mDC, hbitmap, 0, bitmap.bmHeight, &bgra[0], (BITMAPINFO*) &bi, DIB_RGB_COLORS);
	::BitBlt(gdc, rect.X, rect.Y, rect.Width, rect.Height, mDC, 0, 0, SRCCOPY);

	::SelectObject(mDC, mOldObject);
	::DeleteObject(hbitmap);
	ReleaseDC(NULL, mDC);
	mGDIContext.ReleaseHDC(gdc);
	return ( true );
}

} /* namespace rikiGlue */
