**Project Overview: IRC Server Implementation**

### Introduction
This project is a custom implementation of an IRC (Internet Relay Chat) server written in C++. It provides fundamental IRC functionalities, allowing multiple clients to connect, create channels, send messages, and interact through an organized chat environment. Additionally, the server includes role-based user management, channel moderation, and a simple bot feature for interactive gaming.

### Features and Functionalities

#### 1. **Client Management**
- Clients connect to the server using a valid port and password.
- Each client is assigned a unique nickname.
- Registration follows a sequence: the client must send a password, a nickname, and a username to be authenticated.
- Clients can exchange private messages, join and leave channels, and change their nicknames.

#### 2. **Channel Management**
- Clients can create and join channels.
- Operators (moderators) can manage channels by setting access controls, kicking users, and assigning operator roles.
- Channels support invite-only mode, password protection, and user limits.
- Topics can be set and modified by operators.

#### 3. **Server Core Functionality**
- Handles client connections and disconnections.
- Processes and responds to IRC commands such as JOIN, PRIVMSG, NICK, KICK, INVITE, and MODE.
- Uses non-blocking sockets and `poll()` to efficiently handle multiple clients.
- Supports real-time communication with minimal latency.

#### 4. **Error Handling and Responses**
- Implements standard IRC error messages for incorrect commands, invalid usernames, and permission issues.
- Uses predefined response codes to notify clients about the status of their requests.

#### 5. **Security and Access Control**
- Password authentication ensures that only authorized users connect.
- Channel modes restrict access and user permissions.
- Operators have privileges to moderate channels and enforce rules.

#### 6. **Bot Feature: Rock-Paper-Scissors Game**
- Users can challenge each other to a Rock-Paper-Scissors (RPS) game.
- The game operates within a channel, broadcasting results after each round.
- Challenges have a timeout mechanism to prevent stale interactions.

### Implementation Details

#### **Server Class**
- Manages client connections and maintains the list of active users.
- Handles client messages and routes them appropriately.
- Processes IRC commands by parsing input and executing relevant actions.

#### **Client Class**
- Represents an individual user connected to the server.
- Stores client-specific data such as nickname, username, and socket descriptor.
- Handles input buffering and message parsing.

#### **Channel Class**
- Manages a chat room with a list of clients.
- Supports various channel modes (invite-only, password-protected, user limit).
- Broadcasts messages to all channel members.

#### **ChannelManager Class**
- Maintains a list of all active channels.
- Provides utilities for creating, searching, and removing channels.

#### **Bot Functionality**
- Introduces interactive elements such as Rock-Paper-Scissors.
- Uses a challenge-based system where one user challenges another.
- Ensures fairness with timeouts and result broadcasts.

### Running the Server
To start the server, run the executable with two arguments: the port number and the server password:
```
./server <port> <password>
```
The server will listen for incoming connections and process commands accordingly.

### Conclusion
This project is a fully functional IRC server with enhanced user and channel management features. It serves as a foundation for understanding network programming, socket communication, and real-time client-server interactions in C++. The inclusion of a bot feature adds an interactive element, making the server more engaging for users.

