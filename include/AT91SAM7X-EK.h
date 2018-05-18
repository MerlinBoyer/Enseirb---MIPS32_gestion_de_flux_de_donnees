// ----------------------------------------------------------------------------
//         ATMEL Microcontroller Software Support  -  ROUSSET  -
// ----------------------------------------------------------------------------
// DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
// DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
// OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// ----------------------------------------------------------------------------
// File Name           : AT91SAM7X-EK.h
// Object              : AT91SAM7X-EK Evaluation Board Features Definition File
//
//  ----------------------------------------------------------------------------

#ifndef AT91SAM7X_EK_H
#define AT91SAM7X_EK_H

#include "include/AT91SAM7X256.h"


///////////////////prototypes //////////////
void Set_curseur(int ligne, int colonne);
void decodeASCII(int col, int lig, char carac);
void S_Set_curseur(char* ligne, char* colonne);



/////////////////////////////////////////////





#define true	-1
#define false	0

/*-----------------*/
/* LEDs Definition */
/*-----------------*/
#define AT91B_LED1            (1<<19)       // AT91C_PIO_PB19 AT91C_PB19_PWM0 AT91C_PB19_TCLK1
#define AT91B_LED2            (1<<20)       // AT91C_PIO_PB20 AT91C_PB20_PWM1 AT91C_PB20_PWM1
#define AT91B_LED3            (AT91C_PIO_PB21)       // AT91C_PIO_PB21 AT91C_PB21_PWM2 AT91C_PB21_PCK1
#define AT91B_LED4            (AT91C_PIO_PB22)       // AT91C_PIO_PB22 AT91C_PB22_PWM3 AT91C_PB22_PCK2
#define LED1                  AT91B_LED1
#define LED2                  AT91B_LED2
#define LED3                  AT91B_LED3
#define LED4                  AT91B_LED4
#define AT91B_NB_LEB          4
#define NB_LED                AT91B_NB_LEB
#define AT91B_LED_MASK        (AT91B_LED1|AT91B_LED2|AT91B_LED3|AT91B_LED4)
#define LED_MASK              AT91B_LED_MASK
#define BASE_PIO_LED         (AT91C_BASE_PIOB)
#define ID_PIO_LED           AT91C_ID_PIOB

#define AT91B_POWERLED        (1<<25)       // PB25


/*-------------------------------*/
/* JOYSTICK Position Definition  */
/*-------------------------------*/
#define AT91B_PBUP          (1<<21)  // PA21 Up Button	  AT91C_PA21_TF  AT91C_PA21_NPCS10
#define AT91B_PBDOWN        (1<<22)  // PA22 Down Button  AT91C_PA22_TK	 AT91C_PA22_SPCK1
#define AT91B_PBLEFT        (1<<23)  // PA23 Left Button  AT91C_PA23_TD  AT91C_PA23_MOSI1
#define AT91B_PBRIGHT       (1<<24)  // PA24 Right Button AT91C_PA24_RD	 AT91C_PA24_MISO1
#define AT91B_PBCENTER      (1<<25)  // PA25 Push Button  AT91C_PA25_RK	 AT91C_PA25_NPCS11
#define AT91B_PB_MASK       (AT91B_PBUP|AT91B_PBDOWN|AT91B_PBLEFT|AT91B_PBRIGHT|AT91B_PBCENTER)



#define BASE_PIO_PUSHB      AT91C_BASE_PIOA
#define ID_PIO_PUSHB        AT91C_ID_PIOA

/*-------------------*/
/* UART Definition   */
/*-------------------*/

#define ID_UART                 AT91C_ID_US0
#define BASE_UART               AT91C_BASE_US0
#define BASE_PIO_UART           AT91C_BASE_PIOA
#define UART_RX_PIN             AT91C_PA0_RXD0
#define UART_TX_PIN             AT91C_PA1_TXD0


/*-------------------*/
/* DBGU Definition   */
/*-------------------*/
//De meme que pour les autres modules on cree des constantes pour DBGU
//pour ameliorer la lisibilite du code

#define DBGU_RX_PIN             AT91C_PA27_DRXD
#define DBGU_TX_PIN             AT91C_PA28_DTXD


/*------------------*/
/* CAN Definition   */
/*------------------*/
#define AT91B_CAN_TRANSCEIVER_RS  (1<<2)    // PA2

/*--------------*/
/* Clocks       */
/*--------------*/
#define AT91B_MAIN_OSC        18432000               // Main Oscillator MAINCK
#define EXT_OC                AT91B_MAIN_OSC   // External ocilator MAINCK
#define AT91B_MCK             ((AT91B_MAIN_OSC*73/14)/2)   // Output PLL Clock
#define MCK                   AT91B_MCK
#define MCKKHz                (MCK/1000)

#endif /* AT91SAM7X-EK_H */
