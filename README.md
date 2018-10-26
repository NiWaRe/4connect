# 4connect
A GUI-4-Connect game with multiplayer capabilities (tcp and udp) based on Qt

# Getting Started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

# Installing
You will need <a href="https://www.qt.io/download">QT Creator</a> to build a playable Application Window on your chosen OS.

# How to play
1. Find an opponent
Select if you want to start the game as Server or Client by selecting either the radio-button: server or client. 
_Optional (Not yet implemented in GUI) use the broadcast classes (**netbroadcast.cpp/hpp**) to implement a simple button (the same on server and client) 
which automatically triggers a UDP-Broadcast-Listener and -Sender at the same time and enables automatic detection of an opponent in the local network_ 

* Server Mode: Enter port number in the port field
* Client Mode: Enter IP address and port number of the server you want to connect with
`IP address: localhost
 Port: 1234`

2. *Click "Start Game" and _Play the 4c-Game and optionally chat with your opponent!_

# Built With
* C++ - Used for backend
* QT5 - The GUI and TCP/UPD toolkit used

### Authors
* Oleksii Khakhlyuk - _Logic_
* Sherif Ashraf Shousha - _GUI_
* Nicolas Remerscheid - _Network_

# Acknowledgments
This project is part of the course Blockpraktikum C++ offered by the Chair of Security in Information Technology at the Technical University of Munich.
We want to thank everyone responsible for this course giving us the opportunity to learn an object oriented programming language.
_READ-ME with Henry He._
