
cpp = meson.get_compiler('cpp')
be = cpp.find_library('be')
tracker = cpp.find_library('tracker')

executable('HandNotes', ['main.cpp','ExportWindow.cpp','HandApplication.cpp','HandWindow.cpp','HandView.cpp','Path.cpp','Page.cpp','Disk.cpp'],
	dependencies:[be,tracker]
	)
