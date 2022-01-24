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

#include "HandWindow.hpp"

#include <Application.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
//#include <Path.h>
#include <Entry.h>
#include <Directory.h>
#include <File.h>
#include <OS.h>

#include <iostream>

using namespace std;

HandWindow::HandWindow()
: BWindow(BRect(100, 100, 100 + 720, 100 + 512), "HandNotes", B_TITLED_WINDOW, 0)
{

	BMenuBar* menu = new BMenuBar(BRect(0, 0, Bounds().Width(), 22), "menubar");
	BMenu* menuFile = new BMenu("File");
	
	menu->AddItem(menuFile);
	
	menuFile->AddItem(new BMenuItem("New",new BMessage('HNNW')));
	menuFile->AddItem(new BMenuItem("Open",new BMessage('HNLD')));
	menuFile->AddItem(new BMenuItem("Save",new BMessage('HNSV')));
	menuFile->AddItem(new BMenuItem("Quit",new BMessage('HNQT')));
	
	AddChild(menu);
	view = new HandView(BRect(0,23,100,100));
	AddChild(view);
	
	BMessage *saveMsg = new BMessage(B_SAVE_REQUESTED);
	
	savePanel = new BFilePanel(B_SAVE_PANEL, NULL, NULL,
		B_FILE_NODE, false, saveMsg, NULL, true, true);
	
	savePanel->SetTarget(this);
	
	BMessage *openMsg = new BMessage(B_REFS_RECEIVED);
	
	openPanel = new BFilePanel(B_OPEN_PANEL, NULL, NULL,
		B_FILE_NODE, false, openMsg, NULL, true, true);
	
	openPanel->SetTarget(this);

	/*
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
	*/
}

HandWindow::~HandWindow()
{
}

bool HandWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

void HandWindow::MessageReceived(BMessage* message)
{

	switch (message->what) {
	
		//new
		case 'HNNW':
		break;
		
		//load file
		case 'HNLD':
			openPanel->Show();
		break;
		
		//save file
		case 'HNSV': {
			savePanel->Show();
		}
		break;
		
		case 'HNQT':
			be_app->PostMessage(B_QUIT_REQUESTED);
		break;
		
		case B_SAVE_REQUESTED: {
			
			entry_ref ref;
			if (message->FindRef("directory", 0, &ref) == B_OK) {
				BPath path;
				BEntry entry = BEntry(&ref);
				
				entry.GetPath(&path);
				BString filename = message->FindString("name");
				path.Append(filename);
				
				clog<<"save to "<<path.Path()<<endl;
			}
			
		}
		break;
		
		case B_REFS_RECEIVED:
			clog<<"open requested!"<<endl;
		break;
		
		default:
		BWindow::MessageReceived(message);
	}
}
