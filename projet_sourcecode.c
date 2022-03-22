#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct ouvrages* pouvrages;
typedef struct ouvrages
{
       char cote[25];
       char titre[25];
       int NE;
       int NEP;
       pouvrages suiv;
}ouvrages;


typedef struct date
{
        int jour;
        int mois;
        int annee;
}date;


typedef struct etudiant* petudiant;
typedef struct etudiant
{
    int mat;
    char abo;
    int pen;
    petudiant suiv;
} etudiant;


typedef struct eprnt* peprnt;
typedef struct eprnt{
       int mat;
       char cote[25];
       date DE;
       date DR;
       peprnt suiv;}eprnt;
/*strcmp: comparaison 2 chainnes de caracteres
exemple : strcmp(chainne1,chainne2) si =0 identique sinon non identique*/
typedef struct ElementFile
{
      int mat;
      char cote[25];
      date DE;
      date DR;
}ElementFile;


typedef struct attente* pattente;
typedef struct attente{
      ElementFile elementFile;
      pattente suiv;}attente;

typedef struct File
{
	pattente Tete,Queue;
}File;


void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

void read(char *chaine, int taille)
{
    char *positionEntree = NULL;

    if (fgets(chaine, taille, stdin) != NULL)
    {
        positionEntree = strchr(chaine, '\n');
        if (positionEntree != NULL) *positionEntree = '\0';
        else                         viderBuffer();
    }
    else viderBuffer();
}


void initfile(File *F)
{
    (*F).Tete=NULL;
    (*F).Queue=NULL;
}

void Enfiler(File *F, ElementFile y){
 pattente p;
 p=malloc(sizeof(attente));
 p->elementFile=y;
 p->suiv=NULL;
 if ((*F).Tete==NULL){(*F).Tete=p;
                      (*F).Queue=p;}
 else {(*F).Queue->suiv=p;
       (*F).Queue=p;}
}

void Defiler(File *F, ElementFile *x){

	pattente p;
	p=(*F).Tete; *x=p->elementFile;
	if((*F).Tete==(*F).Queue) { (*F).Tete=NULL;(*F).Queue=NULL;}
	else
	{(*F).Tete=(*F).Tete->suiv;}
	free(p);
}


bool Filevide(File F){

    if (F.Tete==NULL) return true;
    else              return false;
}


bool checkbook(File *F, char *cote)
{
	File F1;
	ElementFile x;
	bool find=false;
	initfile(&F1);
	while(!Filevide(*F)){
	    Defiler(&(*F),&x);
	    Enfiler(&F1, x);
	    if(strcmp(x.cote, cote)==0) find=true;}
	*F=F1;
	return find;
}

bool Dispoetudiant(petudiant E, int matricule)
{
	 while(E!=NULL && matricule!=E->mat) E=E->suiv;
     if(E!=NULL) return true;
     else return false;
}


bool DispoLivre (pouvrages O , char *cote){
   while(O!=NULL && strcmp(cote,O->cote)!=0) O=O->suiv;
   if(O==NULL) return false;
   else if (O->NE==0) return false;
   else return true;
}
int Bissextile(int annee)
{
	if(annee%400==0 || (annee%4==0 && annee%100!=0)) return 1;
	else                                             return 0;
}
int ValideDate(date date){
	switch(date.mois){
		case 1: if(date.jour>0 && date.jour<=31) return 1; break;
		case 2: if(Bissextile(date.annee)) {if(date.jour>0 && date.jour<=29) return 1;}
				else {if(date.jour>0 && date.jour<=28) return 1;} break;
		case 3: if(date.jour>0 && date.jour<=31) return 1; break;
		     case 4: if(date.jour>0 && date.jour<=30) return 1; break;
		case 5: if(date.jour>0 && date.jour<=31) return 1; break;
		     case 6: if(date.jour>0 && date.jour<=30) return 1; break;
		case 7: if(date.jour>0 && date.jour<=31) return 1; break;
		     case 8: if(date.jour>0 && date.jour<=31) return 1; break;
		case 9: if(date.jour>0 && date.jour<=30) return 1; break;
             case 10:if(date.jour>0 && date.jour<=31) return 1; break;
		case 11:if(date.jour>0 && date.jour<=30) return 1; break;
		     case 12:if(date.jour>0 && date.jour<=31) return 1; break;
		default: return 0;}
	return 0;
}
int ComparerDate(date date1, date date2){
	if(date1.annee>date2.annee) return 1;
	else if(date1.annee<date2.annee) return -1;
    else if(date1.mois>date2.mois) return 1;
    else if(date1.mois<date2.mois) return -1;
    else if(date1.jour>date2.jour) return 1;
    else if(date1.jour<date2.jour) return -1;
    else                      return 0;
 }

int CalculeNbrJours(date date){
    int nombre=date.jour,i;
    for(i=1;i<date.mois;i++){
       	switch(i)
	  	{
	  		case 1: nombre=nombre+31; break;
	  		    case 2: if(Bissextile(date.annee)) nombre=nombre+29;
				    else                        nombre=nombre+28; break;
		    case 3: nombre=nombre+31; break;
		            case 4: nombre=nombre+30; break;
		    case 5: nombre=nombre+31; break;
                    case 6: nombre=nombre+30; break;
		    case 7: nombre=nombre+31; break;
		            case 8: nombre=nombre+31; break;
		    case 9: nombre=nombre+30; break;
		            case 10:nombre=nombre+31; break;
		    case 11:nombre=nombre+30; break;
		            case 12:nombre=nombre+31; break;
		}
	}
	return nombre;
 }
int DiffDate(date date1, date date2)
{
      date date; int i,nbrjours;
	  if(ComparerDate(date1, date2)== -1) {date=date1; date1=date2; date2=date;}
	  if(date1.annee==date2.annee) nbrjours= CalculeNbrJours(date1)-CalculeNbrJours(date2);
	  else
	  {
	  	if(Bissextile(date2.annee)) nbrjours= 366-CalculeNbrJours(date2)+CalculeNbrJours(date1);
	  	else                        nbrjours= 365-CalculeNbrJours(date2)+CalculeNbrJours(date1);
	  	for(i=date2.annee+1;i<date1.annee;i++)
	    {
	  	        if(Bissextile(i)) nbrjours=nbrjours+366;
	            else              nbrjours=nbrjours+365;
	    }
	  }
	  return nbrjours;
}
void ajouter_ouv(File *C,File *PR,pouvrages *O,peprnt *M){
pouvrages p,q,t;
File C1,PR1;
int i,n;
ElementFile x,y;
peprnt d;
char cote[25];
p=malloc(sizeof(ouvrages));
printf("donnez la cote du livre svp :");gets(p->cote);
printf("donnez le nombre d'exemplaire svp : ");
        scanf( "%d", &p->NE);getchar();
if (DispoLivre ( *O , p->cote)==false){
        printf("donnez le nom de ce livre svp :");
        read(p->titre,25);
        (p->NEP)=0;
        p->suiv=NULL;

        if ((*O)==NULL){
            (*O)=p;}
        else{q=(*O);
            while (q->suiv!=NULL) q=q->suiv;
                q->suiv=p;}
        q=NULL;}
        else {
             strcpy(cote,p->cote);
             q=(*O);
             while(strcmp(cote,q->cote)!=0){q=q->suiv;}
             q->NE=(q->NE)+(p->NE);}


      if(checkbook(PR,p->cote)==true && p->NE>0) {

           while (!Filevide(*PR)){
                 Defiler(&(*PR),&x);
                 if (strcmp(x.cote,p->cote)!=0)
                 Enfiler(&PR1, x);
                 else y=x; }

           *PR=PR1;
           p->NEP++;
           p->NE--;
            printf("\nun etudiant a ete defiler de la file des premium !");
           
					  //ajouter l'element defiler a la listes des emprunts
           d=malloc(sizeof(eprnt));
          strcpy(d->cote,y.cote);
           d->mat=y.mat;
           d->DE.jour= y.DE.jour;
           d->DE.mois=y.DE.mois;
           d->DR.annee=y.DE.annee;
           d->DR.jour=y.DR.jour;
           d->DR.mois=y.DR.mois;
           d->DE.annee=y.DR.annee;
           d->suiv=NULL;
            if ((*M)==NULL)(*M)=d;
              else{ d->suiv=(*M);
                    (*M)=d; }

                     printf("\nemprunt ajoute succesivement ! ");
          
                       

      }
      if(checkbook(C,p->cote)==true && p->NE>0 ) {
        while (!Filevide(*C)){
        Defiler(&(*C),&x);
        if (strcmp(x.cote,p->cote)!=0)
            Enfiler(&C1, x);
            else y=x;
           }
           *C=C1;
           p->NEP++;
           p->NE--;
           printf("\nun etudiant a ete defiler de la file des classic !");
            
					  //ajouter l'element defiler a la listes des emprunts
           d=malloc(sizeof(eprnt));
           strcpy(d->cote,y.cote);
           d->mat=y.mat;
           d->suiv=NULL;
            if ((*M)==NULL)(*M)=d;
              else{ d->suiv=(*M);
                    (*M)=d; }
                    
                     printf("\nemprunt ajoute succesivement ! ");
}}



// pour afficher
void affiche_ouv(pouvrages O){
    pouvrages p;
p=O;
if(O==NULL)printf("la liste des ouvrages est vide ! \n");
else {while(p!=NULL){
    printf("voici le titre du livre : %s\n",p->titre);
    printf("voici la cote du livre : %s\n",p->cote);
    printf("voici le nombre d'exemplaire : %d\n", p->NE);
    printf("voici le nombre d'exemplaire emprunte : %d\n",p->NEP);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    p=p->suiv;}
}
}

void Supprime_ouv(pouvrages *O){
	pouvrages p=NULL,q=NULL; char cote[25];
	printf("Donnez la cote du livre:"); read(cote,25);
	if(*O==NULL || !DispoLivre(*O,cote)) printf("\nLe livre que vous voulez supprimer n'existe pas !");
		else
		{
		  p=*O;
		  if(strcmp((*O)->cote, cote)==0) *O=(*O)->suiv;
		  else
		  {
			q=*O;
			while(q->suiv!=NULL && strcmp(q->suiv->cote, cote)!=0) q=q->suiv;
			p=q->suiv; q->suiv=p->suiv;
		  }
		  free(p); p=NULL; q=NULL;
		  printf("\n le livre a etait supprime ! ");

		}
	}


void DispoExmpe(pouvrages *O ){
   pouvrages p=*O;
   char cote[25];
   printf("donnez la cote du livre que vous voulez verifier la disponibilite de ces exemplaire : ");read(cote,25);
   while(p!=NULL && strcmp(cote,p->cote)!=0) p=p->suiv;
   if(p==NULL) printf("\nil n'y a pas d'ouvrage portant cette cote");
   else if(p->NE==0) printf("\nil n'y a pas d'exemplaire disponible ! ");
   else printf("\nil y'a des exemplaire pour emprunter ");
}

bool DispoExmp(pouvrages O , char *cote){
   while(O!=NULL && strcmp(cote,O->cote)!=0) O=O->suiv;
   if(O==NULL) return false;
   else if(O->NE==0) return false;
   else if(O->NE!=0) return true;
}


void ajout_etudiant(petudiant *E ){
    petudiant t=*E,p;
    int mat;char abo;
    p=malloc(sizeof(etudiant));
    printf("\ndonnez le type d'abonement de cet etudiant : ");scanf("%s",&p->abo);
	printf("donnez le matricule de l'etudiant qui vous voulez ajouter : ");scanf("%d", &p->mat);getchar();
    p->suiv=NULL;
    p->pen=0;
    if(Dispoetudiant(*E,p->mat)==false){
        if(*E==NULL){
            *E=p;
        }
    else{
        while(t->suiv != NULL){
        t=t->suiv;}
            t->suiv=p;
        }
    } else {
        printf("\ncet etudiant existe deja ! ");
        free(p);
    }
}


void cons_membre(petudiant E ){
  int mat;
  printf("Donnez le metricule de l'etudiant:"); scanf("%d",&mat); getchar();
  while(E!=NULL && E->mat!=mat) E=E->suiv;
  if(E==NULL) printf("\nMembre introuvable");
  else printf("\nLe type d'abonnement de cet etudiant est: %c",E->abo);
}


void cons_pen(petudiant *E){
petudiant p;
p=*E;
if(*E==NULL)printf("la liste des etudiant est vide ! \n");
else {while(p!=NULL){
    printf("l'etudiant avec le matricule %d ", p->mat);
    printf("\nvoici le nombre de ces penalité : %d\n",p->pen);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    p=p->suiv;}
}
}


//suprimer un membre de la listes des etudiant
void sup_membre(petudiant *E)
{
    petudiant p=*E,q; int mat;
    printf("\nDonnez le matricule de l'etudiant:"); scanf("%d",&mat); getchar();
    if(*E==NULL) printf("\nLa liste est vide.");
    else
    {
    	if((*E)->mat==mat) *E=(*E)->suiv;
    	else
    	{
    		q=*E;
    		while(q->suiv!=NULL && q->suiv->mat!= mat) q=q->suiv;
    		if(q->suiv==NULL) printf("\nCet etudiant n'existe pas.");
    		else {p=q->suiv; q->suiv=p->suiv;}
    		free(p); q=NULL;
    		printf("l'etudiant a ete supprimer");
		}
	}
}


void modifier_membre(petudiant E){
    char x;
    int mat;
    printf("\ndonnez le matricule de l'etudiant que vous voulez modifier : ");scanf("%d", &mat);
    if(Dispoetudiant((E),mat)==true){
         while (E!=NULL && mat!=E->mat){
                E=E->suiv;
    }
        printf("\ndonner le nouveau type d'abonnement de cet etudiant : ");scanf("%c",&x);getchar();
        E->abo=x;
        if(x == 'p') E->pen=0;
    
 }
 else {
     printf("\n cet etudiant n'existe pas ou la liste est vide");
 }
}


void rtr_empr(File *C,File *PR,peprnt (*M),pouvrages(*O))
{
	bool resume;
    File C1,PR1;
    ElementFile x,y;
    peprnt p,q;
    pouvrages t;
   
    char cote[25];int mat;
    p=(*M);
    printf("\ndonnez la cote et le matricule de l'emprunt que vous voulez retourner ");
    printf("\nla cote : ");read(cote,25);
    printf("\nle matricule : ");scanf("%d",&mat);getchar();
    if((*M)->mat==mat&&strcmp((*M)->cote, cote)==0){
    (*M)=(*M)->suiv;
    resume=true;
}
else{

     while (p!=NULL && (strcmp(cote,p->cote)!=0 || p->mat!=mat)){
        q=p;
        p=p->suiv;
    }
    if (p==NULL){
        printf("cette cote n'existe pas \n");
        resume=false;
    }
    else{
        q->suiv=p->suiv;
        free(p);
        resume=true;
    }
    }


    //pour retourner un exemplaire emprinté
    if(resume==true){
	

           pouvrages r,s;
           r=(*O);
        while(r!=NULL && strcmp(cote,p->cote)!=0 ){
        r=r->suiv;
       }
       if ( r->NEP>0){
       r->NEP=r->NEP-1;
       r->NE=r->NE+1;
        

       }
       printf("\n l'emprunt retourner successivement");


      if(checkbook(PR,r->cote)==true && r->NE>0 ) {

           while (!Filevide(*PR)){
        Defiler(&(*PR),&x);
        if (strcmp(x.cote,p->cote)!=0)
	    Enfiler(&PR1,x);
	     else y=x;
           }
          
		   
           *PR=PR1;
           printf("\n un etudiant a ete defiler de la file des premium");
           //ajouter l'element defiler a la listes des emprunts
           p=malloc(sizeof(eprnt));
           strcpy(p->cote,y.cote);
           p->mat=y.mat;
           p->DE.jour= y.DE.jour;
           p->DE.mois=y.DE.mois;
           p->DR.annee=y.DE.annee;
           p->DR.jour=y.DR.jour;
           p->DR.mois=y.DR.mois;
           p->DE.annee=y.DR.annee;
           p->suiv=NULL;
            if ((*M)==NULL)(*M)=p;
              else{ p->suiv=(*M);
                    (*M)=p; }
                    t=(*O);
                     while (strcmp((t)->cote,cote)!=0) (t)=(t)->suiv;
                                 (t)->NEP=(t)->NEP+1;
                                 (t)->NE=(t)->NE-1;
                     printf("\nemprunt ajoute succesivement ! ");
                     
                    
      }
      if(checkbook(C,cote)==true && r->NE>0) {

           while (!Filevide(*C)){
        Defiler(&(*C),&x);
        if (strcmp(x.cote,p->cote)!=0)
	    Enfiler(&C1, x);
	    else y=x;
           }
           
           *C=C1;
           printf("\n un etudiant a ete defiler de la file des classiques");
            //ajouter l'element defiler a la listes des emprunts
           p=malloc(sizeof(eprnt));
           strcpy( p->cote,y.cote);
           p->mat=y.mat;
           p->DE.jour= y.DE.jour;
           p->DE.mois=y.DE.mois;
           p->DR.annee=y.DE.annee;
           p->DR.jour=y.DR.jour;
           p->DR.mois=y.DR.mois;
           p->DE.annee=y.DR.annee;
           p->suiv=NULL;
            if ((*M)==NULL)(*M)=p;
              else{ p->suiv=(*M);
                    (*M)=p; }
                     t=(*O);
                    while (strcmp((t)->cote,cote)!=0) (t)=(t)->suiv;
                                 (t)->NEP=(t)->NEP+1;
                                 (t)->NE=(t)->NE-1;
                     printf("\nemprunt ajoute succesivement ! ");
           
       }
    }
}


bool cherche_pen(petudiant E , int mat){

           while(E!=NULL && mat!=E->mat)  E=E->suiv;
           if (E->pen<7) return true;
           else  return false;
}

//modifier un emprint
void modifier_empr(peprnt (*M),petudiant (*E)){
    peprnt p,q;
    char cote [25];
    int matricule;
    p=(*M);
    printf("donner la cote");
    read(cote,25);
    printf("donner la matricule");
    scanf("%d",&matricule);
    if(Dispoetudiant(*E,matricule)==true){

    while ((p!=NULL) && ((strcmp(cote,p->cote)!=0)|| p->mat!=matricule)){
        p=p->suiv;
    }
    if (p==NULL){
        printf("il n'y a pas cette emprunte\n");
    }

    else{
        printf("Donner le jour de retour \n");
        scanf("%d", &p->DR.jour);
        printf("Donner le mois d'emprint \n");
        scanf("%d", &p->DR.mois);
        printf("Donner l'année de l'emprint \n");
        scanf("%d", &p->DR.annee);
        if(ValideDate(p->DR)==0){
            printf("cette date n'est pas valide");

        }

        }

}
else printf("cet etudiant n'existe pas");
}

//supprimer une penalite
 void sup_pen(petudiant (E)){
     int mat;
     printf("\n donnez le matricule de l'etudiant que vous voulez le supprimer la penalite : ");scanf("%d",&mat);

    while ((E)->suiv!=NULL && mat!=(E)->mat){
        (E)=(E)->suiv;
    }
    if ((E)->suiv!=NULL){
        printf("ce matricule n'existe pas \n");
    }
    else{ 
        if ( (E)->pen>0){
        	(E)->pen = (E)->pen-1;
       printf("une penalite a ete supprimer ");
   }
       else 
       	printf("cet etudiant n'a aucune penalite");
	   
    }
		}
       


//maximum d'ouvrages a emprinter
void max_emprunt(pouvrages O)
{
    int cpt=0;
    cpt=cpt+(O->NE);
    while (O->suiv!=NULL)
    {
        O=O->suiv;
        cpt=cpt+(O->NE);
    }
    printf("voici le max d'ouvrages a emprunter : %d", cpt);
}


void cons_eprnt(peprnt M){
    int mat;char cote[25];
    if(M!=NULL)
    {
    printf("donnez la cote est le matricule de l'emprunt que vous voulez consulter \n");
    printf("le matricule : ");scanf("%d",&mat); getchar();
    printf("la cote : ");read(cote,25);

        while(M!=NULL && (M->mat!=mat || strcmp(cote,M->cote)!=0)) M=M->suiv;
        if ( M!=NULL){
        printf("\nvoici la date d'emprunt JJ/MM/AA: %d/%d/%d", M->DE.jour ,M->DE.mois ,M->DE.annee);
        printf("\nvoici la date de retour d'emprunt :%d/%d/%d", M->DR.jour ,M->DR.mois ,M->DR.annee);}

        else printf("l'emprunt n'existe pas");
    }
    else printf("la liste des emprunt est vide !");
}


void ajout_pen(petudiant *E){
   petudiant p=*E;int mat;
   char h [2];
   printf("\ndonnez le matricule de l'etudiant que vous voulez ajouter la penalite : ");scanf("%d",&mat);
   if(*E!=NULL){
    while(p!=NULL && p->mat!=mat){
        p=p->suiv;
    }
    if(p==NULL)printf("\ncet etudiant n'existe pas !");
    else if (p->abo=='p') printf("\n l'etudiant est de type d'abo premium impossible de ajouter la penalite !");
    else p->pen++; printf("la penalite a ete ajouter");
   }
}

void ajouter_empr(File *C,File *PR,pouvrages *O,petudiant *E,peprnt *M)
{
peprnt p,q;
ElementFile s;
pouvrages t=*O;
char cote[25],abo;
int matricule;
 printf("\nDonnez les informations de l'emprunt que vous voulez ajouter : ");
 printf("la cote : ");read(cote,25);
 printf("le matricule : ");scanf("%d", &matricule);getchar();
 printf("le type d'abonnement : ");scanf("%c", &abo);getchar();


  if (Dispoetudiant(*E,  matricule)==true){

     if (cherche_pen (*E, matricule)==true){

        if( DispoExmp( *O , cote)==true){
             p=malloc(sizeof(eprnt));
             strcpy(p->cote,cote);
             p->mat=matricule;
             do{
             printf("donnez la date d'aujourd'hui  JJ/MM/AA : ");
             scanf ( "%d", &p->DE.jour);getchar();scanf ( "%d", &p->DE.mois);getchar();scanf("%d", &p->DE.annee);getchar();
             printf("\nc'est quoi la date de retour JJ/MM/AA : ");
             scanf ( "%d", &p->DR.jour);getchar();scanf ( "%d", &p->DR.mois);getchar();scanf ( "%d", &p->DR.annee);getchar();
             }while(ValideDate(p->DE)==0 && ValideDate(p->DR)==0);
              p->suiv=NULL;
              if ((*M)==NULL)(*M)=p;
              else{ p->suiv=(*M);
                    (*M)=p; }
              while (strcmp((t)->cote,cote)!=0) (t)=(t)->suiv;
                                 (t)->NEP=(t)->NEP+1;
                                 (t)->NE=(t)->NE-1;
                     printf("\nemprunt ajoute succesivement ! ");
              }
         else{
                      s.mat=matricule;
                      strcpy( s.cote,cote);
                      s.DE.jour=p->DE.jour;
                      s.DE.mois=p->DE.mois;
                      s.DE.annee=p->DR.annee;
                      s.DR.jour=p->DR.jour;
                      s.DR.mois=p->DR.mois;
                      s.DR.annee=p->DE.annee;
                      if (abo=='c') {
                      Enfiler(C,s);
                      printf("\netudiant ajoute a la file d'attente classique");
                      }
                      else {
                      Enfiler(PR,s);
                      printf("\netudiant ajoute a la file d'attente premium ");}
             }
      }   else printf("\ncelui est interdit d'emprunter !");
  }    else printf("\nl'etudiant n'existe pas ! ");

}

void sup_empr(peprnt (*M),pouvrages(*O))
{
    peprnt p,q;
    int j,m,a,mat;
    char cote[25];
    printf("\ndonnez la cote et la matricule de l'emprunt que vous voulez supprimer ");
    printf("\nla cote : ");read(cote,25);
    printf("\nle matricule : ");scanf("%d", &mat);

if((*M)->mat==mat&&strcmp((*M)->cote, cote)==0){
    (*M)=(*M)->suiv;
}
else{

    p=(*M);
    while (p!=NULL && ((strcmp(cote,p->cote)!=0) || mat!=p->mat)){
        q=p;
        p=p->suiv;
    }
    if (p==NULL){
        printf("cet matricule n'existe pas \n");
    }
    else{
        q->suiv=p->suiv;
        free(p);

    //pour retourner un exemplaire emprinté

           pouvrages r,s;
           r=(*O);
        while(r->suiv!=NULL && strcmp(cote,p->cote)!=0 ){
        r=r->suiv;
       }
       if ( r->NEP>0){
       r->NEP=r->NEP-1;
       r->NE=r->NE+1;

       }
       else {
           printf("erreur! y a pas d'exemplaire emprinté \n");
       }
    }
    }
    }




int DureeEmp(peprnt liste)
{
	int matricule; char cote[25];
	printf("\nDonnez le matricule de l'etudiant "); scanf("%d", &matricule); getchar();
	printf("\nDonnez la cote du livre:"); read(cote,25);
	while(liste!=NULL && (liste->mat!=matricule || strcmp(liste->cote, cote)!=0)) liste=liste->suiv;
	if(liste==NULL) printf("\nErreur: emprunt introuvable.");
	else return DiffDate(liste->DE, liste->DR);
}

int main(int argc, char *argv[])
{   File PR,C;
    pouvrages O=NULL;petudiant E=NULL;peprnt M=NULL;initfile(&PR);initfile(&C);
    char cote[25];
    int choix,i,retourner=1;
    do{
    	printf("\n~~~~~~~~~~~~~~~~~~MENU~~~~~~~~~~~~~~~~~~\n");
    	printf("\ncliquez 1 pour ajouter un livre.");
    	printf("\ncliquez 2 pour consulter votre liste de livre.");
    	printf("\ncliquez 3 pour supprimer un livre.");
    	printf("\ncliquez 4 pour ajouter un etudiant.");
    	printf("\ncliquez 5 pour consulter un membre .");
    	printf("\ncliquez 6 pour supprimer un etudiant.");
    	printf("\ncliquez 7 pour modifier un etudiant.");
    	printf("\ncliquez 8 pour verifier la disponibilite des exemplaires.");
    	printf("\ncliquez 9 pour ajouter un emprunt.");
    	printf("\ncliquez 10 pour consulter un emprunt.");
    	printf("\ncliquez 12 pour retourner un emprunt.");
    	printf("\ncliquez 13 pour supprimer un emprunt.");
    	printf("\ncliquez 14 pour ajouter une penalite.");
    	printf("\ncliquez 15 pour supprimer une penalite.");
    	printf("\ncliquez 16 pour consulter une penalite.");
    	printf("\ncliquez 17 pour affichier la duree d'un emprunt.");
    	printf("\ncliquez 18 pour modifier un emprunt.");
		printf("\ncliquez 20 pour afficher le max d'ouvrages a emprunter.");
		printf("\ncliquez 0 pour arreter le programme.");
        printf("\n\nVeuillez saisir votre choix:"); do{scanf("%d", &choix); getchar();}while(choix<0 || choix>21);
		switch(choix)
		{
			case 0:  retourner=0;                  break;
			case 1:  ajouter_ouv(&C,&PR,&O,&M);    break;
			case 2:  affiche_ouv(O);               break;
			case 3:  Supprime_ouv(&O);             break;
			case 4:  ajout_etudiant(&E);           break;
			case 5:  cons_membre(E);               break;
			case 6:  sup_membre(&E);               break;
			case 7:  modifier_membre(E);           break;
			case 8:  DispoExmpe(&O);               break;
			case 9:  ajouter_empr(&C,&PR,&O,&E,&M);break;
			case 10: cons_eprnt(M);               break;
			case 12: rtr_empr(&C,&PR,&M,&O);       break;
			case 13: sup_empr(&M,&O);              break;
			case 14: ajout_pen(&E);                break;
			case 15: sup_pen(E);                  break;
			case 16: cons_pen(&E);                 break;
			case 17: printf("la duree est %d jour",DureeEmp(M));break;
			case 18: modifier_empr(&M,&E);         break;
			case 20: max_emprunt(O);               break;
		}
	}while(retourner==1);
	return 0;
}



