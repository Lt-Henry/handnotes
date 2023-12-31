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

#ifndef HAND_VECTOR
#define HAND_VECTOR

#include <Point.h>

namespace handnotes
{

	class Vec2
	{
		public:

		Vec2()
		{
		}

		Vec2(BPoint a,BPoint b)
		{
			data = b-a;
		}

		Vec2(BPoint v)
		{
			data = v;
		}

		Vec2(float x,float y)
		{
			data = BPoint(x,y);
		}

		float Norm() const
		{
			return std::sqrt((data.x*data.x)+(data.y*data.y));
		}

		Vec2 Unit() const
		{
			float norm = Norm();
			BPoint v;

			v.x=data.x/norm;
			v.y=data.y/norm;

			return Vec2(v);
		}

		Vec2 Invert()
		{
			return Vec2(-data.x,-data.y);
		}

		Vec2 operator * (float s)
		{
			return Vec2(data.x*s,data.y*s);
		}
		
		Vec2 operator + (Vec2 v)
		{
			return Vec2(data + v.Point());
		}
		
		Vec2 operator * (Vec2 v)
		{
			Vec2 o;
			
			return o;
		}

		BPoint Point() const
		{
			return data;
		}
		
		float X() const
		{
			return data.x;
		}
		
		float Y() const
		{
			return data.y;
		}

		BPoint data;
	};
}

#endif
