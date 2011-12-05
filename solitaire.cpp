//test deck
//card and pile data structures

#include <cstdlib>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <string>

#define null 0
#define charOffset 48

using namespace std;

string errorString;
int finish;
// card is a node for the linkedlist pile
class card {
    private :
        char suit;// card suit, valid suits=[H,D,C,S]
        int rank;// card rank, valid cards=[1(A),2,3,4,5,6,7,8,9,10,11(J),12(Q),13(K)]
        
        public:
            
            //debug char onPile; //[X=none,A=Active,C=Completed,D=Deck]
            card* next; // if next==null, at top of pile
            card* prev;
            //card* prev; // if next==null, at bottom of pile
            bool flipped;// whether card value or --- is displayed (used in blackjack and solitaire/patience)
            
            //instantiates card of certain rank and suit
            //card(void){};
            card(int _rank,char _suit){
                rank=_rank;
                suit=_suit;
                flipped=1;
                next=NULL;
                prev=NULL;
            }
            
            
            //set vars
            //void setSuit(char x){suit=x;}
            void setFlipped(bool x){flipped=x;}
            
            bool isRed(){
                if (suit=='H'||suit=='D')return 1;
                else return 0;
            }
            char getSuit(){return suit;}
            int getRank(){return rank;}
            
            //get string representation of card
            string toString() {
                string ret;
                if(flipped){
                    switch (rank){
                        case 1: ret=" A"; break; // Rank = A
                        case 2: ret=" 2"; break; // Rank = 2
                        case 3: ret=" 3"; break; // Rank = 3
                        case 4: ret=" 4"; break; // Rank = 4
                        case 5: ret=" 5"; break; // Rank = 5
                        case 6: ret=" 6"; break; // Rank = 6
                        case 7: ret=" 7"; break; // Rank = 7
                        case 8: ret=" 8"; break; // Rank = 8
                        case 9: ret=" 9"; break; // Rank = 9
                        case 10: ret="10"; break; //Rank = 10
                        case 11: ret=" J"; break; //Rank = J
                        case 12: ret=" Q"; break; //Rank = Q
                        case 13: ret=" K"; break;//Rank = K
                    }
                    return ret+""+suit+" ";
                }
                else {
                    return "--- ";
                }
            }
};
//pile is a superclass card node handler, implementing different functions
//to access different linkedlist 'piles':
//active,completed and deck
//methods are overridden when neccessary in the various implementation.
class pile {
    public:
        card* head;
        //card* grabbed;
        char type;
        //assign null to pile vals
        pile(){
            head=null;
            type= 'X';
        }
        pile(char _type){
            pile();
            type= _type;
        }
        // returns size of pile
        int size(){
            if (this->head){
                card* current=this->head;
                int temp=1;
                while (current && current->next){
                    temp++;
                    current=current->next;
                }
                return temp;
            }
            else return 0;
        }
        
        //adds new card to pile
        void add(int rank,char suit){
            card* newbie=new card(rank,suit);
            //if pile is !empty, add newbie to head of pile
            if (this->head) {
                newbie->next=this->head;
                this->head->prev=newbie;
                this->head=newbie;
                newbie->prev=null;
            }
            //else start pile with newbie
            else      {
                newbie->next=null;
                newbie->prev=null;
                this->head=newbie;
            }
        }
        /// newbie...head................tail.
        void add(card* newbie){
            if(newbie->next)newbie->next->prev=null;//set newbies nexts cards prev pointer to null
            if (this->head)this->head->prev=newbie;//set old heads prev pointer to newbie
            
            newbie->next=this->head; //set newbies next pointer to oldhead
            newbie->prev=null;//set newbies prev to null
            
            this->head=newbie;//set head pointer of pile to newbie
        }
        /// null..newbiehead......newbie->prev....newbie...head................tail.
        void addStack(card* newbie){
            //find the head of the stack to move
            card* newbiehead=newbie;
            while (newbiehead->prev){
                newbiehead=newbiehead->prev;
            }
            
            if(newbie->next)newbie->next->prev=null;//set newbies nexts cards prev pointer to null
            if (this->head)this->head->prev=newbie;//set old heads prev pointer to newbie
            
            newbie->next=this->head; //set newbies next pointer to oldhead
            
            this->head=newbiehead;//set head pointer of pile to newbie
        }
        //checks for valid move, and if valid, moves.
        //preconditions, from.type [T,1,2,3,4,5,6,7] to.type [1,2,3,4,5,6,7,H,D,C,S]
        bool moveTo(pile* to){
            card* newbie=this->head;
            bool valid=0;
            char totype=to->type;
            if(!this->head)return 0;//throw invalid if from pile is empty
            switch (this->type){
                //'from' is deck.
                case 'T':
                    if (totype=='A'){
                        //move deck to active
                        //if to pile is not empty
                        if(to->head){
                            //valid move if from card is 1 less than to card, and different color
                            if (this->head->getRank()+1==to->head->getRank()                && this->head->isRed()!=to->head->isRed())valid=1;
                        }
                        else{
                            //if to pile is empty, check if card is king
                            if (this->head->getRank()==13)valid=1;
                        }
                    }
                    else if (totype=='T'){
                        //move deck to deck
                        errorString +="Cannot move deal to deck. try deal command.";
                    }
                    else if (totype==this->head->getSuit()){//if dest is completed pile and also same suit
                        //if to pile is not empty
                        if(to->head){
                            //valid move if from card is 1 more than to card
                            if (this->head->getRank()==to->head->getRank()+1 )valid=1;
                        }
                        else{
                            //if to pile is empty, check if card is ace
                            if (this->head->getRank()==1)valid=1;
                        }
                    }
                    
                    
                    //if valid move from deck
                    if(valid){
                        if(newbie->next)this->head=newbie->next;//move head pointer along
                        to->add(newbie);
                    }
                    else {
                        errorString +="\nInvalid Move!";
                    }
                    break;
                    
                    //'from' is active.
                case 'A':
                    if (totype=='A'){
                        //move active to active
                        //if to pile is not empty
                        if(to->head){
                            while (newbie && !valid && newbie->flipped){
                                //valid move if from card is 1 less than to card, and different color
                                if (newbie->getRank()+1==to->head->getRank()                && newbie->isRed()!=to->head->isRed()){
                                    valid=1;
                                    break;
                                }
                                newbie=newbie->next;
                            }
                        }
                        else{
                            while (newbie && !valid){
                                //if to pile is empty, check if card is king
                                if (newbie->getRank()==13){
                                                           valid=1;
                                                           break;
                                                           }
                                newbie=newbie->next;
                            }
                        }
                    }
                    else if (totype=='T'){
                        //move active to deck
                        errorString +="Cannot move Active to deck";
                    }
                    else if (totype==this->head->getSuit()){//if dest is completed pile and also same suit
                        //if to pile is not empty
                        if(to->head){
                            //valid move if from card is 1 more than to card
                            if (this->head->getRank()==to->head->getRank()+1 )valid=1;
                        }
                        else{
                            //if to pile is empty, check if card is ace
                            if (this->head->getRank()==1)valid=1;
                        }
                    }
                    //if valid move from deck
                    if(valid){
                        if(newbie){
                                         this->head=newbie->next;//move head pointer along
                                         if (this->head)this->head->setFlipped(1);
                                         }
                        else {
                             this->head=null;
                             }
                        to->addStack(newbie);
                    }
                    else {
                        errorString +="\nInvalid Move!";
                    }
                    break;
                    
                    //invalid from
                default: cout << "Invalid from pile!"; return 0;
            }
        }
        
        //default show, used only in deck pile, active and completed overide with custom show methods
        string show(){
            if (head) return head->toString();
            else return " .  ";
        }
        //deletes nodes/cards
        void burn(){
             while (head && head->next){
                   head=head->next;
                   delete head->prev;
                   }
                   delete head;
                   head=null;
             }
};
class completedPile: public pile {
    public:
        completedPile(){pile();}
        string show(){
            if (head) return head->toString();
            else {
                string ret=" -";
                ret+=this->type;
                return ret+" ";
            }
        }
        
};
class activePile: public pile {
    public:
        card* disp;//used for display algorithim
        activePile(){pile();type='A';disp=null;}
        //overides pile show
        string show(){
            card* tail=this->head;
            //navigate to tail
            while (tail && tail->next){
                tail=tail->next;
            }
            disp=tail;
            if (disp) return disp->toString();
            else return "    ";
        }
        //new method for displaying active piles
        string  showNext(){
            if(disp && disp->prev) {
                disp=disp->prev;//moves from tail to head to print on display
                return disp->toString();
            }
            else {
                return "    ";
            }
        }
};
class deckPile: public pile {
    public:
        deckPile(){pile();}
        void deal(){
            //if deck not empty and deck.next exists
            if (this->head && this->head->next){
                card* current=head;
                //move to tail of list
                while (current->next){
                    current=current->next;
                }
                current->next=head;//set tails next to head
                head->prev=current;//set heads prev to tail
                
                head=head->next;//move the head pointer of the list along to next node
                
                current->next->next=null;//sever the new bottom cards list link.*/
                current->next->prev=current;//set the new bottom cards prev to the prev bottom card
            }
            else errorString+="\nLast Card in Deck!";
        }
        
};
class solitaireWorld {
    //create deck , 4 completed piles, and 7 active piles
    
    public:
        activePile active[7];
        deckPile deck;
        completedPile completed[4];
        
        //create world
        solitaireWorld() {
            // instantiate decks
            deck.type='T';/*
            for (int i=0;i<7;i++){
                active[i].type=charOffset+i+1;
            }*/
            
            completed[0].type='H';
            completed[1].type='D';
            completed[2].type='C';
            completed[3].type='S';
            
        }
        void burn() {
            // delete decks
            deck.burn();
            for (int i=0;i<7;i++){
                active[i].burn();
            }
            for (int i=0;i<4;i++){
                completed[i].burn();
            }
            
        }
        void help(){
            //cls
            cout << "\nCommands:";
            cout << "\nHELP ";
            cout << "\n      displays this screen";
            cout << "\n";
            cout << "\nDEAL ";
            cout << "\n      deals the next card from the deck!";
            cout << "\n";
            cout << "\nMOVE [source] TO [destination] ";
            cout << "\n      makes a move! source=[T1234567], destination=[T1234567HDCS]";
            cout << "\n";
            cout << "\nLOAD [followed by between 28 and 52 unique card labels ]*";
            cout << "\n       Loads a preset game";
            cout << "\n";
            cout << "\nQUIT ";
            cout << "\n       exit the game ";
            
        }
        void deal(){
            //deal next card. if nothing is done, add an error to global errorstring.
            deck.deal();
            //if(!deck.deal()) errorString+="\nLast card in Deck!";
        }
        void load(card* loadThese[],int amount){
            //iterate through list, dealing cards horizontally onto the active field
            this->burn();
            int startpile=0;
            int currpile=0;
            for (int i=0;i<28;i++){
                if (currpile>6) {
                    startpile++;//start deal at next deck along
                    currpile=startpile;//move to next start deck
                }
                else {
                     loadThese[i]->setFlipped(0);//flip card over
                }
                if(i==0)loadThese[i]->setFlipped(1);
                active[currpile].add(loadThese[i]);
                //debug stuff cout << "\nCard:"<<loadThese[i]->toString()<<" Pile:"<<currpile<<"!";
                currpile++;//deal to next horizontal pile
            }
            for (int i=amount-1;i>27;i--){
                //add cards 52..51..50......28 to deck
                deck.add(loadThese[i]);
            }
        }
        void quit(){
            finish=1;
        }
        void paint(){
            // cls;
            cout << "\n PatienC++ by Nathan Dunn.© 2005";
            cout << "\n ______________________________";
            cout << "\n|                              |";
            cout << "\n| Deck   T     H   D   C   S   |";
            cout << "\n| |";
            //format deck size into 2 positions only
            
            if (deck.size()>9){
                cout << deck.size();
            }
            else {
                
                cout << " " << deck.size();
            }
            //show deck and completed piles
            cout << "|  "<< deck.show() << "  " << completed[0].show()<< completed[1].show();
            cout << completed[2].show() << completed[3].show() << " |";
            cout << "\n|______________________________|";
            cout << "\n|                              |";
            
            // cout << "\n| Active piles:                |";
            cout << "\n|  1   2   3   4   5   6   7   |";
            int lines=1;
            
            //find amount of lines needed to print
            for (int i=0;i<7;i++){
                if (active[i].size()>lines) {
                    lines=active[i].size();
                }
            }
            //print top row of active data
            cout << "\n| ";
            for (int i=0;i<7;i++){
                cout << active[i].show();
            }
            //print next rows of active data
            for (int line=1;line<lines;line++){
                cout << " |\n| ";//new line
                for (int i=0;i<7;i++){
                    cout << active[i].showNext();//show next card of all active decks
                }
            }
            
            
            cout << " |\n|______________________________|\n";
            cout << errorString;
            errorString="";
            checkwin();
        }
        void checkwin() {
             bool win=1;
             for (int i=0;i<7;i++){
                 if(active[i].size()>0)win=0;break;
                 }
             if(deck.size()>0)win=0;
             if (win==1){
                        cout <<"\n\n\n\n     CONGRATULATIONS!!!!\n    You Have Won the game!\n\n\n\n";
                        quit();
                        }
             }
};
class parseSolitaire {
    public:
        parseSolitaire() {}
        
        void parse(string input,solitaireWorld* world){
            string data="";
            
            //convert everything to caps
            //cout << input.size() << "\n";
            for (int i=0;i<input.length();i++){
                data+=toupper(input[i]);//converts by char
            }
            data+=" ";//prevents nullpointer crashes when looking at next char
            int len=data.length()-1;//puts data.length into an int variable to speed up execution
            // cout << "\\converted "<< input <<" to "<<data;
            //nextWord();
            string word=data.substr(0,4);
            if (word=="HELP"){
                world->help();
            }
            else if (word=="DEAL"){
                world->deal();
                world->paint();
            }
            else if (word=="LOAD"){
                //erase all piles
                //i designates char operating on
                char current;
                int rank;
                int term;
                card* loadThese[52];
                term=0;
                
                for (int i=4;i<len;i++) {
                    current=data.at(i);
                    rank=0;
                    switch(current){
                        case 'A': rank=1; break; // Rank = A
                        case '2': rank=2; break; // Rank = 2
                        case '3': rank=3; break; // Rank = 3
                        case '4': rank=4; break; // Rank = 4
                        case '5': rank=5; break; // Rank = 5
                        case '6': rank=6; break; // Rank = 6
                        case '7': rank=7; break; // Rank = 7
                        case '8': rank=8; break; // Rank = 8
                        case '9': rank=9; break; // Rank = 9
                        case 'J': rank=11; break; //Rank = J
                        case 'Q': rank=12; break; //Rank = Q
                        case 'K': rank=13; break;//Rank = K
                        case ' ': break;//Rank = K
                        default: cout << "\nInvalid rank:"<< current<<" at position:"<< i-1 ;
                        
                        case '1':
                            
                            //if (i+1<len) {
                            i++;
                            current=data.at(i);
                            if (current=='0'){
                                rank=10; break; //Rank = 10
                            }
                            //handles error if invalid rank 1X,
                            else {
                                cout << "\nExpected rank '10' instead of '"<< current << "' at position:"<< i-1;
                                //try to work around user error.
                                if (current=='H' || current=='D' || current=='C' || current=='S'){
                                    cout <<". Assuming A was meant.";
                                    i--;
                                    rank=1;break;
                                }
                                else if (current=='1') {
                                    cout <<". Assuming J was meant.";
                                    i--;
                                    rank=11;break;
                                }
                                else if (current=='2') {
                                    cout <<". Assuming Q was meant.";
                                    i--;
                                    rank=12;break;
                                }
                                else if (current=='3') {
                                    cout <<". Assuming K was meant.";
                                    i--;
                                    rank=13;break;
                                }
                                else  {
                                    i--;break;
                                }
                                // stop trying to work around user error
                            }
                            //}
                            
                    }
                    //if valid rank found
                    if (rank) {
                        i++;
                        //if (i<len){
                        current=data.at(i);
                        //if valid suit
                        if (current=='H' || current=='D' || current=='C' || current=='S') {
                            //cout << "\n card " << rank << current;
                            loadThese[term]=new card(rank,current);//create a new card
                            term++;//increment card count
                        }
                        else {
                            cout << "\nInvalid suit:"<< current<<" at position:"<< i-1 ;
                        }
                        //}
                    }
                }
                if (term < 28){
                    cout << "\nPlease enter MORE cards than "<<term<<", amount of cards can only be from 28-52.";
                }
                else if (term >52) {
                    cout << "\nPlease enter LESS cards than "<<term<<", amount of cards can only be from 26-52.";
                }
                else {
                    // load cards into world
                    world->load(loadThese,term);
                    world->paint();
                }
            }
            else if (word=="MOVE"){
                pile* from;
                pile* to;
                from=null;
                to=null;
                bool pastToDelim;
                char current;
                //
                for (int i=4;i<len;i++){
                    current=data.at(i);
                    //find from
                    if(!from){
                        switch(current){
                            case 'T': from=&world->deck;break;
                            case '1': from=&world->active[0];break;
                            case '2': from=&world->active[1];break;
                            case '3': from=&world->active[2];break;
                            case '4': from=&world->active[3];break;
                            case '5': from=&world->active[4];break;
                            case '6': from=&world->active[5];break;
                            case '7': from=&world->active[6];break;
                            case ' ': break;//ignore spaces
                            default: errorString+="\nInvalid FROM parameter in move command:";errorString+=current;break;
                        }
                    }
                    else if(!to) {
                        //move past 'TO'
                        if(pastToDelim){
                            //find destination pile
                            switch(current){
                                case '1': to=&world->active[0];break;
                                case '2': to=&world->active[1];break;
                                case '3': to=&world->active[2];break;
                                case '4': to=&world->active[3];break;
                                case '5': to=&world->active[4];break;
                                case '6': to=&world->active[5];break;
                                case '7': to=&world->active[6];break;
                                case 'H': to=&world->completed[0];break;
                                case 'D': to=&world->completed[1];break;
                                case 'C': to=&world->completed[2];break;
                                case 'S': to=&world->completed[3];break;
                                case ' ': break;//ignore spaces
                                default: break;
                            }
                        }
                        //if 'TO' is the next term (check data length to prevent crash from
                        if (data.at(i)=='T' && data.at(i+1)=='O') {
                            pastToDelim=1;
                            i++;//flag that delimiter has been found and move along)
                        }
                        
                    }
                }
                //if validly parsed move, perform the move
                if(from && to ){
                    //cout << "Moved from:"<< from->head->toString() << " to:" << to->head->toString();
                    from->moveTo(to);
                }
                world->paint();
            }
            else if (word=="QUIT"||word=="EXIT"){
                world->quit();
            }
            else {
                cout << "\nInvalid Command!";
            }
        }
};
int main(void) {
    solitaireWorld world;
    parseSolitaire parser;
    string input;
    //load default deck
    // parser.parse("load KS KC KD KH QS QC QD QH JS JC JD JH 10S 10C 10D 10H 9S 9C 9D 9H 8S 8C 8D 8H 7S 7C 7D 7H 6S 6C 6D 6H 5S 5C 5D 5H 4S 4C 4D 4H 3S 3C 3D 3H 2S 2C 2D 2H AS AC AD AH",&world);
    parser.parse("load KS KC KD KH QS QC QD 2C JS JC JD JH 10S 10C 10D 10H 9S 9C 9D 9H 8S 8C 8D 8H 7S 7C 7D 7H 6S 6C 6D 6H 5S 5C 5D 5H 4S 4C 4D 4H 3S 3C 3D 3H 2S 2C 2D 2H AS AC AD AH",&world);

    while (finish==0){
        
        //get next command
        cout << "\n?";
        getline(cin,input);
        parser.parse(input,&world);//pass control to the parser (no pun intended)
        input = "";
    }
    
    cout << "\n\nBye!";
};

