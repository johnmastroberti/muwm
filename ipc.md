# How interprocess communication will work
* muwm sets up a socket for incoming commands on startup
* muctl sends querries or commands on that socket and receives a reply from muwm
* One command will register a "subscriber" pid with muwm
* Every time something notable changes within muwm, muwm will signal all the subscriber pids with SIGUSR1
* Upon receiving SIGUSR1, the subscribers can use muctl to querry muwm for whatever info they need
