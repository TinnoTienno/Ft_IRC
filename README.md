# ft_irc

## Overview
`ft_irc` is a simple IRC (Internet Relay Chat) server implementation written in C++. It is designed to comply with the RFC 2812 standard and provide basic IRC functionalities.

## Features
- User authentication and registration
- Channel creation and management
- Private messaging
- Basic IRC commands (JOIN, PART, PRIVMSG, etc.)
- Multi-client support
- **Bot**: A file transfer bot that can list and send files, refresh the file list, and kick users for banned words.

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

### Running the Server

1. Start the server:
    ```sh
    ./ircserver <port> <password>
    ```
    - `<port>`: The port number on which the server will listen.
    - `<password>`: The password required for clients to connect to the server.

2. Connect to the server using an IRC client:
    - Use Irssi IRC client to connect to the server using the specified port and password.

### Running the Bot

1. Start the bot:
    ```sh
    ./bot <server_address> <channel> <password> <port>
    ```
    - `<server_address>`: The address of the IRC server.
    - `<channel>`: The channel to join.
    - `<password>`: The password for the server.
    - `<port>`: The port number of the server.

2. The bot supports the following commands:
    - `!list`: Display the list of transferable files.
    - `!send <filename>`: Transfer the specified file.
    - `!refresh`: Refresh the list of handled files.

### File Transfers with `nc` and `irssi`

#### Using `irssi` for File Transfers

- **Send a file**:
    ```sh
    /dcc send [user] [complete/path/file]
    ```

- **Receive a file**:
    ```sh
    /dcc get [user] [file]
    /dcc get -n [newFileName] -d [path/to/paste/dir] [user] [file]
    ```

#### Using `nc` (Netcat) for File Transfers

- **Send a file**:
    ```sh
    nc [Ipaddress] [port] < [complete/path/file]
    ```

- **Receive a file**:
    ```sh
    nc -l [port] > [new/path/and/filename]
    ```
    
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