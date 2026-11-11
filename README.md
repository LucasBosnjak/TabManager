# TabManager
A tab manager which stores and handles user data and inputs, and has admin features.


# Inshort
A user management application built with C++ and Raylib that allows administrators to manage user balances through an intuitive graphical interface. User data is persisted to a local file, enabling balance tracking across sessions.

User information is stored in a local file: numbers.txt


# Application Workflow
Users can:

Select an account from the user list.

Choose predefined amounts using quick-add buttons.

Stage balance changes before applying them.

Confirm or reset pending adjustments.


Administrative (people who have the password) users can:

Create new user accounts.

Remove user accounts.

Add custom balance amounts.

Subtract custom balance amounts.

Manage all stored accounts.

Authentication

Current default password: admin


# Prerequisites for use!
C++17 compatible compiler

Raylib library installed

Required project files:

main.cpp

matikka.h

matikka.cpp

Using g++, if raylib is installed in default location:
g++ main.cpp matikka.cpp -o Manager.exe -lraylib -lopengl32 -lgdi32 -lwinmm

# User Interface / UI
The application includes:

User list panel

Quick-add monetary buttons

Manager login button

Administrative management console

Confirmation modals

Context-sensitive help screen

Success notification highlighting

Controls

Scroll user list	Mouse wheel

# Future Improvements / Coming soon!
Secure password hashing and storage

Multiple types of administrator accounts

User roles and permissions

Transaction history logging

Audit trail for account modifications

# Author: Lucas Bosnjak
