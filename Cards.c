#include<stdio.h>
#include "SimpleList.h"
#include "Amestecari.h"
#include <string.h>

int min( int a , int b )
{
    if( a < b )
        return a ;
    return b ;
}

Deck* InitDeck( )
{
    Deck* d = createDeck() ;
    int i ;
    for( i = 15 ; i >= 2 ; i-- )
    {
        if( i == 11 ) continue ;
        addToRear( d , i , 's' );
        addToRear( d , i , 'h' );
        addToRear( d , i , 'c' );
        addToRear( d , i , 'd' );
    }
    return d ;
}

Deck* Suffle( FILE *in )
{
    Deck *d = InitDeck();
   int n , i  ;
   char suffle[ 20 ];
   fscanf( in , "%d" , &n );
   for( i = 1 ; i<= n ; i++ )
        {
            fscanf( in , "%s" , suffle );
            if( strcmp( suffle  , "inversare") == 0 )
                invertDeck( d );
            if( strcmp( suffle  , "intercalare") == 0 )
                d = IntercalareDeck( d  );
            if( strcmp( suffle  , "mongean") == 0 )
                d = MongeanDeck( d  );
            if( strcmp( suffle  , "npiles") == 0 )
                d = NpilesDeck( d , in  );
        }
    return d ; 
}


void DoTask1( FILE *in , FILE *out  )
{
   Deck *d = Suffle( in );
   PrintDeck( d , out ); 
   destroyDeck( d );
}

void MoveNNodes ( Deck *from , Deck *to , int n )
{
    if ( n == 0 )
        return ;
    Node *no ;
    no = GetFirstNode( from );
    MoveNNodes( from , to , n - 1 );
    AddNodeToRear( to , no );     
}

void Razboi( Deck *p1 , Deck *p2 , FILE *out )
{
    int i , gata = 0 , cardp1 , cardp2 , nrcards ;
    int fp1[16]={0} , fp2[16]={0} ;
    Node *k ;
    for( i = 1 ; i <= 100 && gata == 0 ; i++)
    {   
        // cartile puse de ambii jucatori
        cardp1 = GetTheNrOfNth( p1 , 1 );
        cardp2 = GetTheNrOfNth( p2 , 1 );
        
        if( cardp1 > cardp2 )
            {
                MoveNNodes( p1 , p1 , 1 );
                MoveNNodes( p2 , p1 , 1 );
            }
        if( cardp1 < cardp2 )
            {
                MoveNNodes( p2 , p2 , 1 );
                MoveNNodes( p1 , p2 , 1 );
            }
        if( cardp1 == cardp2 )
        {   //in caz de razboi aflam cartile de la ambii jucatori care vin jos
            nrcards = min ( min( 1 + cardp1 , p1 -> size ) , min( 1 + cardp2 , p2 -> size ) );
            cardp1 = GetTheNrOfNth( p1 , nrcards );
            cardp2 = GetTheNrOfNth( p2 , nrcards );
            if( cardp1 > cardp2 )
                {
                    MoveNNodes( p1 , p1 , nrcards );
                    MoveNNodes( p2 , p1 , nrcards );
                }
            if( cardp1 < cardp2 )
                {
                    MoveNNodes( p2 , p2 , nrcards );
                    MoveNNodes( p1 , p2 , nrcards );
                }
        }
        if( p1 -> size == 0 || p2 -> size == 0 )
            gata = 1 ;
    }
    if( gata == 1 )
        {
        if( p1 -> size == 0 )
            fprintf( out , "2" );
        else 
            fprintf( out , "1" );
        }
    else
        {   
            //calculam elementele din vectorii de frecventa
            k = p1 -> front;
            while( k != NULL )
                {
                    fp1[ k -> nr ] ++ ;
                    k = k -> next ;
                }
            k = p2 -> front;
            while( k != NULL )
                {
                    fp2[ k -> nr ] ++ ;
                    k = k -> next ;
                }
            for( i = 15 ; i >= 2 ; i-- )
                if( fp1[ i ] > fp2[ i ])
                    {
                     fprintf( out , "1 ");
                     PrintNr( i , out );
                     break ;
                    }
                else 
                    if( fp1[ i ] < fp2[ i ] )
                        {
                        fprintf( out , "2 ");
                        PrintNr( i , out );
                        break ;
                        }
            
        }
    destroyDeck( p1 );
    destroyDeck( p2 );
}

void DoTask2( FILE *in , FILE *out )
{
  int nr;
  Deck *d = Suffle( in );
  Node *i;
  Deck *p1 , *p2 ;
  p1 = createDeck();
  p2 = createDeck();
  i = d -> front ;
  //cream pachetul primului jucator
  for( nr = 1 ; nr <= d -> size /2 ; nr++ )
  {
      addToRear( p1 , i -> nr , i -> color );
      i = i -> next ;
  } 
  //cream pachetul celui de al doilea jucator
  for( nr = 1 ; nr <= d -> size /2 ; nr++ )
  {
      addToRear( p2 , i -> nr , i -> color );
      i = i -> next ;
  }  
  destroyDeck( d );
  Razboi( p1 , p2 , out ); 
}

int nextcolor( int a[ 4 ][ 4 ] , int player , int color)
{
 int i ;
 for( i = color + 1 ; i <= 3 ; i++ )
    if( a[ player ][ i ] != 0 )
        return i;
 for( i = 0 ; i <= color ; i++ )
    if( a[ player ][ i ] != 0 )
        return i;  
return 4;   
}

void RollingStone( FILE *out , Deck *p[ 4 ] )
{
    int cards[4][4] = {0};
    //cards[ i ][ j ] = numarul de carti pe care il are playerul i + 1 de culoare j dupa regula de jos
    // 0 == s 
    // 1 == h 
    // 3 == c
    // 4 == d 
    Node *node ;
    int i , played = 0 , color , startp , size[ 4 ] = { 13 , 13 , 13 , 13}, loser , win;
    //construim matriecea a
    for( i = 0 ; i < 4 ; i++ )
    {
        for( node = p[ i ] -> front ; node != NULL ; node = node -> next)
            {
                if( node -> color == 's' )
                    cards[ i ][ 0 ] ++;
                if( node -> color == 'h' )
                    cards[ i ][ 1 ] ++;
                if( node -> color == 'c' )
                    cards[ i ][ 2 ] ++;
                if( node -> color == 'd' )
                    cards[ i ][ 3 ] ++;    
            }
    }
    // avand matricea a nu mai avem nevoie de pachetele initiale de carti 
    for( i = 0 ; i < 4 ; i++ )
        destroyDeck( p[ i ] );
    //precizam ca pozitiile in care le-am memorat sunt mai mici cu 1 decat cele cerute
    //playerul care incepe
    startp = 0 ;
    //culoarea de inceput ar trebui sa fie 3 pentru a incepe cu 0 corespondetul lui s
    color = 3 ;
    //playerul care a pierdut o mana
    loser = -1 ;
    //jucatorul care a castigat
    win = -1;
    // pana cand jocul nu s-a terminat
    while( win == -1 )
    {   
        //aflam culoare jucata de jucatorul care incep mana
        color = nextcolor( cards , startp , color);    
        //simulam punerea de carti jos
        while( loser == -1 )
        {
            // parcurgem jucatorii de la cel care incepe 
            for( i = startp ; i < 4 && loser == -1 ; i++ )
                {   
                    //daca playerul i are o carte de culoarea ceruta o pune 
                    if( cards[ i ][ color ] != 0 )
                        {
                            played ++ ;
                            cards[ i ][ color ]-- ;
                            size[ i ] -- ;
                        }
                    //altfel fie ia cartile sau castiga jocul
                    else
                        {
                            if( size[ i ] != 0 )
                                {
                                 loser = i ;
                                 break ;
                                }
                            else 
                                {
                                    loser = 4 ;
                                    win = i ;
                                    break ;
                                }
                        }
                }
            //parcurgerea jucatorilor de la 0 la jucatorul care a inceput pentru a inchide bucla
            for( i = 0; i < startp && loser == -1 ; i++ )
                {
                    if( cards[ i ][ color ] != 0 )
                        {
                            played ++ ;
                            cards[ i ][ color ]-- ;
                            size[ i ] -- ;
                        }
                    else
                        {
                            if( size[ i ] != 0 )
                                {
                                 loser = i ;
                                 break ;
                                }
                            else 
                                {
                                 loser = 4 ;
                                 win = i ;
                                 break ;
                                }
                        }
                }
        }

       if( loser != 4 )
            {
                cards[ loser ][ color ] += played;
                size[ loser ] += played ;
                played = 0 ;
                startp = loser ; 
                loser = -1;
            } 
        
    }

    fprintf( out , "%d" , win + 1 );
}

void DoTask3( FILE *in , FILE * out )
{
    Deck *d = Suffle( in );
    Deck *p[ 4 ] ;
    Node *i ;
    int nr , k ;
    i = d -> front ;

    for( k = 0 ; k < 4 ; k ++ )
       { 
         
        p[ k ] = createDeck();
        
        for( nr = 1 ; nr <= d -> size / 4 ; nr ++ )
            {
                addToRear( p[ k ] , i -> nr , i -> color );
                i = i -> next ;
            }
        }
    destroyDeck( d );
    RollingStone( out , p );
    
}

typedef struct player
{
	int nr;
	struct player *next;
}player;


player* initPlayerList()
{
    player *p ;
    p = malloc( sizeof( player ) );
    p -> nr = 0 ;
    p -> next = malloc( sizeof( player ) );
    p -> next -> nr = 1 ;
    p -> next -> next = malloc( sizeof( player ) );
    p -> next -> next -> nr = 2 ;
    p -> next -> next -> next = malloc( sizeof( player ) );
    p -> next -> next -> next -> nr = 3 ;
    p -> next -> next -> next -> next = p ;
    return p ;
}

void MoveNNodes2 ( Deck *from , Deck *to , int n )
{
    if ( n == 0 )
        return ;
    Node *no ;
    no = GetFirstNode( from );
    AddNodeToRear( to , no );    
    MoveNNodes2( from , to , n - 1 );    
}

void TurnGame( player *p , int size )
{
    player *next = NULL , *prev = NULL , *cur ;
    cur = p ;
    for( int i = 1 ; i <= size ; i ++ )
        {
            next = cur -> next ;
            cur -> next = prev ;
            prev = cur ;
            cur = next ;
        }
        next -> next = prev ;
}

int special( int nr )
{
    if( nr == 15 || nr == 14 || nr == 13 || nr == 12 || nr == 8 )
        return 1 ;
    return 0 ;
}

void Elim( player *p , int size , int val )
{
    int i ;
    player *j , *aux ;
    j = p ; 
    for( i = 1 ; i <= size ; i++ )
        {
            if( j -> next -> nr == val )
                {
                    aux = j -> next ;
                    j -> next = j -> next -> next ;
                    free (aux);
                    return; 
                }
            j = j -> next ;
        }

}

void Scarabei( FILE *out , Deck *p[ 4 ] )
{   
    player *j = initPlayerList() , *prev ; 
    Deck *table;
    Node *n ;
    //lost  = daca s-a pierdut mana curenta
    //nrPl numarul de player ramasi
    //isLastScar daca ultima carte jucata este scarabeu
    int nrPl = 4 , lost , isLastScar  , aux   ;
    //in table tinem cartile de pe masa
    table = createDeck();
    //cat timp avem mai mult de un jucator ramas
    while( nrPl != 1 )
    {   
        lost = 0 ;
        isLastScar = 0 ;
        //punem cartea primului jucator pe masa 
        n = GetFirstNode( p[ j -> nr ] );
        AddNodeToFront( table , n );
        //daca pune jos o carte care este scarabeu sau 8
        if( special( table -> front -> nr ) == 1 )
              if( table -> front -> nr == 8 )
                    TurnGame( j , nrPl ) ; //intoarcem ordinea jucatorilor
              else 
                isLastScar = 1 ; 
        //trecem la urmatorul jucator
        prev = j ;
        j = j -> next ;
        
        while( lost == 0 )
        {   
            //jucatorul curent isi pune cartea jos
            n = GetFirstNode( p[ j -> nr ] );
            AddNodeToFront( table , n );
            //daca nu este 8 sau scarabeu
            if( special( table -> front -> nr ) == 0 )
                { 
                 if( isLastScar == 1 ) // daca inainte am avut un scaabeu jucatorul pierde
                     {
                        lost = 1 ;
                        isLastScar = 0 ;
                        MoveNNodes2( table , p[ prev -> nr ] , table -> size );
                     }
                }
            else
                {
                    if( table -> front -> nr == 8 )
                        if( isLastScar == 0 ) //daca se pune un 8 si nu am avut inainte scarabeu
                            TurnGame( j , nrPl );
                        else
                            {
                                j = j -> next ; //altfel trecem la urmatorul jucator
                                continue ;
                            }
                    
                    else
                       isLastScar = 1 ; 
                }
            if( p[ prev -> nr ] -> size == 0 ) // daca cel de dinainte a ramasa fara carti va fi scos din joc
                {
                  Elim( j , nrPl , prev -> nr );  
                  nrPl -- ;
                }
            if( lost == 0) // daca nu s-a terminat trecem la playerul urmator
                {
                prev = j  ;
                j = j -> next ;
                }
            else    //daca jocul curent este gata
                {
                  j = prev ; //ne repozitionam la jucatorul care a castigat
                  if( p[ j -> next -> nr ] -> size == 0 )   //daca un jucator de dupa a ramas fara carti
                    {
                        Elim( j , nrPl , j -> next -> nr );  // il scoatem
                        nrPl -- ;
                    }     
                }
        }
    } 
    destroyDeck( table ); //eliberam memoria
    fprintf( out, "%d " , j -> nr + 1 );  

}
void DoTask4( FILE *in , FILE * out )
{
    Deck *d = Suffle( in );
    Deck *p[ 4 ] ;
    Node *i ;
    int nr , k ;
    i = d -> front ;
    // impartim pachetul in cele 4 grupuri cerute
    for( k = 0 ; k < 4 ; k ++ )
       { 
         
        p[ k ] = createDeck();
        
        for( nr = 1 ; nr <= d -> size / 4 ; nr ++ )
            {
                addToRear( p[ k ] , i -> nr , i -> color );
                i = i -> next ;
            }
        }
    Scarabei( out , p );
    //golim memoria folosita
    destroyDeck( d );
    destroyDeck( p[ 0 ] );
    destroyDeck( p[ 1 ] );
    destroyDeck( p[ 2 ] );
    destroyDeck( p[ 3 ] );
}



int main()
{   
    FILE *in , *out ;
    char task[20];
    in = fopen("in" , "r");
    out = fopen("out" , "w");
    fscanf( in , "%s" , task );

    if( strcmp( task , "amestecare") == 0 )
        DoTask1( in , out );
    if( strcmp( task , "razboi") == 0 )
        DoTask2( in , out );
    if( strcmp( task , "rollingstone") == 0 )
        DoTask3( in , out );
    if( strcmp( task , "scarabei") == 0 )
        DoTask4( in , out );
    return 0;
}