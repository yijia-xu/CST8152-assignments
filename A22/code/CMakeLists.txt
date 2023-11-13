#
# CMakeList.txt : CMake project for Compilers, include source and define project specific logic here.
#

#=---------------------------------------=
#|  COMPILERS - ALGONQUIN COLLEGE (F23)  |
#=---------------------------------------=
#
#
#_|          _|_|    _|      _|    _|_|
#_|        _|    _|  _|_|    _|  _|    _|
#_|        _|_|_|_|  _|  _|  _|  _|_|_|_|
#_|        _|    _|  _|    _|_|  _|    _|
#_|_|_|_|  _|    _|  _|      _|  _|    _|
#

cmake_minimum_required (VERSION 3.8)

project ("Lana")

# Add source to this project's executable: check name and dependences
add_executable (Lana
	"Compilers.h"	"Compilers.c"
	"Reader.h"		"Reader.c"		"MainReader.c"
	"Scanner.h"		"Scanner.c"		"MainScanner.c"
	#"Parser.h"		"Parser.c"		"MainParser.c"
	# TO_DO: Add A22 (Scanner) and A32 (Parser)
)

# TO_DO: Add tests and install targets if needed.
