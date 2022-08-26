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

#ifndef HAND_VIEW
#define HAND_VIEW

#include "Path.hpp"
#include "Page.hpp"

#include <View.h>
#include <Cursor.h>

#include <vector>

namespace handnotes
{
	enum class Action
	{
		None,
		Draw,
		Drag
	};

	enum class Tool
	{
		Pencil,
		Highlighter
	};

	class HandView : public BView
	{
		public:

		HandView(BRect frame);
		~HandView();

		virtual void AttachedToWindow(void);
		virtual void MessageReceived(BMessage* message);
		virtual void FrameResized(float width, float height);
		virtual void MouseDown(BPoint point);
		virtual void MouseUp(BPoint point);
		virtual void MouseMoved(BPoint point, uint32 transit,const BMessage* message);
		virtual void KeyDown(const char* bytes, int32 numBytes);
		virtual void Draw(BRect updateRect);
		
		handnotes::Page* page;
		
		protected:

		double dpi;
		double scale;
		float ox,oy;
		
		Action action;
		Tool tool;
		
		BPoint start;
		std::vector<BPoint> outline;
		std::vector<handnotes::Path> paths;
		
		
		
		BCursor* cursor_default;
		BCursor* cursor_grab;
	};

}
#endif
