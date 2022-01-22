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

using namespace handnotes;
using namespace std;

HandView::HandView(BRect frame)
: BView(frame, "HandView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
	scale = 1.0f;
	ox = 0.0f;
	oy = 0.0f;
	action = Action::None;
	tool = Tool::Pencil;
	cursor_default = new BCursor(B_CURSOR_ID_SYSTEM_DEFAULT);
	cursor_grab = new BCursor(B_CURSOR_ID_GRABBING);
	
	page = new handnotes::Page(PageFormat::A4, PageType::Dotted);
}

HandView::~HandView()
{
}

void HandView::AttachedToWindow(void)
{
	ResizeTo(Window()->Bounds().right,Window()->Bounds().bottom);
	SetEventMask(B_KEYBOARD_EVENTS);
}

void HandView::MessageReceived(BMessage* message)
{
	float delta;
	
	switch (message->what) {
		case B_MOUSE_WHEEL_CHANGED:
			if(message->FindFloat("be:wheel_delta_y",&delta) == B_OK) {
				delta=-delta;
				scale = scale + (delta*0.1);

				if (scale<0.1) {
					scale=0.1;
				}
				
				if (scale>3.0) {
					scale=3.0;
				}
			Invalidate();
			}
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

void HandView::MouseDown(BPoint where)
{
	int32 buttons;
	
	if(Window()->CurrentMessage()->FindInt32("buttons",&buttons) != B_OK) {
		return;
	}

	if ((buttons & B_PRIMARY_MOUSE_BUTTON) != 0) {
		action = Action::Draw;
		
		where.x = where.x / scale;
		where.y = where.y / scale;
		where.x = where.x - ox;
		where.y = where.y - oy;
		outline.push_back(where);
		Invalidate();
	}

	if ((buttons & B_SECONDARY_MOUSE_BUTTON) != 0) {
		SetViewCursor(cursor_grab);
		action = Action::Drag;
		where.x = where.x / scale;
		where.y = where.y / scale;
		where.x = where.x - ox;
		where.y = where.y - oy;
		start = where;
	}
}

void HandView::MouseUp(BPoint where)
{

	if (action == Action::Draw) {
		action = Action::None;
		switch (tool) {
			case Tool::Pencil:
				page->Add(new Path(outline,{94,129,172,128},1.5f));
				
			break;
			
			case Tool::Highlighter:
				page->Add(new Path(outline,{255,117,0,64},12.0f));
			break;
		}
		outline.clear();
		Invalidate();
	}
	
	if (action == Action::Drag) {
		SetViewCursor(cursor_default);
		action = Action::None;
		Invalidate();
	}
}

void HandView::MouseMoved(BPoint where, uint32 transit,const BMessage* message)
{

	if (action==Action::Draw) {
		where.x = where.x / scale;
		where.y = where.y / scale;
		where.x = where.x - ox;
		where.y = where.y - oy;

		outline.push_back(where);
		Invalidate();
	}
	
	if (action == Action::Drag) {
		where.x = where.x / scale;
		where.y = where.y / scale;
		BPoint tmp = where-start;
		ox = tmp.x;
		oy = tmp.y;
		Invalidate();
	}
}

void HandView::KeyDown(const char* bytes, int32 numBytes)
{
	for (int32 n=0;n<numBytes;n++) {
		switch(bytes[n]) {
			case 'h':
				ox=0;
				oy=0;
				Invalidate();
			break;
			
			case 'z':
			//TODO: simple undo mechanism
				paths.pop_back();
				Invalidate();
			break;
			
			case '1':
				tool = Tool::Pencil;
			break;

			case '2':
				tool = Tool::Highlighter;
			break;
			
		}
	}
}

void HandView::Draw(BRect updateRect)
{
	
	//rgb_color bg = {253,246,227,255};
	rgb_color bg = {250,250,250,255};
	rgb_color fg = {0,0,0,0};
	
	BAffineTransform mat;
	SetTransform(mat);
	ScaleBy(scale,scale);
	TranslateBy(ox,oy);
	
	page->Draw(this);
	
	if (action == Action::Draw) {
		switch (tool) {
			case Tool::Pencil:
				SetHighColor({94,129,172,128});
				SetPenSize(1.5f);
				StrokePolygon(outline.data(),outline.size(),false);
			break;
			
			case Tool::Highlighter:
				SetHighColor({255,117,0,64});
				SetPenSize(12.0f);
				StrokePolygon(outline.data(),outline.size(),false);
			break;
		}
		
	}
}
