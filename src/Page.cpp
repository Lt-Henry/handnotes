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

Page::Page(PageFormat format, PageType type) : Object('PAGE'), format(format), type(type)
{
	
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

}

void Page::Draw(BView* view)
{
	float dpmm = 96.0/25.4;

	double width_px = width * dpmm;
	double height_px = height * dpmm;
	
	clog<<"page width "<< width<< " mm"<<endl;
	clog<<"page height "<< height<< " mm"<<endl;
	clog<<"pixels: "<<width_px<<"x"<<height_px<<endl;
	
	rgb_color bg = {250,250,250,255};
	rgb_color fg = {0,0,0,0};
	
	view->SetHighColor(bg);
	view->FillRect(BRect(0,0,width_px,height_px));
	view->SetHighColor(fg);
	view->StrokeRect(BRect(0,0,width_px,height_px));
	
	int dots_w = 1.0 + ((width*0.90)/5.0);
	int dots_h = 1.0 + ((height*0.90)/5.0);
	double dot_x = 0.5 * (width - ((dots_w-1) * 5.0));
	double dot_y = 0.5 * (height - ((dots_h-1) * 5.0));
	
	clog<<"dots "<<dots_w<<","<<dots_h<<endl;
	clog<<"90%:"<<width*0.90<<endl;
	clog<<"5%:"<<dot_x<<endl;
	
	for (int i=0;i<dots_w;i++) {
		dot_y = 0.5 * (height - ((dots_h-1) * 5.0));;
		for (int j=0;j<dots_h;j++) {
			view->SetHighColor({200,200,200,255});
			view->FillEllipse(BPoint(dot_x*dpmm,dot_y*dpmm),0.5,0.5);
			
			dot_y+=5.0;
		}
		dot_x+=5.0;
	}
	
	for (Object* child : fChildren) {
		child->Draw(view);
	}
	
	return;
}
