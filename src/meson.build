
cpp = meson.get_compiler('cpp')
be = cpp.find_library('be')
tracker = cpp.find_library('tracker')
translation = cpp.find_library('translation')

executable('HandNotes', ['main.cpp','ExportWindow.cpp','HandApplication.cpp','HandWindow.cpp','HandView.cpp','Path.cpp','Line.cpp','Arrow.cpp','Measure.cpp','Page.cpp','Disk.cpp'],
	dependencies:[be,tracker,translation]
	)
