# SFML_Game_Development

## Installing SFML
1. Go to http://www.sfml-dev.org/download/sfml/2.3.2 and download SFML for OSX
2. Extract it
3. Copy the contets of Frameworks to /Library/Frameworks (the SFML library)
4. Copy the contents from extlibs to /Library/Frameworks (SFML external library dependencies)
5. Copy the SFML directory from templates to /Library/Developer/Xcode/Templates (create the folders if they don't exist yet). Xcode SFML templates for quick project setup.

## Xcode
1. Create new project in XCode - and choose the "SFML App" template
2. Make sure the SFML binaries are set to "Frameworks" since we installed SFML as frameworks in the "Installing SFML" steps.
3. Make sure the C++ Compiler and Standard Library is set to C++11 with Clang and libc++
4. Compile and run the skeleton template.

## Upto Page 113
