Deck* IntercalareDeck( Deck *d  )
{
    Deck *aux = createDeck() ;
    Node *i = d -> front , *j = d -> front ;
    int k ;
    for( k = 1 ; k <= d -> size / 2 ; k++)
        j = j -> next ;
    for( k = 1 ; k <= d -> size / 2 ; k++)
    {
        addToRear( aux , i -> nr , i -> color );
        addToRear( aux , j -> nr , j -> color );
        i = i -> next ;
        j = j -> next ;
    } 
    destroyDeck( d );
    return aux ;     
}


Deck* MongeanDeck( Deck *d  )
{      
    Deck *aux = createDeck() ;
    Node *i = d -> front ;
    int k;
    for( k = 1 ; k <= d -> size / 2 ; k++)
    {
        addToRear( aux , i -> nr , i -> color );
        i = i -> next ;
        addToFront( aux , i -> nr , i -> color );
        i = i -> next ;
        
    }     
    destroyDeck( d );
    return aux ;    
}

Deck* NpilesDeck( Deck *d , FILE *in )
{
  Deck *Piles[ 53 ]  ;
  Node *poz;
  int n , i , j  , a[ 53 ];
  fscanf( in , "%d" , &n );
  for( i = 1 ; i <= n ; i++ )
      fscanf( in , "%d" , &a[ i ] );
      
  for( i = 1 ; i <= n ; i++ )  
    Piles[ i ] = createDeck();
   
   poz = d -> front ;
   for( i = 1 ; i <= n ; i++ )
    for( j = 1 ; j<= d -> size / n ; j++ )
        {
         
          addToRear( Piles[ i ] , poz -> nr , poz -> color );
          poz = poz -> next ;  
          
        } 
  
   destroyDeck( d );
   d = createDeck();
   for( i = 1 ; i <= n ; i++ )  
   {
     poz = Piles[ a[ i ] ] -> front ;
     while( poz != NULL )
         {
            addToRear( d , poz -> nr , poz -> color );
            poz = poz -> next; 
         } 
   }
  for( i = 1 ; i <= n ; i++ ) 
    destroyDeck( Piles[ i ] );
  return d;  
}

void invertDeck( Deck *d )
{
    Node *curr = d -> front , *prev = NULL  , *next = NULL , *aux;
    while( curr != NULL )
    {
        next = curr -> next ;
        curr -> next = prev ;
        prev = curr ;
        curr = next ;
    }

    aux = d -> front ;
    d -> front = d -> rear ;
    d -> rear = aux ;   
    
}