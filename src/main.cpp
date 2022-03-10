// Support Code written by Michael D. Shah
// Last Updated: 1/21/17
// Please do not redistribute without asking permission.

// Functionality that we created
#include "SDLGraphicsProgram.hpp"
#include "Object.hpp"

int main(int argc, char** argv){

	// Create an instance of an object for a SDLGraphicsProgram
	SDLGraphicsProgram mySDLGraphicsProgram(1280,720);

	std::string fileName(argv[1]);
	mySDLGraphicsProgram.LoadOBJ(fileName);

	// Run our program forever
	mySDLGraphicsProgram.Loop();
	// When our program ends, it will exit scope, the
	// destructor will then be called and clean up the program.
	std::cout.flush();
	return 0;
}
