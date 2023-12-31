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
#include "Path.hpp"
#include "Line.hpp"
#include "Arrow.hpp"
#include "Measure.hpp"

#include <Window.h>

#include <iostream>

using namespace handnotes;

using namespace std;

HandView::HandView(BRect frame)
: BView(frame, "HandView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_SUBPIXEL_PRECISE)
{
	dpi = 96.0;
	scale = 1.0f;
	ox = 0.0f;
	oy = 0.0f;
	action = Action::None;
	SetTool(Tool::FreeHand);
	SetDrawingTool(DrawingTool::Pencil);
	
	cursor_default = new BCursor(B_CURSOR_ID_SYSTEM_DEFAULT);
	cursor_grab = new BCursor(B_CURSOR_ID_GRABBING);
	
	page = new handnotes::Page(PageFormat::A4, PageType::Dotted, dpi);
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
		
		switch (tool) {
			case Tool::FreeHand:
				preview = new handnotes::Path(toolColor, toolWidth);
				clog<<"start drawing..."<<endl;
			break;
			
			case Tool::Ruler:
				preview = new handnotes::Line(toolColor, toolWidth);
			break;
			
			case Tool::Arrow:
				preview = new handnotes::Arrow(toolColor, toolWidth);
			break;
			
			case Tool::Measure:
				preview = new handnotes::Measure(toolColor, toolWidth, 0);
			break;
		}
		
		preview->Begin(where);
		
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
	where.x = where.x / scale;
	where.y = where.y / scale;
	where.x = where.x - ox;
	where.y = where.y - oy;
		
	if (action == Action::Draw) {
		action = Action::None;
		
		preview->End(where);
		page->Add(preview);
		preview = nullptr;
		clog<<"done"<<endl;
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

		preview->Step(where);

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
		preview->Draw(this);
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
	
	switch (drawingTool) {
		case DrawingTool::Pencil:
			toolWidth = 1.0f;
			toolColor = {94,129,172,230};
		break;

		case DrawingTool::Rollerball:
			toolWidth = 2.2f;
			toolColor = {32,32,32,255};
		break;

		case DrawingTool::Highlighter:
			toolWidth = 8.0f;
			toolColor = {255,117,0,128};
		break;
	}
}

void HandView::SetTool(Tool t)
{
	tool = t;
}
