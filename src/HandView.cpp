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

#include "HandView.hpp"

#include <Window.h>

#include <iostream>

using namespace handnotes;

using namespace std;

class Vec2
{
	public:
	
	Vec2(BPoint a,BPoint b)
	{
		data = b-a;
	}
	
	float Norm() const
	{
		return std::sqrt((data.x*data.x)+(data.y*data.y));
	}
	
	BPoint data;
};

HandView::HandView(BRect frame)
: BView(frame, "HandView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_SUBPIXEL_PRECISE)
{
	dpi = 96.0;
	scale = 1.0f;
	ox = 0.0f;
	oy = 0.0f;
	action = Action::None;
	tool = Tool::FreeHand;
	drawingTool = DrawingTool::Pencil;
	
	cursor_default = new BCursor(B_CURSOR_ID_SYSTEM_DEFAULT);
	cursor_grab = new BCursor(B_CURSOR_ID_GRABBING);
	
	page = new handnotes::Page(PageFormat::A4, PageType::Squared, dpi);
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
				
				if (delta < 0) {
					ZoomIn();
				}
				else {
					ZoomOut();
				}
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
		
		if (tool == Tool::Ruler) {
			outline.push_back(where);
		}
		
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
		
		switch (drawingTool) {
			case DrawingTool::Pencil:
				page->Add(new Path(outline,{94,129,172,230},1.0f));

			break;

			case DrawingTool::Rollerball:
				page->Add(new Path(outline,{32,32,32,255},2.2f));
			break;

			case DrawingTool::Highlighter:
				page->Add(new Path(outline,{255,117,0,128},8.0f));
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
		
		switch(tool) {
			case Tool::FreeHand:
				outline.push_back(where);
			break;
			
			case Tool::Arrow:
			case Tool::Ruler:
				outline[1] = where;
			break;
		}

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
				Home();
			break;
			
			case 'z':
				Invalidate();
			break;
			
			case '1':
				drawingTool = DrawingTool::Pencil;
			break;

			case '2':
				drawingTool = DrawingTool::Highlighter;
			break;
			
			case 'r':
				tool = Tool::Ruler;
			break;
			
			case 'f':
				tool = Tool::FreeHand;
			break;
		}
	}
}

void HandView::Draw(BRect updateRect)
{
	BAffineTransform mat;
	SetTransform(mat);
	ScaleBy(scale,scale);
	TranslateBy(ox,oy);
	
	page->Draw(this);
	
	if (action == Action::Draw) {
		switch (tool) {
			case Tool::FreeHand:
				switch(drawingTool) {
					case DrawingTool::Pencil:
						SetHighColor({94,129,172,230});
						SetPenSize(1.0);
						StrokePolygon(outline.data(),outline.size(),false);
					break;

					case DrawingTool::Rollerball:
						SetHighColor({32,32,32,255});
						SetPenSize(2.2);
						StrokePolygon(outline.data(),outline.size(),false);
					break;

					case DrawingTool::Highlighter:
						SetHighColor({255,117,0,128});
						SetPenSize(8.0f);
						StrokePolygon(outline.data(),outline.size(),false);
					break;
				}
			break;
			
			case Tool::Ruler:
				SetHighColor({32,32,32,128});
				SetPenSize(1.0f);
				MovePenTo(outline[0]);
				StrokeLine(outline[1]);
			break;
			
			case Tool::Arrow:
				SetHighColor({32,32,32,128});
				SetPenSize(1.0f);
				MovePenTo(outline[0]);
				StrokeLine(outline[1]);
			break;
		}
		
	}
}

void HandView::Home()
{
	ox=0;
	oy=0;
	Invalidate();
}

void HandView::ZoomIn()
{
	scale = scale - 0.1;
	if (scale < 0.1) {
		scale = 0.1;
	}
	Invalidate();
}

void HandView::ZoomOut()
{
	scale = scale + 0.1;
	if (scale > 5.0) {
		scale = 5.0;
	}
	Invalidate();
}

void HandView::NoZoom()
{
	scale = 1;
	Invalidate();
}

void HandView::ZoomFitPage()
{
	ox = 0;
	oy = 0;
	
	float dpmm = dpi/25.4f;
	
	if (Bounds().Height() < Bounds().Width()) {
		scale = Bounds().Height() / (page->Height()*dpmm);
	}
	else {
		scale = Bounds().Width() / (page->Width()*dpmm);
	}
	clog<<"scale:"<<scale<<endl;
	
	Invalidate();
}

void HandView::ZoomFitDrawing()
{
	BRect area = page->ChildrenBounds();
	
	float dpmm = dpi/25.4f;
	
	ox = area.left * dpmm;
	oy = area.top * dpmm;
	
	if (Bounds().Height() < Bounds().Width()) {
		scale = Bounds().Height() / (area.Height()*dpmm);
	}
	else {
		scale = Bounds().Width() / (area.Width()*dpmm);
	}
	
	clog<<"area"<<endl;
	area.PrintToStream();
	clog<<"scale:"<<scale<<endl;
	
	Invalidate();
}

void HandView::SetDrawingTool(DrawingTool dt)
{
	drawingTool = dt;
}

void HandView::SetTool(Tool t)
{
	tool = t;
}
