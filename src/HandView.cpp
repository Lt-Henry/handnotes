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

#include "HandView.h"

#include <Window.h>

#include <iostream>

using namespace std;

HandView::HandView(BRect frame)
: BView(frame, "HandView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
	action = Action::None;
}

HandView::~HandView()
{
}

void HandView::AttachedToWindow(void)
{
	ResizeTo(Window()->Bounds().right,Window()->Bounds().bottom);
}

void HandView::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case B_MOUSE_WHEEL_CHANGED:

		break;

		default:
			BView::MessageReceived(message);
		break;
	}
}

void HandView::FrameResized(float width, float height)
{
	BView::FrameResized(width,height);
}

void HandView::MouseDown(BPoint point)
{
	action = Action::Pencil;
	outline.push_back(point);
	Invalidate();
}

void HandView::MouseUp(BPoint point)
{
	action = Action::None;
	outline.clear();
	Invalidate();
}

void HandView::MouseMoved(BPoint point, uint32 transit,const BMessage* message)
{
	if (action==Action::Pencil) {
		outline.push_back(point);
		Invalidate();
	}
}

void HandView::Draw(BRect updateRect)
{
	rgb_color bg = {253,246,227,255};
	rgb_color fg = {0,0,0,0};
	
	SetHighColor(bg);
	FillRect(Bounds());
	
	if (action == Action::Pencil) {
		size_t count = outline.size();
		if (count>=2) {
			BeginLineArray(count);
			
			for (size_t n=0;n<count-1;n++) {
				AddLine(outline[n],outline[n+1],fg);
			}
			
			EndLineArray();
		}
	}
}
