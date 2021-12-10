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

#include "Path.h"

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
	float epsilon=0.3f;
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

Path::Path(vector<BPoint>& nodes, rgb_color color, float width) :
	Object('PATH'), color(color), width(width)
{
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
