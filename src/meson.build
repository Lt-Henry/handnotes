
cpp = meson.get_compiler('cpp')
be = cpp.find_library('be')
tracker = cpp.find_library('tracker')

executable('HandNotes', ['main.cpp','HandApplication.cpp','HandWindow.cpp','HandView.cpp','Path.cpp','Page.cpp'],
	dependencies:[be,tracker]
	)
