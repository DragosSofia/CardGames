
#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
	int nr;
    char color;
	struct Node *next;
}Node;

typedef struct Deck{
	Node *front;
	Node *rear;
	long size;
}Deck;

Deck* createDeck(void){
	Deck *deck ;
	deck = malloc( sizeof( Deck ) );
	deck -> front = deck -> rear = NULL ;
	deck -> size = 0 ;
	return deck;
} 

void addToFront( Deck *d , int nr , char color )
{   
    Node *aux ;
    if( d == NULL )
        return ;
    if( d -> front == NULL && d -> rear == NULL )
    {
        aux = malloc( sizeof( Node) );
        aux -> color = color ;
        aux -> nr = nr ;
        aux -> next = NULL ;
        d -> front = d -> rear = aux ;
        d -> size ++ ; 
        return ;
    } 

    aux = malloc( sizeof( Node) );
    aux -> color = color ;
    aux -> nr = nr ;
    aux -> next = d -> front ;
    d -> front = aux ;  
    d -> size ++ ;
}

void addToRear( Deck *d , int nr , char color )
{   
    Node *aux ;
    if( d == NULL )
        return ;
    if( d -> front == NULL && d -> rear == NULL )
    {   
        d -> size ++ ;
        aux = malloc( sizeof( Node) );
        aux -> color = color ;
        aux -> nr = nr ;
        aux -> next = NULL ;
        d -> front = d -> rear = aux ; 
        return ;
    } 

    aux = malloc( sizeof( Node) );
    aux -> color = color ;
    aux -> nr = nr ;
    aux -> next = NULL ;
    d -> rear -> next = aux ;
    d -> rear = aux ;  
    d -> size ++ ;
}

void PrintNr( int nr , FILE *out )
{
    if( nr == 15 )
            fprintf( out ,  "A" );
        if( nr == 14 )
            fprintf( out ,  "K" );
        if( nr == 13 )
            fprintf( out ,  "Q" );
        if( nr == 12 )
            fprintf( out ,  "J" );
        if( nr == 10 )
            fprintf( out ,  "T" );
        if( nr >= 2 && nr < 10  )
            fprintf( out ,  "%d" , nr );
}

void PrintDeck( Deck *d , FILE *out )
{   
    if( d == NULL )
        return ;
    Node *aux ;
    for( aux = d -> front ; aux != NULL ; aux = aux -> next )
    {
        PrintNr( aux -> nr , out );
        fprintf( out ,  "%c " , aux -> color ); 
    }
}

void AddNodeToRear( Deck *d , Node * n )
{
    if( d -> front == NULL && d -> rear == NULL )
        {
            d -> front = d -> rear = n ;
            d -> size = 1 ;
            d -> rear -> next = NULL;
        }
    else
        {
            d -> rear -> next = n ;
            d -> rear = n ;
            d -> size ++ ;
            d -> rear -> next = NULL;
        }
}

void AddNodeToFront( Deck *d , Node * n )
{
    if( d -> front == NULL && d -> rear == NULL )
        {
            d -> front = d -> rear = n ;
            d -> size = 1 ;
            d -> rear -> next = NULL;
        }
    else
        {
            n -> next = d -> front ;
            d -> front = n ;
            d -> size ++ ;
        }
}

Node * GetFirstNode( Deck *d )
{  
   Node *aux;  
   if( d -> front == d -> rear )
   {
       aux = d -> front ;
       d -> front = d -> rear = NULL;
       d -> size -- ;
       return aux ;
       
   } 

   aux = d -> front ;
   d -> front = d -> front -> next ;
   d -> size -- ;
   return aux ;
}

int GetTheNrOfNth( Deck *d , int n )
{
    int i ;
    Node *j = d -> front ;
    if( n > d -> size )
        return -1;
    for( i = 1 ; i<n ; i++ )
        j = j -> next ;
    return j -> nr ;
}

void destroyDeck(Deck *d){
	Node * aux ;
	while( d -> front != NULL )
	{
		aux = d -> front ;
		d -> front = d -> front -> next ;
		free( aux );
	}

	free( d );
}