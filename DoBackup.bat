cd ..
del TTTTTGGL.zip
rmdir /S /Q TTTTTGGL
mkdir TTTTTGGL


xcopy TAGGL\*.* TTTTTGGL\TAGGL\ /S /E
cd TTTTTGGL
cd TAGGL
call CleanProjectFullyWithAttrib.bat
