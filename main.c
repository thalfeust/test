#include "consoleEtFichier.h"
#include "encodage.h"


void minusculeStringCommande();
void affichageBinaireCommande();

int main() {

	int sortieBoucle = 0;
	int mode = 0;

	FILE * ficOutB = fopen("fichierSortieBinaire.txt", "a");
	FILE * ficOutH = fopen("fichierSortieHexa.txt", "a");
	FILE * ficIn = fopen("fichierEntree.txt", "r");

	printf("\nChoix du mode :\n\t-entree des commandes dans la console (1)\n\t-lecture d'un fichier de commandes(2)");
	scanf("%d",&mode);

	if(mode == 1) {
		while(sortieBoucle==0) {
			consoleLectureCommande();
			minusculeStringCommande();
			decodageInstruction();
			affichageBinaireCommande();

			printf("\nContinuer? OUI (0) ou NON (1)");
			scanf("%d",&sortieBoucle);
		}		
	}else {
		while(sortieBoucle==0) {
			printf("/1");
			fichierLectureCommande(ficIn);
			affichageStringCommande();

			minusculeStringCommande();

			decodageInstruction();
			affichageBinaireCommande();
			convertCommande();

			if(ecritUCharTab(ficOutB, 'b')) printf("\nEcriture B reussie.");
			else printf("\nErreur d'ecriture B.");
			
			convertCommande();
			
			if(ecritUCharTab(ficOutH, 'h')) printf("\nEcriture H reussie.");
			else printf("\nErreur d'ecriture H.");

			printf("\nContinuer? OUI (0) ou NON (1)");
			scanf("%d",&sortieBoucle);
		}			
	}

	printf("\n");

	fclose (ficIn);
	fclose (ficOutB);
	fclose (ficOutH);

	return(0);
}


void affichageBinaireCommande() {
	
	char c;

	printf("\n");

	for(int i=31; i>-1; i--) {
		c = commandeBinaire[i];
		printf("%c", c);
	}

	printf("\nfin");
}

void minusculeStringCommande(void) {

	int i=0;
	char c;	
	c = commande[i];
	
	do {
		if ((commande[i]>=0x41) && (commande[i]<0x5B)) {

			commande[i] = commande[i] + 0x20;
		}

		i++;
		c = commande[i];

	}while(c != '\n' && c!= '\0' && c != EOF);
}
