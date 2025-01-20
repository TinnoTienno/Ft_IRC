# ft_irc

## Overview
`ft_irc` is a simple IRC (Internet Relay Chat) server implementation written in C++. It is designed to comply with the RFC 2812 standard and provide basic IRC functionalities.

## Features
- User authentication and registration
- Channel creation and management
- Private messaging
- Basic IRC commands (JOIN, PART, PRIVMSG, etc.)
- Multi-client support

## Installation
To install and run `ft_irc`, follow these steps:

1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/ft_irc.git
    ```
2. Navigate to the project directory:
    ```sh
    cd ft_irc
    ```
3. Build the project:
    ```sh
    make
    ```


## Usage

1. Start the server:
    ```sh
    ./ircserver <port> <password>
    ```
    - `<port>`: The port number on which the server will listen.
    - `<password>`: The password required for clients to connect to the server.

2. Connect to the server using an IRC client:
    - Use Irssi IRC client to connect to the server using the specified port and password.


## Commands

### JOIN
- Syntax: `JOIN <channel>{,<channel>} [<key>{,<key>}]`
- Description: Joins the specified channel(s).

### PART
- Syntax: `PART <channel>{,<channel>}`
- Description: Leaves the specified channel(s).

### PRIVMSG
- Syntax: `PRIVMSG <target>{,<target>} <message>`
- Description: Sends a private message to the specified target(s).

### NOTICE
- Syntax: `NOTICE <target>{,<target>} <message>`
- Description: Sends a notice to the specified target(s).

### NICK
- Syntax: `NICK <nickname>`
- Description: Changes the client's nickname.

### MODE
- Syntax: `MODE <channel> {[+|-]|o|i|t|k|l|b} [<limit>|<user>|<banmask>]`
- Description: Changes the mode of a channel or user.

### KICK
- Syntax: `KICK <channel> <user> [<comment>]`
- Description: Removes the specified user from the channel.

### INVITE
- Syntax: `INVITE <nickname> <channel>`
- Description: Invites the specified user to the channel.

### QUIT
- Syntax: `QUIT [<message>]`
- Description: Disconnects the client from the server.

### PING
- Syntax: `PING <server>`
- Description: Checks the connection between the client and server.

## Contributing
Contributions are welcome! Please fork the repository and submit a pull request with your changes.

## Authors

- aduvilla@student.42.fr
- eschussl@student.42.fr