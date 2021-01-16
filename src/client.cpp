#include "client.hpp"

// Set up what we can here, rely on XorgConnection to
// fill in the rest
Client::Client(Window w, XWindowAttributes* wa) : win{w} {

