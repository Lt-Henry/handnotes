/*
The MIT License (MIT)

Copyright (c) 2026 Enrique Medina Gremaldos <quiqueiii@gmail.com>

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

#ifndef HAND_DOCUMENT
#define HAND_DOCUMENT

#include "HandNotes.hpp"

#include <vector>

namespace handnotes
{
	class Page;
	
	class Document
	{
		public:
		
		Document(PageFormat format, PageType type,float dpi);
		~Document();
		
		float GetDpi() const
		{
			return dpi;
		}
		
		void SetDpi(float value)
		{
			dpi = value;
		}
		
		Page* AddPage();
		void RemovePage(int index);
		void RemovePage(Page* page);
		
		std::vector<Page*>& Pages()
		{
			return pages;
		}
		
		protected:
		
		float dpi;
		PageFormat format;
		PageType type;
		
		std::vector<Page *> pages;
	};
}

#endif
