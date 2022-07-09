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

#include "HandNotes.hpp"
#include "ExportWindow.hpp"

#include <Application.h>
#include <LayoutBuilder.h>
#include <Button.h>

#include <iostream>

using namespace handnotes;

using namespace std;

ExportWindow::ExportWindow()
: BWindow(BRect(100, 100, 100 + 200, 100 + 100), "Export", B_FLOATING_WINDOW_LOOK, B_FLOATING_ALL_WINDOW_FEEL,0)
{
	dpi = 96;
	clog<<"ExportWindow"<<endl;
	
	BButton* btnExport = new BButton("Export", new BMessage(Message::ExportRequest));
	dpiControl = new BTextControl("DPI","96",new BMessage(Message::DpiChanged));
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(dpiControl)
		.Add(btnExport);
	//GetLayout()->Add(btnExport);

}

ExportWindow::~ExportWindow()
{
}

bool ExportWindow::QuitRequested()
{
	//TODO: not close the whole app
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

void ExportWindow::MessageReceived(BMessage* message)
{
	switch (message->what) {
	
		case Message::DpiChanged: {
				clog<<"Dpi:"<<dpiControl->Text()<<endl;
				try {
					dpi = std::stof(dpiControl->Text());
				}
				catch(std::exception& e) {
					cerr<<"can not parse dpi"<<endl;
				}
			}
		break;

		case Message::ExportRequest:
			clog<<"Export to "<<dpi<<endl;
		break;

		default:
		//clog<<"message: "<<message->what<<endl;
		BWindow::MessageReceived(message);
	}
}

void ExportWindow::WindowActivated(bool active)
{
	clog<<"activated:"<<active<<endl;
	
}
