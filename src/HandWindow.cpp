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

#include "HandWindow.h"

#include <Application.h>

HandWindow::HandWindow()
: BWindow(BRect(100, 100, 100 + 720, 100 + 512), "HandNotes", B_TITLED_WINDOW, 0)
{
	htoolbar = new BGroupView();
	htoolbar->ResizeTo(Bounds().Width(),36);
	htoolbar->SetResizingMode(B_FOLLOW_LEFT_RIGHT);
	AddChild(htoolbar);
	
	view = new HandView(BRect(0,36,100,100));
	AddChild(view);
	
	rtoolbar = new BGroupView(B_VERTICAL);
	rtoolbar->MoveTo(view->Bounds().Width()-36,36);
	rtoolbar->ResizeTo(36,view->Bounds().Height());
	rtoolbar->SetResizingMode(B_FOLLOW_V_CENTER | B_FOLLOW_RIGHT);
	view->AddChild(rtoolbar);
	
}

HandWindow::~HandWindow()
{
}

bool HandWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
