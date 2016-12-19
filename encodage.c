#include "encodage.h"

void adresseRegistre(int indice) {

	int adresse;

	switch(commande[indice]) {
		case 'a' : /* registres arguments de sous-routine */
			if(commande[indice+1] == 't') {
				adresse = 1;
			}else {
				adresse = commande[indice+1] - '0';
				adresse = adresse + 4;
			}
		break;
		case 'f' : /* Frame pointer */
			adresse = 30;
		break;
		case 'g' : /* Global pointer */
			adresse = 28;
		break;
		case 'k' : /* Kernel */
			adresse = commande[indice+1] - '0';
			adresse = adresse + 26;
		break;
		case 'r' : /* Return address */
			adresse = 31;
		break;
		case 's' : /* Registre temporaires, sous-routine */
			adresse = commande[indice+1] - '0';
			adresse = adresse + 16;
		break;
		case 't' : /* Registre temporaires */
			if(commande[indice+1] < 56) {
				adresse = commande[indice+1] - '0';
				adresse = adresse + 8;
			}else {
				adresse = commande[indice+1] - '0';
				adresse = adresse + 16;
			}
		break;
		case 'v' : /* Valeur retournees, sous-routine */
			adresse = commande[indice+1] - '0';
			adresse = adresse + 2;
		break;
		case 'z' : /* Registre toujours nul */
			adresse = 0;
		break;
		default :
			printf("\nRegistre inconnu");
		break;
	}

	adresseRegistreBinaire(adresse);
}

void adresseRegistreBinaire(int registre) {
	printf("\n%c",commande[registre]);
	int nbr;

	for(int i=4; i>-1; i--) {
		nbr = registre >> i;
		if( nbr & 1 ){
			registreBinaire[i] = '1';
		}else {
			registreBinaire[i] = '0';			
		}
	}
}

void decodageInstruction() {
/* Cette procedure va nous permettre de déterminer le type d'instruction (R,I,J) */

	int i = 0;

	switch(commande[i]) {
		case 'a' : /* ADD, ADDI ou AND */

			if(commande[i+3] == 'i') { /* ADDI */
				encodageInstructionI("001000","111");
			} else { /* ADD ou AND */
				if(commande[i+1] == 'd') { /* ADD */
					encodageInstructionR("000000","100000","1110");				
				} else { /* AND */
					encodageInstructionR("000000","100100","1110");					
				}
			}

		break;
		case 'b' : /* BEQ, BNE, BGTZ ou BLEZ */

			switch(commande[i+1]) {
				case 'e' : /* BEQ */
					encodageInstructionI("000100","111");
				break;
				case 'n' : /* BNE */
					encodageInstructionI("000100","111");
				break;
				case 'g' : /* BGTZ */
					encodageInstructionI("000111","101");
				break;
				case 'l' : /* BLEZ */
					encodageInstructionI("000110","101");
				break;
			}

		break;
		case 'd' : /* DIV */

			encodageInstructionR("000000","011010","1100");

		break;
		case 'j' : /* J, JAL ou JR */

			if(commande[i+1] == 'r') { /* JR */
				encodageInstructionR("000000","001000","1000");
			} else { /* JAL ou J */
				if(commande[i+1] == 'a') { /* JAL */
					encodageInstructionJ();				
				} else { /* J */
					encodageInstructionJ();					
				}
			}

		break;
		case 'l' : /* LUI ou LW */
			if(commande[i+1] == 'u') { /* LUI */
				encodageInstructionI("001111","011");				
			} else { /* LW */
				encodageInstructionI("100011","111");					
			}

		break;
		case 'm' : /* MULT, MFHIS ou MFLO  */
			if(commande[i+1] == 'u') { /* MULT */
				encodageInstructionR("000000","011000","1100");				
			}else { /*  MFHIS ou MFLO */
				if(commande[i+2] == 'h') { /* MFHIS */
					encodageInstructionR("000000","010000","0010");				
				}else { /* MFLO */
					encodageInstructionR("000000","010010","0010");
				}				
			}

		break;
		case 'o' : /* OR */

			encodageInstructionR("000000","100101","1110");

		break;
		case 'r' : /* ROTR */

			encodageInstructionR("000000","000010","2110");

		break;
		case 's' : /* SUB, SLL, SRL, SLT ou SW */

			if(commande[i+1] == 'w') { /* SW */
				encodageInstructionI("101011","211");
			} else { /* SUB, SLL, SRL ou SLT */
				if(commande[i+1] == 'u') { /* SUB */
					encodageInstructionR("000000","100010","1110");				
				}else if(commande[i+1] == 'r'){ /*  SRL */
					encodageInstructionR("000000","000010","1110");					
				}else {
					if(commande[i+2] == 'l') { /* SLL */
						encodageInstructionR("000000","000000","0111");				
					}else { /* SLT */
						encodageInstructionR("000000","101010","1110");
					}				
				}
			}

		break;
		case 'x' : /* XOR */

			encodageInstructionR("000000","100110","1110");

		break;
		default :

			printf("\nPas d'instruction reconnue");

		break;
	}
}

void encodageInstructionR(char opcode[], char function[],  char operandes[]) {
	/* Opcode est l'opcode */
	/* function est la fonction */
	/* operande (4 char) represante rs, rt, rd et sa : */
	/* 1 : si plein */
	/* 0 : si vide */
	/* 2 : cas pour le ROTR */

	printf("\n%c",'R');
	int indice = 30;

	for(int i=31; i>25; i--) {
		commandeBinaire[i] = opcode[31-i];
	}

	/* Sa */
	if (operandes[3] == '1') {
		indice = chercherValeur(indice);
		encodageValeur(indice+1);
		for(int i=10; i>5; i--) {
			commandeBinaire[i] = valeurBinaire[i-6];
		}		
	} else {
		for(int i=10; i>5; i--) {
			commandeBinaire[i] = '0';
		}
	}	

	/* Rt */
	if (operandes[1] == '1') {
		indice = chercherRegistre(indice);
		adresseRegistre(indice+1);
		for(int i=20; i>15; i--) {
			commandeBinaire[i] = registreBinaire[i-16];
		}
	}else {
		for(int i=20; i>15; i--) {
			commandeBinaire[i] = '0';
		}
	}
	/* Rs */
	if (operandes[0] == '1') {
		indice = chercherRegistre(indice);
		adresseRegistre(indice+1);
		for(int i=25; i>20; i--) {
			commandeBinaire[i] = registreBinaire[i-21];
		}
	} else {
		for(int i=25; i>20; i--) {
			commandeBinaire[i] = '0';
		}		
	}
	/* Rd */
	if (operandes[2] == '1') {
		indice = chercherRegistre(indice);
		adresseRegistre(indice+1);
		for(int i=15; i>10; i--) {
			commandeBinaire[i] = registreBinaire[i-11];
		}
	} else {
		for(int i=15; i>10; i--) {
			commandeBinaire[i] = '0';
		}		
	}	

	/* Function */
	for(int i=5; i>-1; i--) {
		commandeBinaire[i] = function[5-i];
	}	
}

void encodageInstructionI(char opcode[],  char operandes[]) {
	/* Opcode est l'opcode */
	/* operande (4 char) represante rs, rt et immediate : */
	/* 1 : si plein */
	/* 0 : si vide */
	/* 2 : cas du SW */

	printf("\n%c",'I');

	int indice = 30;

	for(int i=31; i>25; i--) {
		commandeBinaire[i] = opcode[31-i];
	}

	/* immediate */
	if (operandes[2] == '1') {
		indice = chercherValeur(indice);
		encodageImmediat(indice+1);
		for(int i=15; i>-1; i--) {
			printf(" %c", immediatBinaire[i] );
			commandeBinaire[i] = immediatBinaire[i];
		}		
	} else {
		for(int i=15; i>-1; i--) {
			commandeBinaire[i] = '0';
		}
	}	

	/* Rt */
	if (operandes[1] == '1') {
		indice = chercherRegistre(indice);
		adresseRegistre(indice+1);
		for(int i=20; i>15; i--) {
			commandeBinaire[i] = registreBinaire[i-16];
		}
	}else {
		for(int i=20; i>15; i--) {
			commandeBinaire[i] = '0';
		}
	}
	/* Rs */
	if (operandes[0] == '1') {
		indice = chercherRegistre(indice);
		adresseRegistre(indice+1);
		for(int i=25; i>20; i--) {
			commandeBinaire[i] = registreBinaire[i-21];
		}
	} else {
		for(int i=25; i>20; i--) {
			commandeBinaire[i] = '0';
		}		
	}
}

void encodageInstructionJ() {
	printf("\nJ");
}

void encodageValeur(int indice) {

	int i = 0;
	unsigned int valeur = 0;
	int nbr;

	do {
		valeur = valeur * 10;
		valeur += commande[i+indice] - '0';
		i++;
	}while(commande[indice+i] >= 48 && commande[indice+i] <= 57);

	for(int i=4; i>-1; i--) {
		nbr = valeur >> i;
		if( nbr & 1 ){
			valeurBinaire[i] = '1';
		}else {
			valeurBinaire[i] = '0';			
		}
	}	
}

void encodageImmediat(int indice) {

	int i = 0;
	unsigned int valeur = 0;
	int nbr;

	do {
		valeur = valeur * 10;
		valeur += commande[i+indice] - '0';
		i++;
	}while(commande[indice+i] >= 48 && commande[indice+i] <= 57);

	for(int i=15; i>-1; i--) {
		nbr = valeur >> i;
		if( nbr & 1 ){
			immediatBinaire[i] = '1';
		}else {
			immediatBinaire[i] = '0';			
		}
	}	
}

int chercherRegistre(int indice) {
	
	int i = indice;

	do {
		i--;
	}while(commande[i] != '$');

	return i;
}

int chercherValeur(int indice) {
	
	int i = indice;
	char trouve = '0';

	do {
		i--;
		if(commande[i] >= 48 && commande[i] <= 57) {
			trouve = '1';
		}else if(trouve == '1') {
			trouve = '2';
		}
	}while(trouve != '2');
	return i;
}

void convertCommande() {

	unsigned int buff=0;
	char k=0;
	int y=0;
	int flag=0;
	char symboles[] = "0123456789ABCDEF";
	char tampon;

	for (int i=0;i<32;i++) {	    
		
		buff = buff | ((commandeBinaire[4*y+(k)]-48)<<k);
		k++;
		if (k>=4) {
			commandeHexa[y] = buff;
			k=0;
			printf("%d %d %d %d\n",buff, commandeHexa[y],y,i);
			buff=0;
			y++;
		}
	}
	for (int h=0;h<8;h++) {

		commandeHexa[h]=symboles[commandeHexa[h]];
	}
	for(int i=0; i<4; i++) {
		tampon = commandeHexa[i];
		commandeHexa[i] = commandeHexa[7-i];
		commandeHexa[7-i] = tampon;
	}
	printf("\n\n%s\n",commandeHexa);
}