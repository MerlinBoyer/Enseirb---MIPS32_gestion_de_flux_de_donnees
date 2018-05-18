// Include Standard files
#include "include/AT91SAM7X-EK.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global variables */


//var pr le projet :
char FIFO[8000]; int fiw;int fir;  //fifo et ses indices d'écriture/lecture






///////////////////////// fct send   ////////////////////////////

void send_char_UART(char carac){  //envoie un carac sur la liaison USART
  while(!AT91C_BASE_US0->US_CSR&AT91C_US_TXRDY){}       //attend que le module d'envoi soit libre
  AT91C_BASE_US0->US_THR = carac;
}

void send_char_DBGU(char carac){  //envoie un carac sur la liaison DBGU
  while(!AT91C_BASE_DBGU->DBGU_CSR&AT91C_US_TXRDY){}       //attend que le module d'envoi soit libre
  AT91C_BASE_DBGU->DBGU_THR = carac;
}

void send_word(char *tab){ //envoie tout le tableau de carac passé en argument dans l'usart (vers Teraterm)
  int i =0;
  while(tab[i]!='\0'){
    while(!(AT91C_BASE_US0->US_CSR&AT91C_US_TXRDY)){}
    AT91C_BASE_US0->US_THR = tab[i];
    i++;
  }
}

void send_back(){ //echo de carac sur l'usart
  if((AT91C_BASE_US0->US_CSR&AT91C_US_RXRDY)){
    char carac = AT91C_BASE_US0->US_RHR&0x00FF; //recup des 8 derniers bits
    send_char_UART(carac);
  }
}


void send_fifo(char *tab){  //envoie les caracs de fir à fiw de la fifo
  while(fir <= fiw){  
  	//Verifie qu'il a plus de caracteres ecrits que lus
    while(!(AT91C_BASE_US0->US_CSR&AT91C_US_TXRDY)){}
    	//attend que la liaision usart soit libre
    if (tab[fir]==255){ 
    	//detection de la reception d'une trame de l'image ascii en retour d'un 'L'
      if(fir+3 > fiw){;} 
      	//on n'a pas encore recu les caracteres suivants de la trame
      else{
      	decodeASCII(tab[fir+1],tab[fir+2],tab[fir+3]); 
      	//on affiche le caractere a la bonne place
      	fir += 4;
      	//incrementation de l'indice de lecture de la fifo
      }
    }else{  //sinon on affiche simplement le message
      AT91C_BASE_US0->US_THR = tab[fir];
      	//envoie d'un caractere sur la liaison USART (teraTerm)
      fir++;
      	//incrementation de l'indice de lecture de la fifo
    }
  }
}






///////////////////////// fct curseur et dessin   ////////////////////////////

///// determine centaine/dizaine/unite a partir d'un char*
char Scentaine(char* carac){
  int centaine = atoi(carac)/100;
  return centaine+'0';
}

char Sdizaine(char* carac){
  int dizaine = (atoi(carac)/10)-(atoi(carac)/100)*10;
  return dizaine+'0';
}
char Sunite(char* carac){
  int unite = atoi(carac)%10;
  return unite+'0';
}

///// determine centaine/dizaine/unite a partir d'un int
char centaine(int nb){
  int centaine = nb/100;
  return centaine+'0';
}

char dizaine(int nb){
  int dizaine = (nb/10)-(nb/100)*10;
  return dizaine+'0';
}
char unite(int nb){
  int unite = nb%10;
  return unite+'0';
}


void Set_curseur_default(void){
  char chaine[10];
  chaine[0] = 0x1B;
  chaine[1] = '[';
  chaine[2] = 'H';
  send_word(chaine);
}


void Set_curseur(int ligne, int colonne){  
  char chaine[10];
  chaine[0] = 0x1B;
  chaine[1] = '[';
  chaine[2] = centaine(ligne);
  chaine[3] = dizaine(ligne);
  chaine[4] = unite(ligne);
  chaine[5] = ';';
  chaine[6] = centaine(colonne);
  chaine[7] = dizaine(colonne);
  chaine[8] = unite(colonne);
  chaine[9] = 'H';
  send_word(chaine); // il faut envoyer les caractere ESC et '[' en un seul envoi 
}

void S_Set_curseur(char* ligne, char* colonne){  //probleme : affiche le H et ne place pas le curseur
  char chaine[10];
  chaine[0] = 0x1B;
  chaine[1] = '[';
  chaine[2] = Scentaine(ligne);  //cas generale avec les centaine et les dizaines,
  chaine[3] = Sdizaine(ligne);  // peut etre egal a 0
  chaine[4] = Sunite(ligne);
  chaine[5] = ';';
  chaine[6] = Scentaine(colonne);
  chaine[7] = Sdizaine(colonne);
  chaine[8] = Sunite(colonne);
  chaine[9] = 'H';
  send_word(chaine); // il faut envoyer les caractere ESC et '[' en un seul envoi 
}


void decodeASCII(int col, int lig, char carac){  //place carac en position lig,col lors de la reception de l'image en ASCII 
  Set_curseur(lig, col);
  AT91C_BASE_US0->US_THR = carac;
}






///////////////////////// fct IT   ////////////////////////////

void IT_ReadC_dbgu(){ //stoque un carac recu du kit dans la fifo
  if((AT91C_BASE_DBGU->DBGU_CSR&AT91C_US_RXRDY)){
    FIFO[fiw] = AT91C_BASE_DBGU->DBGU_RHR&0x00FF;  //recup des 8 derniers bits
    fiw++;
  }
}


void IT_ReadC_uart(){ //envoie un carac recu du PC directement au kit
  if((AT91C_BASE_US0->US_CSR&AT91C_US_RXRDY)){
    char carac =  AT91C_BASE_US0->US_RHR&0x00FF;  //recup des 8 derniers bits
    if(carac=='+'||carac=='s'||carac=='?'||carac=='L'||carac==' '){  //verifie que la commande correspond a une commande prédéfinie
      while(!AT91C_BASE_DBGU->DBGU_CSR&AT91C_US_TXRDY){}       //attend que la liaison dbgu soit libre
      AT91C_BASE_DBGU->DBGU_THR = carac;          //envoie le carac au kit
    }
  }
}







///////////////////////// main   ////////////////////////////

int main()
{ 
    //////////  initialisation  ///////////////
  
    fiw = 0;fir = 0; //init indice de la fifo
    
    // d'abord, il faut activer l'horloge sur le PIO
       AT91C_BASE_PMC->PMC_PCER =  (1 << ID_PIO_LED);
    // ensuite, il faut activer l'horloge sur le module UART
       AT91C_BASE_PMC->PMC_PCER =  (1 << ID_UART);
    // Horloge sur module DBGU : deja parametree
       
    // Ensuite, on attribue les broche des LEDs au PIO, puis on les configure
    // en sortie.
	BASE_PIO_LED->PIO_PER = LED_MASK; // Set in PIO mode
	BASE_PIO_LED->PIO_OER = LED_MASK; // Configure in Output


    // on commence par initialiser les LED dans l'état éteint
    // on écrit dans le registre SODR, pour mettre à '1' car on est en
    // logique inversée
       BASE_PIO_LED->PIO_SODR = LED_MASK ;

    // il faut définir que les broches UART_RX_PIN et UART_TX_PIN  ne sont pas en mode PIO
       AT91C_BASE_PIOA->PIO_PDR = UART_RX_PIN | UART_TX_PIN; 
    // puis les attribuer au module A
       AT91C_BASE_PIOA->PIO_ASR = UART_RX_PIN | UART_TX_PIN;
       // ces 2 etapes servent a 
       //s'assurer que l'UART est bien connecte au PIOA et pas PIOB.
    // De meme pour DBGU :
        AT91C_BASE_PIOA->PIO_PDR = DBGU_TX_PIN | DBGU_RX_PIN; 
        AT91C_BASE_PIOA->PIO_ASR = DBGU_TX_PIN | DBGU_RX_PIN; 

    // cet accès valide les RX et TX.
       AT91C_BASE_US0->US_CR= AT91C_US_RXEN | AT91C_US_TXEN;
    // Config module USART0 (US0)
       AT91C_BASE_US0->US_MR= AT91C_US_USMODE_NORMAL|AT91C_US_CHMODE_NORMAL|(0x0<<8)|AT91C_US_CHRL_8_BITS|AT91C_US_PAR_NONE; 
       /* AT91C_US_CHRL_8_BITS = carac sur 8 bits (0x3 << 6)
          AT91C_US_RSYNC = Sync mode reset (0x0 << 8)
          AT91C_US_PAR_NONE = pas de parite (0x4 <<  9)
       */
    // Configuration du baud Rate pour l'UART:
       AT91C_BASE_US0->US_BRGR =MCK/16/9600;  //baud rate = 9600, carac impose par le PC : BR<115200
        
    // Config DBGU :
        AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN | AT91C_US_TXEN ; //autorise TX et RX
        AT91C_BASE_DBGU->DBGU_MR = AT91C_US_CHMODE_NORMAL | AT91C_US_PAR_NONE; //mode normal & pas de parite
        AT91C_BASE_DBGU->DBGU_BRGR = MCK/16/200000; // Baud rate = 200kBps (énoncé)
          
        //interr :
    // Enable interrupt USART
        AT91C_BASE_US0->US_IER=AT91C_US_RXRDY;  // En. interruption sur la reception
        AT91C_BASE_US0->US_IDR=AT91C_US_TXRDY; // En. interruption sur l'envoi 
    // Puis Enable interrupt DBGU
        AT91C_BASE_DBGU->DBGU_IER = AT91C_US_RXRDY;
        AT91C_BASE_DBGU->DBGU_IDR = AT91C_US_TXRDY;
        
    // AIC initialization
        //UART
	AT91C_BASE_AIC->AIC_IECR = (1<<AT91C_ID_US0); //AT91C_ID_US0 = 6; identifiant du vecteur d'interruption
        AT91C_BASE_AIC->AIC_SVR[AT91C_ID_US0] = (unsigned int)IT_ReadC_uart ;  //inter si reception sur uart
        AT91C_BASE_AIC->AIC_SMR[AT91C_ID_US0] = (0x02<<5)|(0x03<<0); //type d'inter (6 = front montant,...) et priorité (de 1 a 7)
        //DBGU :
        AT91C_BASE_AIC->AIC_IECR = (1<<AT91C_ID_SYS);
        AT91C_BASE_AIC->AIC_SVR[AT91C_ID_SYS] = (unsigned int)IT_ReadC_dbgu; //inter si reception sur dbgu
        AT91C_BASE_AIC->AIC_SMR[AT91C_ID_SYS] = (0x00<<5)|(0x04<<0);
        
        
       
    //////////  boucle infinie  ///////////////
    for (;;)
    {
      if(fir < fiw ){ //la fifo a été modifiée
        Set_curseur(5,5);  //place le surseur en position 2,2
        send_fifo(FIFO); //on envoie les carac ajoutés
        fir = 0;         //re-init des indices de la fifo
        fiw = 0;
      }
    }
}
