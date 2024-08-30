/*
The MIT License (MIT)

Copyright (c) 2023 Enrique Medina Gremaldos <quiqueiii@gmail.com>

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

#include "Measure.hpp"
#include "Vector.hpp"

#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace handnotes;

using namespace std;

Measure::Measure(rgb_color color, float width,int flags) :
	Object('LINE',color,width)
{
}

BRect Measure::Bounds()
{
	BRect ret(0,0,0,0);
	
	ret.left = start.x;
	ret.top = start.y;
	ret.right = end.x;
	ret.bottom = end.y;
	
	for (Object* child : fChildren) {
		BRect childBounds = child->Bounds();
		
		ret = ret | childBounds;
	}
	
	return ret;
}

void Measure::Draw(BView* view)
{
	float dpi = 96.0f;
	float dpmm = dpi/25.4f;
	
	view->SetLineMode(B_ROUND_CAP,B_SQUARE_JOIN);
	view->SetDrawingMode(B_OP_ALPHA);
	
	view->SetHighColor(StrokeColor());
	view->SetPenSize(StrokeWidth());
	view->StrokeLine(start,end);
	
	//view->SetHighColor({0,0,0,255});
	
	Vec2 ab = Vec2(start,end);
	float dist = ab.Norm();
	
	Vec2 middle = ab * 0.5f;
	middle = middle + start;
	
	Vec2 n(ab.Y(),-ab.X());
	n = n.Unit();
	
	Vec2 baseline(1,0);
	
	float cos_alpha = n * baseline;
	
	n = n * (20.0f/dpmm);
	middle = middle + n;
	

	stringstream ss;
	ss<<std::setprecision(2)<<std::fixed<<(dist/dpmm)<<" mm";
	
	view->MovePenTo(middle.Point());
	view->GetFont(&font);
	float angle = 0.0f;
	
	if (n.Y() > 0) {
		angle = 270.0f - (acos(cos_alpha) * 180.0f/M_PI);
	}
	else {
		angle = (acos(cos_alpha) * 180.0f/M_PI) - 90.0f;
	}

	
	font.SetRotation(angle);
	view->SetFont(&font);
	view->DrawString(ss.str().c_str());
}

void Measure::Begin(BPoint point)
{
	start = point;
	end = start;
}

void Measure::Step(BPoint point)
{
	end = point;
}

void Measure::End(BPoint point)
{
	end = point;
}
