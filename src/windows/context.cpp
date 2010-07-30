/*
   context.cpp
*/


#include "commands/context.h"
#include "common/application.h"

namespace rikiGlue
{

using namespace Gdiplus;
#define mGDIContext (mContext.mGraphics)

void
Context::context_t::transform()
{
	mGraphics.TranslateTransform(0.f, mHeight);
	mGraphics.ScaleTransform(1.f, -1.f);

	const rikiGlue::Rect &rect = Application::instance().transform();
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
	const uint32_t sz = static_cast<uint32_t>(fromStr.length());
	toStr.resize(sz);
	::MultiByteToWideChar(CP_ACP, 0, fromStr.data(), sz, &(*toStr.begin()), sz);
}

void
Context::drawText( const std::string  &str,
                   const Point        &loc,
                   const float_t      fontSize,
                   const std::string  &font )
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
	mGDIContext.DrawRectangles(&brush, &gdiRect);
}

} /* namespace rikiGlue */
