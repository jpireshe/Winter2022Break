This is an exercise focused not in making a game, but rather in making it playable online using only C.
The ttt files run a server (using the port you specify), and you can connect to the running program using netcat.
run it with these commands:

make
./tictactoe [PORT]

[NEW TERMINAL]
nc localhost [PORT] (if local)
OR: nc [IP] [PORT]
