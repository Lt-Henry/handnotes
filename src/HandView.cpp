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
	scale = 1.0f;
	ox = 0.0f;
	oy = 0.0f;
	action = Action::None;
	tool = Tool::Pencil;
	cursor_default = new BCursor(B_CURSOR_ID_SYSTEM_DEFAULT);
	cursor_grab = new BCursor(B_CURSOR_ID_GRABBING);
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
			delta = -message->FindFloat("be:wheel_delta_y");
			scale = scale + (delta*0.1f);

			if (scale<0.1f) {
				scale=0.1f;
			}
			
			if (scale>3.0f) {
				scale=3.0f;
			}
			Invalidate();
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
	int32 buttons = Window()->CurrentMessage()->FindInt32("buttons");

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
				paths.push_back(Path(outline,{94,129,172,128},1.5f));
			break;
			
			case Tool::Highlighter:
				paths.push_back(Path(outline,{255,117,0,64},12.0f));
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
	
	double dpi=96.0;
	double page_width = 8.3 * dpi;
	double page_height = 11.7 * dpi;
	
	//rgb_color bg = {253,246,227,255};
	rgb_color bg = {250,250,250,255};
	rgb_color fg = {0,0,0,0};
	
	BAffineTransform mat;
	SetTransform(mat);
	ScaleBy(scale,scale);
	TranslateBy(ox,oy);
	
	SetHighColor(bg);
	FillRect(BRect(0,0,page_width,page_height));
	SetHighColor(fg);
	StrokeRect(BRect(0,0,page_width,page_height));
	
	int dots_w = 8.0/0.196;
	int dots_h = 11.0/0.196;
	double dot_x=0.3;
	double dot_y=0.7;
	
	for (int i=0;i<dots_w;i++) {
		dot_y=0.35;
		for (int j=0;j<dots_h;j++) {
			SetHighColor({200,200,200,255});
			FillEllipse(BPoint(dot_x*dpi,dot_y*dpi),0.5,0.5);
			
			dot_y+=0.196;
		}
		dot_x+=0.196;
	}
	
	SetLineMode(B_ROUND_CAP,B_BUTT_JOIN);
	SetDrawingMode(B_OP_ALPHA);
	
	for (Path& path : paths) {

		SetHighColor(path.color);
		SetPenSize(path.width);
		StrokePolygon(path.vertices.data(),path.vertices.size(),false);
		/*
		for (BPoint& point : path.vertices) {
			SetHighColor({200,0,0,255});
			FillEllipse(point,0.5,0.5);
		}
		*/
	}
	
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
