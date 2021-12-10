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

#include "Page.h"

#include <vector>

using namespace handnotes;
using namespace std;

typedef struct 
{
	PageFormat format;
	// size in mm
	double width;
	double height;
	
} PageData;

vector<PageData> pageInfo = {
	{PageFormat::A3,297.0,420.0},
	{PageFormat::A4,210.0,297.0},
	{PageFormat::A4Landscape,297.0,210.0},
	{PageFormat::A5,148.0,210.0}
};

Page::Page(PageFormat format, PageType type) : Object('PAGE'), format(format), type(type),picture(nullptr), dpi(96.0)
{
	dpmm = dpi/25.4;
	
	for (PageData& info : pageInfo) {
		if (info.format==format) {
			width = info.width;
			height = info.height;
			break;
		}
	}
}

Page::~Page()
{
	if (picture) {
		delete picture;
	}
}

BPicture* Page::Draw(BView* view)
{
	if (picture) {
		return picture;
	}

	view->BeginPicture(new BPicture);

	double page_width = width * dpmm;
	double page_height = height * dpmm;
	
	rgb_color bg = {250,250,250,255};
	rgb_color fg = {0,0,0,0};
	
	view->SetHighColor(bg);
	view->FillRect(BRect(0,0,page_width,page_height));
	view->SetHighColor(fg);
	view->StrokeRect(BRect(0,0,page_width,page_height));
	
	int dots_w = 8.0/0.196;
	int dots_h = 11.0/0.196;
	double dot_x=0.3;
	double dot_y=0.7;
	
	for (int i=0;i<dots_w;i++) {
		dot_y=0.35;
		for (int j=0;j<dots_h;j++) {
			view->SetHighColor({200,200,200,255});
			view->FillEllipse(BPoint(dot_x*dpi,dot_y*dpi),0.5,0.5);
			
			dot_y+=0.196;
		}
		dot_x+=0.196;
	}

	picture = view->EndPicture();
	
	return picture;
}
