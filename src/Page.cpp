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

#include "Page.hpp"

#include <vector>
#include <iostream>

using namespace handnotes;
using namespace std;

typedef struct 
{
	PageFormat format;
	// size in mm
	float width;
	float height;
	
} PageData;

vector<PageData> pageInfo = {
	{PageFormat::A3,297.0,420.0},
	{PageFormat::A4,210.0,297.0},
	{PageFormat::A4Landscape,297.0,210.0},
	{PageFormat::A5,148.0,210.0}
};

Page::Page(PageFormat format, PageType type,float dpi) : Object('PAGE'), format(format), type(type), dpi(dpi)
{
	
	for (PageData& info : pageInfo) {
		if (info.format==format) {
			width = info.width;
			height = info.height;
			break;
		}
	}
	
	pal = { {250,250,250,255},
			{0,0,0,0},
			{200,200,200,255}
	};
}

Page::~Page()
{

}

void Page::Draw(BView* view)
{
	float dpmm = dpi/25.4f;
	
	float width_px = width * dpmm;
	float height_px = height * dpmm;
	
	view->SetHighColor(pal.background);
	view->FillRect(BRect(0,0,width_px,height_px));
	view->SetHighColor(pal.border);
	view->SetPenSize(0.5f);
	view->StrokeRect(BRect(0,0,width_px,height_px));
	
	switch (type) {
		case PageType::Dotted: {
			int dots_w = 1.0 + ((width*0.90)/5.0);
			int dots_h = 1.0 + ((height*0.90)/5.0);
			double dot_x = 0.5 * (width - ((dots_w-1) * 5.0));
			double dot_y = 0.5 * (height - ((dots_h-1) * 5.0));

			for (int i=0;i<dots_w;i++) {
				dot_y = 0.5 * (height - ((dots_h-1) * 5.0));;
				for (int j=0;j<dots_h;j++) {
					view->SetHighColor(pal.dots);
					view->FillEllipse(BPoint(dot_x*dpmm,dot_y*dpmm),0.5*dpmm,0.5*dpmm);
					
					dot_y+=5.0;
				}
				dot_x+=5.0;
			}
		}
		break;
		
		case PageType::Squared: {
			int lines_w = (width*0.90)/10.0;
			int lines_h = (height*0.90)/10.0;
			double line_x = (width/2.0) - (lines_w * 10.0 / 2.0);
			double line_y = (height/2.0) - (lines_h * 10.0 / 2.0);
			
			BRect square;
			
			square.left = line_x * dpmm;
			square.top = line_y * dpmm;
			square.right = square.left + (10.0 * lines_w *dpmm);
			square.bottom = square.top + (10.0 * lines_h * dpmm);
			
			for (int j=0;j<lines_h;j++) {
				view->SetHighColor(pal.dots);
				view->MovePenTo(square.left,line_y*dpmm);
				view->StrokeLine(BPoint(square.right,line_y*dpmm));
				line_y+=10.0;
			}
			
			for (int i=0;i<lines_w;i++) {
				view->SetHighColor(pal.dots);
				view->MovePenTo(line_x*dpmm,square.top);
				view->StrokeLine(BPoint(line_x*dpmm,square.bottom));
				line_x+=10.0;
			}
			
			view->SetPenSize(2);
			view->StrokeRect(square);
		}
		break;
		
		case PageType::Ruled: {
			int lines_h = 1.0 + ((height*0.90)/10.0);
			double line_y = 0.5 * (height - ((lines_h-1) * 10.0));
			
			for (int j=0;j<lines_h;j++) {
				view->SetHighColor(pal.dots);
				view->MovePenTo(0.10*width*dpmm,line_y*dpmm);
				view->StrokeLine(BPoint(0.90*width*dpmm,line_y*dpmm));
				line_y+=10.0;
			}
		}
		break;
		
		default:
		break;
	}
	
	for (Object* child : fChildren) {
		child->Draw(view);
	}
	
	return;
}
