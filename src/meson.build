
cpp = meson.get_compiler('cpp')
be = cpp.find_library('be')

executable('HandNotes', ['main.cpp','HandApplication.cpp','HandWindow.cpp','HandView.cpp','Path.cpp'],
	dependencies:[be]
	)
