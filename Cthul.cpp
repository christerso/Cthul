// Chtuhl.cpp : Defines the entry point for the application.
//

#include "Cthul.h"

#include "ResourceManager.h"
#include "SDL2Wrapper.h"

using namespace RM;


void Cthul::setup()
{
	rm.load_image("Adam", "resources/images/human.png");
}
