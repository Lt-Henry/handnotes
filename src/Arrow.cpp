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

#include "Arrow.hpp"
#include "Vector.hpp"

#include <cmath>
#include <iostream>

using namespace handnotes;

using namespace std;

Arrow::Arrow(rgb_color color, float width) :
	Object('ARRW',color,width)
{
}

BRect Arrow::Bounds()
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

void Arrow::Draw(BView* view)
{
	view->SetLineMode(B_ROUND_CAP,B_SQUARE_JOIN);
	view->SetDrawingMode(B_OP_ALPHA);
	
	view->SetHighColor(StrokeColor());
	view->SetPenSize(StrokeWidth());
	view->StrokeLine(start,end);
	
	Vec2 ab = Vec2(start,end).Unit();
	Vec2 ba = ab.Invert();
	ba = ba * 10.0f;
	Vec2 left = Vec2(ba.data.y,-ba.data.x);
	Vec2 right = left.Invert();

	BPoint p(end.x+ba.data.x+left.data.x,end.y+ba.data.y+left.data.y);
	BPoint q(end.x+ba.data.x+right.data.x,end.y+ba.data.y+right.data.y);

	view->MovePenTo(end);
	view->StrokeLine(p);
	view->MovePenTo(end);
	view->StrokeLine(q);
	
	//view->SetHighColor({0,0,0,255});
}

void Arrow::Begin(BPoint point)
{
	start = point;
	end = start;
}

void Arrow::Step(BPoint point)
{
	end = point;
}

void Arrow::End(BPoint point)
{
	end = point;
}
