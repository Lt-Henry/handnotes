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

#include "HandNotes.hpp"
#include "HandWindow.hpp"
#include "Disk.hpp"

#include <Application.h>
#include <TranslatorRoster.h>
#include <BitmapStream.h>
#include <Bitmap.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Path.h>
#include <Entry.h>
#include <Directory.h>
#include <File.h>
#include <OS.h>

#include <iostream>

using namespace handnotes;

using namespace std;

HandWindow::HandWindow()
: BWindow(BRect(100, 100, 100 + 720, 100 + 512), "HandNotes", B_TITLED_WINDOW, 0)
{

	BMenuBar* menu = new BMenuBar(BRect(0, 0, Bounds().Width(), 22), "menubar");
	BMenu* menuFile = new BMenu("File");
	
	menu->AddItem(menuFile);
	
	menuFile->AddItem(new BMenuItem("New",new BMessage(Message::MenuNew)));
	menuFile->AddItem(new BMenuItem("Open",new BMessage(Message::MenuOpen)));
	menuFile->AddItem(new BMenuItem("Save",new BMessage(Message::MenuSave)));
	menuFile->AddItem(new BMenuItem("Export",new BMessage(Message::MenuExport)));
	menuFile->AddItem(new BMenuItem("Quit",new BMessage(Message::MenuQuit)));
	
	AddChild(menu);
	view = new HandView(BRect(0,23,100,100));
	AddChild(view);
	
	BMessage *saveMsg = new BMessage(Message::SaveRequest);
	
	savePanel = new BFilePanel(B_SAVE_PANEL, NULL, NULL,
		B_FILE_NODE, false, saveMsg, NULL, true, true);
	
	savePanel->SetTarget(this);
	
	BMessage *openMsg = new BMessage(Message::OpenRequest);
	
	openPanel = new BFilePanel(B_OPEN_PANEL, NULL, NULL,
		B_FILE_NODE, false, openMsg, NULL, true, true);
	
	openPanel->SetTarget(this);

	BMessage *exportMsg = new BMessage(Message::ExportRequest);
	
	exportPanel = new BFilePanel(B_SAVE_PANEL, NULL, NULL,
		B_FILE_NODE, false, exportMsg, NULL, true, true);
	
	exportPanel->SetTarget(this);

	BMenu* menuEdit = new BMenu("Edit");
	menu->AddItem(menuEdit);

	menuEdit->AddItem(new BMenuItem("Un-do",new BMessage(Message::MenuUndo)));
	menuEdit->AddItem(new BMenuItem("Re-do",new BMessage(Message::MenuRedo)));

	BMenu* menuView = new BMenu("View");
	menu->AddItem(menuView);

	menuView->AddItem(new BMenuItem("Home",new BMessage(Message::MenuHome)));
	menuView->AddItem(new BMenuItem("No zoom",new BMessage(Message::MenuNoZoom)));
	menuView->AddItem(new BMenuItem("Zoom in",new BMessage(Message::MenuZoomIn)));
	menuView->AddItem(new BMenuItem("Zoom out",new BMessage(Message::MenuZoomOut)));
	menuView->AddItem(new BMenuItem("Fit page",new BMessage(Message::MenuFitPage)));
	menuView->AddItem(new BMenuItem("Fit drawing",new BMessage(Message::MenuFitDrawing)));

	BMenu* menuTool = new BMenu("Tool");
	menu->AddItem(menuTool);
	
	menuTool->AddItem(new BMenuItem("Pen",new BMessage(Message::MenuPen)));
	menuTool->AddItem(new BMenuItem("Highlighter",new BMessage(Message::MenuHighlighter)));
	
	//exportWindow = new ExportWindow();
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
		case Message::MenuNew:
		break;
		
		//open file
		case Message::MenuOpen:
			openPanel->Show();
		break;
		
		//save file
		case Message::MenuSave: 
			savePanel->Show();
		break;
		
		case Message::MenuExport: {
				exportWindow = new ExportWindow(this);
				exportWindow->Show();
			}
		break;
		
		case Message::MenuQuit:
			be_app->PostMessage(B_QUIT_REQUESTED);
		break;
		
		case Message::SaveRequest: {
			
			entry_ref ref;
			if (message->FindRef("directory", 0, &ref) == B_OK) {
				BPath path;
				BEntry entry = BEntry(&ref);
				
				entry.GetPath(&path);
				BString filename = message->FindString("name");
				path.Append(filename);
				
				clog<<"save to "<<path.Path()<<endl;
				
				BEntry dest(path.Path());
				io::SavePage(&dest,view->page);
			}
			
		}
		break;
		
		case Message::OpenRequest: {
				clog<<"open requested!"<<endl;
				
				entry_ref ref;
				message->FindRef("refs", 0, &ref);
				BEntry entry(&ref, true);
				BPath path;
				entry.GetPath(&path);
				clog<<"path:"<<path.Path()<<endl;
				Page* page = io::LoadPage(&entry);
				//TODO: fix this hack
				if (page) {
					view->page = page;
				}
			}
		break;

		case Message::ExportRequest: {
			clog<<"HandWindow received an export message"<<endl;
			float dpi = message->GetFloat("dpi",96);
			clog<<"DPI:"<<dpi<<endl;
			BBitmap* bitmap = new BBitmap(view->page->PixelBounds(96.0),B_RGBA32, true, true);
			BView* eView = new BView(view->page->PixelBounds(96.0),"export",B_FOLLOW_NONE,B_WILL_DRAW);
			bitmap->AddChild(eView);
			
			eView->LockLooper();
			view->page->Draw(eView);
			eView->Sync();
			eView->UnlockLooper();
			BFile file("/boot/home/hnotes.png",B_WRITE_ONLY | B_CREATE_FILE);
			BBitmapStream stream(bitmap);
			BTranslatorRoster* roster = BTranslatorRoster::Default();
			roster->Translate(&stream, NULL, NULL, &file, B_PNG_FORMAT, B_TRANSLATOR_BITMAP);
			
			stream.DetachBitmap(&bitmap);
		}
		break;
		
		case Message::ExportClose:
			
		break;
		
		case Message::MenuHome:
			view->Home();
		break;
		
		case Message::MenuNoZoom:
			view->NoZoom();
		break;
		
		case Message::MenuZoomIn:
			view->ZoomIn();
		break;
		
		case Message::MenuZoomOut:
			view->ZoomOut();
		break;
		
		case Message::MenuFitPage:
			view->ZoomFitPage();
		break;
		
		case Message::MenuFitDrawing:
			view->ZoomFitDrawing();
		break;
		
		default:
		BWindow::MessageReceived(message);
	}
}
