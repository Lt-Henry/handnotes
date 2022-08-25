/*
The MIT License (MIT)

Copyright (c) 2022 Enrique Medina Gremaldos <quiqueiii@gmail.com>

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

#include "Disk.hpp"
#include "Path.hpp"

#include <File.h>

#include <iostream>

using namespace handnotes;

using namespace std;

Page* handnotes::io::LoadPage(const BEntry* entry)
{
	//ToDo
	if (!entry) {
		cerr<<"null entry"<<endl;
		return nullptr;
	}
	
	BFile file(entry,B_READ_ONLY);
	
	if (!file.IsReadable()) {
		cerr<<"Can not read file"<<endl;
		return nullptr;
	}
	
	uint32 u32[4];
	float f32[4];
	double f64[4];
	
	file.Read(&u32[0],4);
	file.Read(&u32[1],4);
	
	if (u32[0]!='HNDN' or u32[1]!='V000') {
		cerr<<"Invalid header"<<endl;
		return nullptr;
	}
	
	file.Read(&u32[0],4);
	file.Read(&u32[1],4);
	file.Read(&u32[2],4);
	
	if (u32[0]!='PAGE') {
		cerr<<"unexpected ID"<<endl;
		return nullptr;
	}
	
	// dpi
	file.Read(&f64[0],8);
	
	Page*page = new Page(static_cast<PageFormat>(u32[1]),static_cast<PageType>(u32[2]));
	
	read_object:
	if (file.Read(&u32[0],4)>0) {
	
		switch (u32[0]) {
			case 'PATH': {
				file.Read(&f32[0],4); // width
				rgb_color color;
				file.Read(&color,sizeof(rgb_color));
				file.Read(&u32[1],4); //num vertices
				
				std::vector<BPoint> nodes;
				
				for (uint32 vn=0;vn<u32[1];vn++) {
					file.Read(&f32[1],4);
					file.Read(&f32[2],4);
					
					BPoint point(f32[1],f32[2]);
					nodes.push_back(point);
				}
				Path* path = new Path(nodes,color,f32[0],false);
				page->Add(path);
			}
			break;
			
		}
	}
	else {
		return page;
	}
	
	goto read_object;
	
}

void handnotes::io::SavePage(const BEntry* entry,Page* page)
{
	if (!page or !entry) {
		return;
	}
	
	BFile file(entry,B_WRITE_ONLY | B_CREATE_FILE);
	
	uint32 data[8];
	double f64;
	float f32;
	
	data[0] = 'HNDN';
	data[1] = 'V000';
	file.Write(&data[0],8);
	
	data[0] = page->Id();
	file.Write(&data[0],4);
	
	data[0] = static_cast<int32>(page->Format());
	file.Write(&data[0],4);
	
	data[0] = static_cast<int32>(page->Type());
	file.Write(&data[0],4);
	
	f64 = 96.0;
	file.Write(&f64,8);
	
	for (Object* obj:page->Children()) {
		switch (obj->Id()) {
			case 'PATH': {
				data[0] = obj->Id();
				file.Write(&data[0],4);
				Path* path = static_cast<Path*>(obj);
				f32 = path->width;
				file.Write(&f32,4);
				file.Write(&path->color,sizeof(rgb_color));
				data[0] = path->vertices.size();
				file.Write(&data[0],4);
				
				for (BPoint point : path->vertices) {
					file.Write(&point.x,4);
					file.Write(&point.y,4);
				}
			}
			break;
		}
	}
	
	file.Flush();
}
