#include "Program.h"

int main(int argc, char* argv[])
{
	// type of the mesh / screen width / screen height
	Program program(Program::typeMesh::TRIANGLE, 1280, 720);
	program.run();

}