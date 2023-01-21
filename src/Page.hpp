/*
The MIT License (MIT)

Copyright (c) 2021 Enrique Medina Gremaldos <quiqueiii@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef HAND_PAGE
#define HAND_PAGE

#include "HandNotes.hpp"
#include "Object.hpp"

#include <View.h>
#include <Picture.h>

namespace handnotes
{
	enum class PageFormat
	{
		A3,
		A4,
		A4Landscape,
		A5
	};
	
	enum class PageType
	{
		Plain,
		Dotted,
		Squared,
		Ruled
	};
	
	class Page: public Object
	{
		public:
		
		Page(PageFormat format, PageType type,float dpi);
		~Page();
		
		PageFormat Format()
		{
			return format;
		}
		
		PageType Type()
		{
			return type;
		}
		
		float Width()
		{
			return width;
		}
		
		float Height()
		{
			return height;
		}
		
		BSize Size()
		{
			return BSize(width,height);
		}
		
		BRect Bounds() override
		{
			return BRect(0,0,width,height);
		}
		
		BRect PixelBounds(float dpi)
		{
			float dpmm = dpi/25.4;
			return BRect(0,0,dpmm * width, dpmm * height);
		}
		
		BRect ChildrenBounds();
		
		void Draw(BView* view) override;
		
		protected:
		Palette pal;
		
		PageFormat format;
		PageType type;
		
		float width;
		float height;
		
		float dpi;
		
	};
}

#endif
