# How interprocess communication will work
* muwm sets up a socket for incoming commands on startup
* muctl sends querries or commands on that socket and receives a reply from muwm
* One command will register a "subscriber" pid with muwm
* Every time something notable changes within muwm, muwm will signal all the subscriber pids with SIGUSR1
* Upon receiving SIGUSR1, the subscribers can use muctl to querry muwm for whatever info they need

# Commands to implement
## Querry commands
* ! how many monitors and what their numbers are
* ! which tags are being viewed for a given monitor
* ! title of the selected window
* ! which layout is active
## Instruction commands
* ! focusstack (change which client is focused)
* ! setmfact
* ! killclient (close selected client)
* ! setlayout
* ! togglefloating
* ! focusmon (change which monitor is focused)
* ! tagmon (send a client to a given monitor)
* tag commands
  * ! view (view a tag)
  * ! toggleview (add a tag to be viewed)
  * ! tag (send client to a given tag)
  * ! toggletag (add a tag to a client)
* add/remove rules
* ! set borderpx
* ! set gappx
* ! set reserved space for bar(s)
* ! add/remove subscriber pids

# Commands
* mons (prev|next)
* title (mon)
* subscribe (pid|reset)
* set
  * mfact (mon)
  * borderpx
  * gappx
  * reserved (mon) (left|right|top|bottom)
  * layout (mon) (layout)
* tags
  * list (mon)
  * view (tag)
  * toggleview (tag)
  * set (tag)
  * toggletag (tag)
  * mon (prev|next)
* focus
  * (prev|next|monprev|monnext)
* killclient
* togglefloating

