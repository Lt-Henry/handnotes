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

#include "Path.hpp"

#include <cmath>
#include <iostream>

using namespace handnotes;

using namespace std;

static float dot(BPoint &a,BPoint& b)
{
	return a.x * b.x + a.y * b.y;;
}

static float norm(BPoint &a)
{
	return std::sqrt((a.x*a.x)+(a.y*a.y));
}

static void ramer(vector<BPoint>& nodes, vector<int>& valids,int start,int end)
{
	float epsilon=0.5f;
	int worst=start;
	float current=0.0f;
	
	for (int n=start;n<=end;n++) {
		BPoint M=nodes[end]-nodes[start];
		
		BPoint AP = nodes[n] - nodes[start];
		float t0 = dot(M,AP) / dot(M,M);
		
		BPoint C = M;
		C.x*=t0;
		C.y*=t0;
		C = C + nodes[start];
		
		BPoint PC = nodes[n] - C;
		float dist = norm(PC);
		
		if (dist>current) {
			current = dist;
			worst = n;
		}
	}
	
	if (current>epsilon) {
		ramer(nodes,valids,start,worst);
		valids.push_back(worst);
		ramer(nodes,valids,worst,end);
	}
}

Path::Path(vector<BPoint>& nodes, rgb_color color, float width, bool simplify) :
	Object('PATH'), color(color), width(width)
{
	if (simplify) {
		if (nodes.size()<3) {
			vertices = nodes;
		}
		else {
		
			vector<int> valids;
			
			valids.push_back(0);
			ramer(nodes,valids,0,nodes.size()-1);
			valids.push_back(nodes.size()-1);
			
			for (int n:valids) {
				vertices.push_back(nodes[n]);
			}
		}
		
		clog<<"source: "<<nodes.size()<<endl;
		clog<<"ramer: "<<vertices.size()<<endl;
	}
	else {
		vertices = nodes;
	}
}

Path::Path(rgb_color color, float width, bool simplify) :
	Object('PATH',color,width), fSimplify(simplify)
{
}

BRect Path::Bounds()
{
	BRect ret(0,0,0,0);
	
	if (vertices.size()>0) {
		ret.left = vertices[0].x;
		ret.right = ret.left;
		ret.top = vertices[0].y;
		ret.bottom = ret.top;
	}
	
	for (BPoint& point : vertices) {
		if (point.x < ret.left) {
			ret.left = point.x;
		}
		if (point.x > ret.right) {
			ret.right = point.x;
		}
		if (point.y < ret.top) {
			ret.top = point.y;
		}
		if (point.y > ret.bottom) {
			ret.bottom = point.y;
		}
	}
	
	for (Object* child : fChildren) {
		BRect childBounds = child->Bounds();
		
		ret = ret | childBounds;
	}
	
	return ret;
}

void Path::Draw(BView* view)
{
	view->SetLineMode(B_ROUND_CAP,B_SQUARE_JOIN);
	view->SetDrawingMode(B_OP_ALPHA);
	
	view->SetHighColor(color);
	view->SetPenSize(width);
	view->StrokePolygon(vertices.data(),vertices.size(),false);
	
	view->SetHighColor({0,0,0,255});
	//view->SetPenSize(0.1f);
	/*
	for (BPoint& p:vertices) {
		view->FillEllipse(p,0.1,0.1);
	}
	*/
}

void Path::Begin(BPoint point)
{
	vertices.clear();
	vertices.push_back(point);
}

void Path::Step(BPoint point)
{
	vertices.push_back(point);
}

void Path::End(BPoint point)
{
	vertices.push_back(point);
	
	// ramer curve simplification
	if (fSimplify and vertices.size()>3) {
			vector<int> valids;
			
			valids.push_back(0);
			ramer(vertices,valids,0,vertices.size()-1);
			valids.push_back(vertices.size()-1);
			
			vector<BPoint> tmp;
			
			for (int n:valids) {
				tmp.push_back(vertices[n]);
			}
			
			vertices = tmp;
	}
}
