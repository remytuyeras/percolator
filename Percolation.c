/******************************************************************************/
/*HEADERS*/
/******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <MlvTypeC.h>        
#include <MlvProcC.h>  
/******************************************************************************/
/*PARAMATERS*/
/******************************************************************************/
#define W 1000
#define H 600
#define Portex 20
#define Portey 20
#define RES (int)sqrt((((float)(W*H))/10.0))
#define BordG (int)((7.0/8.0)*W)
/******************************************************************************/
/*STRUCTURES*/
/******************************************************************************/
typedef struct Position {
	int		x;
	int 		y;
	} Pos;

enum Materiaux {blanc, bleu, rouge};

typedef enum Materiaux Matiere;

typedef struct Site {
	Pos 		lieu;
	Matiere	 	mat;
	struct Site	*SiteDroit;
	struct Site 	*SiteBas;
	struct Site	*SiteGauche;
	struct Site	*SiteHaut;
	} Sit, *Porte_Sit;

typedef struct Rectangle {
	int 		Gx;
	int 		By;
	int 		Dx;
	int 		Hy;
	} Rect; 

/******************************************************************************/
/*FUNCTIONS*/
/******************************************************************************/

/******************************Alloue_Sit**************************************/

Porte_Sit Alloue_Sit(void) {
	printf(">>Alloue_Sit\n");
	Porte_Sit InterSit;
	if((InterSit=(Porte_Sit)malloc(sizeof(Sit)))==NULL) {
	fprintf(stderr,"Erreur d'Allocation: Alloue_Sit");
	exit(1);
	}
	return InterSit;
	}

/********************************Creer_Sit*************************************/

Porte_Sit Creer_Sit(int x, int y, Matiere mat) {
	printf(">>Creer_Sit\n");
	Porte_Sit InterSit;
	InterSit=Alloue_Sit();
	Pos Position={x,y};
	InterSit->lieu=Position;
	InterSit->mat=mat;
	InterSit->SiteDroit=NULL;
	InterSit->SiteBas=NULL;
	InterSit->SiteGauche=NULL;
	InterSit->SiteHaut=NULL;
	return InterSit;
	}

/*******************************Creer_Milieu***********************************/

void Creer_Milieu(int largeur, int profondeur, Porte_Sit *Med_Porte, MlvStruct *X_ecran) {
	printf(">>Creer_Milieu\n");
	int i,j;
	float c=1.0;
	Porte_Sit *Med_Memoire=Med_Porte;
	
	*Med_Porte=Creer_Sit(0,0,blanc);
	
	draw_filled_rectangle(BordG-225,100,100,4,"white",X_ecran);
	draw_rectangle(BordG-225,100,100,4,"black",X_ecran);
	
	for(i=1; i<largeur; Med_Porte=&(*Med_Porte)->SiteDroit, i++) {
		c++;
		draw_filled_rectangle(BordG-224,103,(int)((c/((float)(RES*RES)))*99),1,"PaleGreen3",X_ecran);
		draw_filled_rectangle(BordG-224,102,(int)((c/((float)(RES*RES)))*99),1,"PaleGreen3",X_ecran);
		draw_filled_rectangle(BordG-224,101,(int)((c/((float)(RES*RES)))*99),1,"DarkSeaGreen1",X_ecran);
		if (i%50==0) display_window(X_ecran);
		(*Med_Porte)->SiteDroit=Creer_Sit(i,0,blanc);
		(*Med_Porte)->SiteDroit->SiteGauche=*Med_Porte;
		}
	
	for(j=1;j<profondeur;j++) {
		Med_Porte=Med_Memoire;
		(*Med_Porte)->SiteBas=Creer_Sit(0,j,blanc);
		(*Med_Porte)->SiteBas->SiteHaut=*Med_Porte;
		Med_Memoire=&(*Med_Porte)->SiteBas;
		c++;
		
		for(i=1;i<largeur; Med_Porte=&(*Med_Porte)->SiteDroit, i++) {
			c++;
		        draw_filled_rectangle(BordG-224,103,(int)((c/((float)(RES*RES)))*99),1,"PaleGreen3",X_ecran);
		        draw_filled_rectangle(BordG-224,102,(int)((c/((float)(RES*RES)))*99),1,"PaleGreen3",X_ecran);
			draw_filled_rectangle(BordG-224,101,(int)((c/((float)(RES*RES)))*99),1,"DarkSeaGreen1",X_ecran);
			if (i%50==0) display_window(X_ecran);
			(*Med_Porte)->SiteBas->SiteDroit=Creer_Sit(i,j,blanc);
			(*Med_Porte)->SiteBas->SiteDroit->SiteGauche=(*Med_Porte)->SiteBas;
			(*Med_Porte)->SiteDroit->SiteBas=(*Med_Porte)->SiteBas->SiteDroit;
			(*Med_Porte)->SiteDroit->SiteBas->SiteHaut=(*Med_Porte)->SiteDroit;
			}
		}
	}

/****************************Liberer_Milieu************************************/

void Liberer_Milieu(int largeur, int profondeur, Porte_Sit *Med_Porte) {
	printf(">>Liberer_Milieu\n");
	int i,j;
	Porte_Sit *Med_Memoire;
	Porte_Sit InterSit;
	for(j=0;j<profondeur; Med_Porte=Med_Memoire, j++) {
		printf(">>Liberer_Milieu: j=%d \n",j);
		Med_Memoire=&(*Med_Porte)->SiteBas;
		
		for(i=0;i<largeur;i++) {
			printf(">>Liberer_Milieu: j=%d et i=%d \n",j,i);
			InterSit=*Med_Porte;
			Med_Porte=&(*Med_Porte)->SiteDroit;
			free(InterSit);
			}
		}
	}
	
/**********************************Mat_Aleat***********************************/

Matiere Mat_Aleat(float p, int precision) {
	printf(">>Mat_Aleat\n");
	float f=((float)precision)*p;
	int d=floor(f);
	printf(">>Mat_Aleat: d=%d et f=%f\n",d,f);
	if (random(precision)<=d-1) {printf(">>Mat_Aleat: bleu\n"); return bleu;}
	else {printf(">>Mat_Aleat: blanc\n"); return blanc;}
	}
	
/*****************************Ouvrir_Milieu************************************/

void Ouvrir_Milieu(int largeur, int profondeur, Porte_Sit InterSit, char *TabColor[3], float p, int precision, MlvStruct *X_ecran) {
	printf(">>Ouvrir_Milieu\n");
	int i,j;
	float c=1.0;
	Porte_Sit Sit_Memoire=InterSit;
	
	draw_filled_rectangle(BordG-225,110,100,4,"white",X_ecran);
	draw_rectangle(BordG-225,110,100,4,"black",X_ecran);
	
	for(j=0;j<profondeur; InterSit=Sit_Memoire, j++) {
		Sit_Memoire=InterSit->SiteBas;
		for(i=0;i<largeur; InterSit=InterSit->SiteDroit, i++) {
		c++;
		draw_filled_rectangle(BordG-224,113,(int)((c/((float)(RES*RES)))*99),1,"red3",X_ecran);
		draw_filled_rectangle(BordG-224,112,(int)((c/((float)(RES*RES)))*99),1,"red3",X_ecran);
		draw_filled_rectangle(BordG-224,111,(int)((c/((float)(RES*RES)))*99),1,"pink1",X_ecran);
		if (i%RES==0) display_window(X_ecran);
		InterSit->mat=Mat_Aleat(p, precision);
		draw_filled_rectangle(Portex+2*(InterSit->lieu.x),Portey+2*(InterSit->lieu.y),1,1,TabColor[InterSit->mat], X_ecran);
		}
		}
	}

/*****************************Initialiser_Milieu************************************/

void Initialiser_Milieu(int largeur, int profondeur, Porte_Sit InterSit) {
	printf(">>Ouvrir_Milieu\n");
	int i,j;
	Porte_Sit Sit_Memoire=InterSit;
	
	for(j=0;j<profondeur; InterSit=Sit_Memoire, j++) {
		Sit_Memoire=InterSit->SiteBas;
		for(i=0;i<largeur; InterSit=InterSit->SiteDroit, i++) 
		InterSit->mat=blanc;
		}
	}

/***************************Percolation_Milieu*********************************/

void Percoler_Milieu(Porte_Sit InterSit, char *TabColor[3], MlvStruct *X_ecran) {
	printf(">>Percoler_Milieu\n");
	if (InterSit!=NULL){
	if (InterSit->mat==bleu) {
	InterSit->mat=rouge;
	draw_filled_rectangle(Portex+2*(InterSit->lieu.x),Portey+2*(InterSit->lieu.y),1,1,TabColor[InterSit->mat], X_ecran);
	/*le décalage est en demi-pixel et les dimensions sont en pixels*/
	display_window(X_ecran);
	Percoler_Milieu(InterSit->SiteDroit, TabColor, X_ecran);
	Percoler_Milieu(InterSit->SiteBas, TabColor, X_ecran);
	Percoler_Milieu(InterSit->SiteGauche, TabColor, X_ecran);
	Percoler_Milieu(InterSit->SiteHaut, TabColor, X_ecran);
	}
	}
	}

/*****************************Lancer_Percolation*******************************/

void Lancer_Percolation(Porte_Sit InterSit, char *TabColor[3], MlvStruct *X_ecran) {
	printf(">>Lancer_Percolation\n");
	for(;InterSit!=NULL;InterSit=InterSit->SiteDroit) {
	printf(">>Lancer_Percolation: TOC\n");
	if (InterSit->mat==bleu) Percoler_Milieu(InterSit, TabColor, X_ecran);
	}
	}

/*******************************Affiche_Milieu*********************************/

void Affiche_Milieu(int largeur, int profondeur, Porte_Sit InterSit, char *TabColor[3], MlvStruct *X_ecran) {
	printf(">>Affiche_Milieu\n");
	int i,j;
	float c=1.0;
	Porte_Sit Sit_Memoire=InterSit;
	
	draw_filled_rectangle(BordG-225,110,100,4,"white",X_ecran);
	draw_rectangle(BordG-225,110,100,4,"black",X_ecran);
	
	for(j=0;j<profondeur; InterSit=Sit_Memoire, j++) {
		Sit_Memoire=InterSit->SiteBas;
		for(i=0;i<largeur; InterSit=InterSit->SiteDroit, i++) {
		c++;
		draw_filled_rectangle(BordG-224,113,(int)((c/((float)(RES*RES)))*99),1,"red3",X_ecran);
		draw_filled_rectangle(BordG-224,112,(int)((c/((float)(RES*RES)))*99),1,"red3",X_ecran);
		draw_filled_rectangle(BordG-224,111,(int)((c/((float)(RES*RES)))*99),1,"pink1",X_ecran);
		if (i%RES==0) display_window(X_ecran);
		draw_filled_rectangle(Portex+2*i,Portey+2*j,1,1,TabColor[InterSit->mat], X_ecran);
		/*le décalage est en demi-pixel et les dimensions sont en pixels*/
		}
		}
	}

/**********************************Option**************************************/	

int Option(int *x, int *y, Rect *Clavier, int Nbr, MlvStruct *X_ecran) {
	printf(">>Option\n");
	int i;
	mouse_wait(x,y,X_ecran);
	for(i=1;i<=Nbr;i++)
	if (*x >= Clavier[i-1].Gx && *x <= Clavier[i-1].Dx 
	&&  *y >= Clavier[i-1].By && *y <= Clavier[i-1].Hy) 
	return i;
	return -1;
	}

/*******************************Affiche_Console********************************/	

void Affiche_Console(Rect *Clavier, char **NomClavier, int n, MlvStruct *X_ecran) {
	printf(">>Affiche_Console\n");
	int i;
	for(i=0;i<n;i++) {
	draw_filled_rectangle (Clavier[i].Gx, Clavier[i].By, Clavier[i].Dx-Clavier[i].Gx,Clavier[i].Hy-Clavier[i].By, "gray", X_ecran);
	draw_string(5+Clavier[i].Gx, Clavier[i].Hy-5, NomClavier[i], "6x13", "black",X_ecran);
	}
	}	
		
/******************************************************************************/
/*MAIN*/
/******************************************************************************/
int main(int argc, char *argv[]) {

printf(">>main\n");

Porte_Sit PorteZero=NULL;
char *TableauCouleur[3]={"gray","blue","red"};
float p=0.50;
int Xmouse, Ymouse;
int h1=100,h2=250,h3=290,h4=400,h5=65;

Rect Clavier[4]={ {BordG,h1,BordG+50,h1+25},{BordG,h2,BordG+50,h2+25},{BordG,h3,BordG+50,h3+25},{BordG,h4,BordG+50,h4+25} };
char *NomClavier[4]={"distrib", "+0.001","-0.001","exit" };

MlvType  X_ecran;
int width = W;
int hight = H;
char *display_name=NULL;

create_window(display_name, width, hight, &X_ecran);
char titre[51]="Percolation Simulator (copyright Remy Tuyeras)";
name_window(titre, &X_ecran);

XPoint ptsh1_1[6] ={ {BordG-3,h1+25+3},{BordG-3,h1-3},{BordG+50+3,h1-3},{BordG+50,h1}, {BordG,h1}, {BordG,h1+25} };
XPoint ptsh1_2[6] ={ {BordG-3,h1+25+3},{BordG+50+3,h1+25+3},{BordG+50+3,h1-3},{BordG+50,h1}, {BordG+50,h1+25}, {BordG,h1+25} };
XPoint ptsh2_1[6] ={ {BordG-3,h2+25+3},{BordG-3,h2-3},{BordG+50+3,h2-3},{BordG+50,h2}, {BordG,h2}, {BordG,h2+25} };
XPoint ptsh2_2[6] ={ {BordG-3,h2+25+3},{BordG+50+3,h2+25+3},{BordG+50+3,h2-3},{BordG+50,h2}, {BordG+50,h2+25}, {BordG, h2+25}};
XPoint ptsh3_1[6] ={ {BordG-3,h3+25+3},{BordG-3,h3-3},{BordG+50+3,h3-3},{BordG+50,h3}, {BordG,h3}, {BordG,h3+25} };
XPoint ptsh3_2[6] ={ {BordG-3,h3+25+3},{BordG+50+3,h3+25+3},{BordG+50+3,h3-3},{BordG+50,h3}, {BordG+50,h3+25}, {BordG, h3+25} };
XPoint ptsh4_1[6] ={ {BordG-3,h4+25+3},{BordG-3,h4-3},{BordG+50+3,h4-3},{BordG+50,h4}, {BordG,h4}, {BordG,h4+25} };
XPoint ptsh4_2[6] ={ {BordG-3,h4+25+3},{BordG+50+3,h4+25+3},{BordG+50+3,h4-3},{BordG+50,h4}, {BordG+50,h4+25}, {BordG, h4+25} };
XPoint ptsh5_1[6] ={ {BordG-3,h5+25+3},{BordG-3,h5-3},{BordG+50+3,h5-3},{BordG+50,h5}, {BordG,h5}, {BordG,h5+25} };
XPoint ptsh5_2[6] ={ {BordG-3,h5+25+3},{BordG+50+3,h5+25+3},{BordG+50+3,h5-3},{BordG+50,h5}, {BordG+50,h5+25}, {BordG, h5+25} };

	Affiche_Console(Clavier,NomClavier, 4, &X_ecran);
	draw_filled_polygone(ptsh1_1, 6, "LightGray",&X_ecran);
	draw_filled_polygone(ptsh1_2, 6,"DimGrey", &X_ecran);
	draw_filled_polygone(ptsh2_1, 6, "LightGray",&X_ecran);
	draw_filled_polygone(ptsh2_2, 6,"DimGrey", &X_ecran);
	draw_filled_polygone(ptsh3_1, 6, "LightGray",&X_ecran);
	draw_filled_polygone(ptsh3_2, 6,"DimGrey", &X_ecran);
	draw_filled_polygone(ptsh4_1, 6, "LightGray",&X_ecran);
	draw_filled_polygone(ptsh4_2, 6,"DimGrey", &X_ecran);
	display_window(&X_ecran);

char Param[20];
int fin=0;

	sprintf(Param, "p=%f", p);
	Param[7]='\0';
	draw_rectangle (BordG, 150, 60 , 30, "gray", &X_ecran);
	draw_string(BordG+5, 170, Param, "6x13", "black",&X_ecran);
	display_window(&X_ecran);
	
	Creer_Milieu(RES, RES, &PorteZero,&X_ecran);
	Affiche_Milieu(RES, RES, PorteZero, TableauCouleur, &X_ecran);
	display_window(&X_ecran); /*necessaire*/

while(!fin==1) {


switch(Option(&Xmouse,&Ymouse,Clavier,4,&X_ecran)) {
	
	case 1 : 
	
	Clavier[0].Gx++;
	Clavier[0].Hy--;
	Affiche_Console(Clavier,NomClavier, 4, &X_ecran);	
	draw_filled_polygone(ptsh1_1, 6, "DimGrey",&X_ecran);
	draw_filled_polygone(ptsh1_2, 6, "LightGray", &X_ecran);
	display_window(&X_ecran);
	
	wait_micros(100000);
	
	Clavier[0].Gx--;
	Clavier[0].Hy++;
	Affiche_Console(Clavier,NomClavier, 4, &X_ecran);
	draw_filled_polygone(ptsh1_1, 6, "LightGray",&X_ecran);
	draw_filled_polygone(ptsh1_2, 6,"DimGrey", &X_ecran);
	display_window(&X_ecran);
	
	
	Initialiser_Milieu(RES, RES, PorteZero);
	Ouvrir_Milieu(RES, RES, PorteZero, TableauCouleur, p, 10000, &X_ecran);
	display_window(&X_ecran); /*necessaire*/
	
	draw_filled_rectangle (BordG, h5, 50, 25, "aquamarine3", &X_ecran);
	draw_string(BordG+5, h5+20, "LAUNCH" , "6x13", "black", &X_ecran);
	draw_filled_polygone(ptsh5_1, 6, "aquamarine1",&X_ecran);
	draw_filled_polygone(ptsh5_2, 6,"aquamarine4", &X_ecran);
	display_window(&X_ecran);
	do{
	mouse_wait(&Xmouse,&Ymouse,&X_ecran);
	}while(Xmouse < BordG || Xmouse > BordG+50 
	||  Ymouse < 70 || Ymouse > 90);
	
	draw_filled_rectangle (BordG, h5, 50, 25, "aquamarine3", &X_ecran);
	draw_string(BordG+1+5, h5+20-1, "LAUNCH" , "6x13", "black", &X_ecran);
	draw_filled_polygone(ptsh5_1, 6, "aquamarine4",&X_ecran);
	draw_filled_polygone(ptsh5_2, 6, "aquamarine1", &X_ecran);
	display_window(&X_ecran);
	
	wait_micros(100000);
	
	draw_filled_rectangle (BordG, h5, 50, 25, "aquamarine3", &X_ecran);
	draw_string(BordG+5, h5+20, "LAUNCH" , "6x13", "black", &X_ecran);
	draw_filled_polygone(ptsh5_1, 6, "aquamarine1",&X_ecran);
	draw_filled_polygone(ptsh5_2, 6,"aquamarine4", &X_ecran);
	display_window(&X_ecran);
	
	Lancer_Percolation(PorteZero, TableauCouleur, &X_ecran);
	
	draw_filled_polygone(ptsh5_1, 6, "white",&X_ecran);
	draw_filled_polygone(ptsh5_2, 6,"white", &X_ecran);
	draw_filled_rectangle (BordG, h5, 50, 25, "white", &X_ecran);
	display_window(&X_ecran);
	
	break;
	
	case 2 : 
	
	Clavier[1].Gx++;
	Clavier[1].Hy--;
	Affiche_Console(Clavier,NomClavier, 4, &X_ecran);
	draw_filled_polygone(ptsh2_1, 6, "DimGrey",&X_ecran);
	draw_filled_polygone(ptsh2_2, 6, "LightGray", &X_ecran);
	display_window(&X_ecran);
	
	wait_micros(50000);
	
	Clavier[1].Gx--;
	Clavier[1].Hy++;
	Affiche_Console(Clavier,NomClavier, 4, &X_ecran);
	draw_filled_polygone(ptsh2_1, 6, "LightGray",&X_ecran);
	draw_filled_polygone(ptsh2_2, 6,"DimGrey", &X_ecran);
	display_window(&X_ecran);
	
	p+=0.001;
	if (p>1) p=1.00;
	sprintf(Param, "p=%f", p);
	Param[7]='\0';
	draw_filled_rectangle (BordG, 150, 60 , 30, "white", &X_ecran);
	draw_rectangle (BordG, 150, 60 , 30, "gray", &X_ecran);
	draw_string(BordG+5, 170, Param, "6x13", "black",&X_ecran);
	display_window(&X_ecran);
	break;
	
	case 3 : 
	
	Clavier[2].Gx++;
	Clavier[2].Hy--;
	Affiche_Console(Clavier,NomClavier, 4, &X_ecran);
	draw_filled_polygone(ptsh3_1, 6, "DimGrey",&X_ecran);
	draw_filled_polygone(ptsh3_2, 6, "LightGray", &X_ecran);
	display_window(&X_ecran);
	
	wait_micros(50000);
	
	Clavier[2].Gx--;
	Clavier[2].Hy++;
	Affiche_Console(Clavier,NomClavier, 4, &X_ecran);
	draw_filled_polygone(ptsh3_1, 6, "LightGray",&X_ecran);
	draw_filled_polygone(ptsh3_2, 6,"DimGrey", &X_ecran);
	display_window(&X_ecran);
	
	p-=0.001;
	if (p<0) p=0.00;
	sprintf(Param, "p=%f", p);
	Param[7]='\0';
	draw_filled_rectangle (BordG, 150, 60 , 30, "white", &X_ecran);
	draw_rectangle (BordG, 150, 60 , 30, "gray", &X_ecran);
	draw_string(BordG+5, 170, Param, "6x13", "black",&X_ecran);
	display_window(&X_ecran);
	break;
	
	case 4 : 
	
	Clavier[3].Gx++;
	Clavier[3].Hy--;
	Affiche_Console(Clavier,NomClavier, 4, &X_ecran);
	draw_filled_polygone(ptsh4_1, 6, "DimGrey",&X_ecran);
	draw_filled_polygone(ptsh4_2, 6, "LightGray", &X_ecran);
	display_window(&X_ecran);
	
	wait_micros(100000);
	
	Clavier[3].Gx--;
	Clavier[3].Hy++;
	Affiche_Console(Clavier,NomClavier, 4, &X_ecran);
	draw_filled_polygone(ptsh4_1, 6, "LightGray",&X_ecran);
	draw_filled_polygone(ptsh4_2, 6,"DimGrey", &X_ecran);
	display_window(&X_ecran);
	
	fin=1;
	break;
	
	default : break;
	
	}
	}

Liberer_Milieu(RES, RES, &PorteZero);


return 0;
}
/******************************************************************************/
/*FIN*/
/******************************************************************************/




