#PatienC++
This is a simple, text based version of Solitaire.

##Overview
The object of the game is to empty the deck. Cards may be removed from the T (Thrown) Pile to a playing area pile numbered 1-7 with the 'MOVE' Command. For example, to move a card from the Thrown pile to the the first playing area pile the player would type 'MOVE T TO 1'. Cards may be stacked when possible in the Suit decks (H,D,C,S) in ascending order in the appropriate pile in order to clear them from the playing areas.

Once the bottom card is removed from a playing area pile, the next card in the pile will be revealed.

Red
H = Hearts
D = Diamonds

Black
C = Clubs
S = Spades

##Commands
HELP
      displays the help screen

DEAL
      deals the next card from the deck!

MOVE [source] TO [destination]
      makes a move! source=[T1234567], destination=[T1234567HDCS]

LOAD [followed by between 28 and 52 unique card labels ]*
       Loads a preset game

QUIT
       exit the game

##Software Architecture
compiled with g++
solitaire.exe = win32 executable
solitaire = unix executable
solitaire.cpp = source

piles are doubly linked lists, cards are nodes

deckPile:
tail(invisible)
.
.
head(top)

completedPile:
tail(invisible)
.
.
head(top)

activePile:
tail(top)
.
.
head(bottom)