#include "menuFunc.h"
#include "FileIO.h"
#include <time.h>

#define RON_TO_USD 0.21
#define RON_TO_EUR 0.20
#define EUR_TO_RON 4.98
#define EUR_TO_USD 1.06
#define USD_TO_RON 4.67
#define USD_TO_EUR 0.94

float conversieSuma(const char *moneda1, const char *moneda2, float suma)
{
    moneda1=strToUpr(moneda1);
    moneda2=strToUpr(moneda2);
    if(strcmp(moneda1,"RON")==0)
    {
        if(strcmp(moneda2,"EUR")==0)
            suma=suma*RON_TO_EUR;
        else if(strcmp(moneda2,"USD")==0)
            suma=suma*RON_TO_USD;
            else if(strcmp(moneda1,moneda2)==0)
                return suma;
                else{
                    printf("Nu s-a putut realiza conversia, va rugam reincercati!");
                    sleep(3);
                    return suma;
                }
    }
    else if(strcmp(moneda1,"EUR")==0)
    {
        if(strcmp(moneda2,"RON")==0)
            suma=suma*EUR_TO_RON;
        else if(strcmp(moneda2,"USD")==0)
            suma=suma*EUR_TO_USD;
            else if(strcmp(moneda1,moneda2)==0)
                return suma;
                else{
                    printf("Nu s-a putut realiza conversia, va rugam reincercati!");
                    sleep(3);
                    return suma;
                }
    }
    else if(strcmp(moneda1,"USD")==0)
    {
        if(strcmp(moneda2,"RON")==0)
            suma=suma*USD_TO_RON;
        else if(strcmp(moneda2,"EUR")==0)
            suma=suma*USD_TO_EUR;
            else if(strcmp(moneda1,moneda2)==0)
                return suma;
                else{
                    printf("Nu s-a putut realiza conversia, va rugam reincercati!");
                    sleep(3);
                    return suma;
                }
    }
    return suma;
}

char *strToLow(char *string){
    for(int i=0;i<strlen(string);i++){
        if(string[i]>='A'&&string[i]<='Z') string[i]+=32;
    }
    return string;
}

char *strToUpr(char *string){
    for(int i=0;i<strlen(string);i++){
        if(string[i]>='a'&&string[i]<='z') string[i]-=32;
    }
    return string;
}

int verificaIban(const char *filePath, const char *ibanToVerify)
{
    int gasit=0;
    FILE* file=openFile(filePath, "r");
    char *p;
    char header[100];
	char data[255];
	fgets(header,100,file);
	while (!feof(file) && gasit==0){
        fgets(data, 255, file);
		p=strtok(data,",");
		p=strtok(NULL,",");
		p=strtok(NULL,",");
		if(strcmp(ibanToVerify,p)==0)
            gasit=1;
	}
	fclose(file);
    return gasit;
}

char *generareIban(char *moneda){
    char *iban;
    iban=(char*)malloc(11*sizeof(char));
    strcpy(iban,moneda);
    if(strcmp(moneda,"RON")==0)
        strcat(iban,"54");
    else if(strcmp(moneda,"EUR")==0)
        strcat(iban,"82");
        else if(strcmp(moneda,"USD")==0)
            strcat(iban,"96");
     srand(time(NULL));
     char *numar;
     numar=(char*)malloc(6*sizeof(char));
     sprintf(numar,"%06d",rand()%10000);
     strcat(iban,numar);
     if(verificaIban("data.csv",iban)==0)
        return iban;
     else{
        while(verificaIban("data.csv",iban)!=0)
            iban=generareIban(moneda);
        return iban;
     }
     free(iban);
}

void creareCont(const char *nume, const char *prenume){
    Cont *cont;
    char monedaControl[4];
    char *nrIban;
    nrIban=(char*)malloc(10*sizeof(char));
    cont=(Cont*)malloc(sizeof(Cont));
    system("cls");
    printf("--------------------------------------\n");
    printf("|            Creare Cont             |\n");
    printf("--------------------------------------\n");
    strcpy(cont->numePers,nume);
    strcpy(cont->pnumePers,prenume);
    printf("Va rugam introduceti moneda (RON,EUR,USD): ");
    scanf("%s",monedaControl);
    char *monedaControlUpr;
    monedaControlUpr=strToUpr(monedaControl);
    strcpy(monedaControl,monedaControlUpr);
    if(strcmp(monedaControl,"RON")!=0 && strcmp(monedaControl,"EUR")!=0 && strcmp(monedaControl,"USD")!=0){
        while(strcmp(monedaControl,"RON")!=0 && strcmp(monedaControl,"EUR")!=0 && strcmp(monedaControl,"USD")!=0){
            printf("Moneda introdusa nu este acceptata, va rugam introduceti una din optiunile prezentate anterior!\n");
            scanf("%s", monedaControl);
            monedaControlUpr=strToUpr(monedaControl);
            strcpy(monedaControl,monedaControlUpr);
        }
    }
    nrIban=generareIban(monedaControl);
    strcpy(cont->IBAN,nrIban);
    strcpy(cont->moneda,monedaControl);
    cont->soldDisp=0;
    writeToCSV("data.csv",cont);
    free(cont);
}

int cautareCont(const char *filePath, const char *argPrimit1, const char *argPrimit2){
    int gasit=0;
    Cont *cont;
    cont=(Cont*)malloc(sizeof(Cont));
    FILE* file=openFile(filePath, "r");
    char *p;
    char header[100];
	char data[255];
	fgets(header,100,file);
    while (!feof(file) && gasit==0){
        fgets(data, 255, file);
		p=strtok(data,",");
		strcpy(cont->numePers,p);
		p=strtok(NULL,",");
		strcpy(cont->pnumePers,p);
        if(strcmp(cont->numePers,argPrimit1)==0 && strcmp(cont->pnumePers,argPrimit2)==0)
            gasit=1;
    }
    fclose(file);
    free(cont);
    return gasit;
}

void trimiteBani(Cont *contSursa, Cont *contDestinatar, float suma)
{
    char nume[20],pnume[20],iban[12],moneda[4],*p;
    char header[100],data[255];
    float sold,suma2;
    char *monedaSursa, *monedaDestinatar;
    monedaSursa=contSursa->moneda;
    monedaDestinatar=contDestinatar->moneda;
    FILE *oldFile=fopen("data.csv","r");
    FILE *newFile=fopen("temp.csv","w");
    fgets(header,100,oldFile);
    header[strlen(header)-1]='\0';
    fprintf(newFile,"%s",header);
    while(!feof(oldFile)){
        fgets(data,255,oldFile);
        p=strtok(data,",");
        strcpy(nume,p);
        p=strtok(NULL,",");
        strcpy(pnume,p);
        p=strtok(NULL,",");
        strcpy(iban,p);
        p=strtok(NULL,",");
        strcpy(moneda,p);
        p=strtok(NULL,",");
        sold=atof(p);
        if(strcmp(contSursa->IBAN,iban)==0){
            sold-=suma;
            fprintf(newFile,"\n%s,%s,%s,%s,%.2f",nume,pnume,iban,moneda,sold);
        }
        else if(strcmp(contDestinatar->IBAN,iban)==0){
            suma2=conversieSuma(monedaSursa,monedaDestinatar,suma);
            sold+=suma2;
            fprintf(newFile,"\n%s,%s,%s,%s,%.2f",nume,pnume,iban,moneda,sold);
        }
        else fprintf(newFile,"\n%s,%s,%s,%s,%.2f",nume,pnume,iban,moneda,sold);
    }
    fclose(oldFile);
    fclose(newFile);
    remove("data.csv");
    rename("temp.csv","data.csv");
}

void tranzactie(Cont **sirConturi,int lungimeSir)
{
    char nume[30],prenume[30];
    Cont **sirConturiDestinatar;
    int nrConturiDestinatar=0;
    float suma;
    int alegere;
    int contAlesDest,contAlesSursa;
    do{
        system("cls");
        printf("-------------------------------------\n");
        printf("|            Tranzactie             |\n");
        printf("-------------------------------------\n");
        for(int i=0;i<lungimeSir;i++){
            printf("Contul %d:\n",i+1);
            printf("\tIBAN: %s\n",sirConturi[i]->IBAN);
        }
        printf("Va rugam selectati contul din care doriti sa trimiteti banii ('0' daca doriti sa reveniti la meniul principal):");
        scanf("%d",&contAlesSursa);
        fflush(stdin);
        if(contAlesSursa==0){
            printf("Se va reveni la meniul principal...");
            sleep(2);
            meniuPrincipal(sirConturi,lungimeSir);
        }
        if(contAlesSursa<=lungimeSir && contAlesSursa>0){
            contAlesSursa=contAlesSursa-1;
        }
        else{
            printf("Nu ati introdus numarul unui cont valid. Se va reveni la meniul principal...\n");
            sleep(3);
            meniuPrincipal(sirConturi,lungimeSir);
        }
        Cont *contSursa=sirConturi[contAlesSursa];
        printf("Numele si Prenumele destinatarului: ");
        scanf("%s%s",nume,prenume);
        if(countConturi("data.csv",nume,prenume)!=0){
            nrConturiDestinatar=countConturi("data.csv",nume,prenume);
            sirConturiDestinatar=readFromCSV("data.csv",nume,prenume,nrConturiDestinatar);
            for(int i=0;i<nrConturiDestinatar;i++)
            {
                if(strcmp(sirConturiDestinatar[i]->IBAN,contSursa->IBAN)!=0){
                    printf("Contul %d:\n",i+1);
                    printf("\tIBAN: %s\n",sirConturiDestinatar[i]->IBAN);
                }
            }
            printf("Va rugam selectati contul catre care doriti sa trimiteti bani ('0' daca doriti sa reveniti la meniul principal):");
            scanf("%d",&contAlesDest);
            if(contAlesDest==0){
                printf("Se va reveni la meniul principal...");
                sleep(2);
                meniuPrincipal(sirConturi,lungimeSir);
            }
            else if(contAlesDest>nrConturiDestinatar || contAlesDest<0){
                printf("Nu ati introdus numarul unui cont valid. Se va reveni la meniul principal...\n");
                sleep(2);
                meniuPrincipal(sirConturi,lungimeSir);
            }
            else if(strcmp(contSursa->IBAN,sirConturiDestinatar[contAlesDest-1]->IBAN)==0)
            {
                printf("Nu puteti trimite banii spre acelasi cont! Se va reveni la meniul principal...");
                sleep(2);
                meniuPrincipal(sirConturi,lungimeSir);
            }
            if(contAlesDest<=nrConturiDestinatar && contAlesDest>0){
                contAlesDest=contAlesDest-1;
            }
            Cont *contDestinatar=sirConturiDestinatar[contAlesDest];
            printf("Suma: ");
            scanf("%f",&suma);
            if(suma<=0){
                printf("Suma introdusa nu este acceptata. Se va reveni la meniul principal...\n");
                sleep(2);
                meniuPrincipal(sirConturi,lungimeSir);
            }
            if(contSursa->soldDisp>=suma){
                trimiteBani(contSursa,contDestinatar,suma);
                sirConturi=readFromCSV("data.csv",contSursa->numePers,contSursa->pnumePers,lungimeSir);
            }
            else{
                printf("Fonduri insuficiente! Tranzactia a esuat. Se va reveni la meniul principal...\n");
                sleep(2);
                meniuPrincipal(sirConturi,lungimeSir);
            }
            printf("Tranzactia a fost realizata cu succes!\n");
            printf("Ce actiune doriti sa efectuati?\n");
            printf("1)Inapoi la meniul principal\n");
            printf("2)Iesire din aplicatie\n");
            scanf("%d",&alegere);
            switch(alegere){
                case 1:
                    meniuPrincipal(sirConturi,lungimeSir);
                    break;
                case 2:
                    exit(0);
                default:
                    printf("Alegere incorecta\n");
                    sleep(2);
            }
        }
        else{
            printf("Nu s-a gasit niciun cont pentru aceasta persoana. Se va reveni la meniul principal...");
            sleep(2);
            meniuPrincipal(sirConturi,lungimeSir);
        }
    }while(alegere!=2);
    free(sirConturiDestinatar);
}

void editareCont(Cont **sirConturi, int lungimeSir)
{
    int alegere,contAles;
    do{
        system("cls");
        printf("-------------------------------------\n");
        printf("|            Editare cont           |\n");
        printf("-------------------------------------\n");
        for(int i=0;i<lungimeSir;i++){
            printf("Contul %d:\n",i+1);
            printf("\tIBAN: %s\n",sirConturi[i]->IBAN);
            printf("\tDetinator: %s %s\n",sirConturi[i]->numePers, sirConturi[i]->pnumePers);
            printf("\tSold: %.2f %s\n",sirConturi[i]->soldDisp,sirConturi[i]->moneda);
        }
        printf("Va rugam selectati contul pe care doriti sa il editati:");
        scanf("%d",&contAles);
        fflush(stdin);
        if(contAles<=lungimeSir && contAles>0){
            contAles=contAles-1;
        }
        else{
            printf("Nu ati introdus numarul unui cont valid. Se va reveni la meniul principal...\n");
            sleep(3);
            meniuPrincipal(sirConturi,lungimeSir);
        }
        printf("Ce doriti sa editati?\n");
        printf("1) IBAN\n");
        printf("2) Sold\n");
        printf("3) Moneda\n");
        printf("4) Inapoi la meniul principal\n");
        scanf("%d",&alegere);
        switch(alegere){
            case 1:
                editareIBAN(sirConturi[contAles],sirConturi,lungimeSir);
                break;
            case 2:
                editareSold(sirConturi[contAles],sirConturi,lungimeSir);
                break;
            case 3:
                editareMoneda(sirConturi[contAles],sirConturi,lungimeSir);
                break;
            case 4:
                meniuPrincipal(sirConturi,lungimeSir);
            default:
                printf("Alegere incorecta\n");
                sleep(2);
        }
    }while(alegere!=4);
}

void vizualizareCont(Cont **sirConturi, int lungimeSir)
{
    int alegere;
    do{
        system("cls");
        for(int i=0;i<lungimeSir;i++){
            printf("Contul %d:\n",i+1);
            printf("\tIBAN: %s\n",sirConturi[i]->IBAN);
            printf("\tDetinator: %s %s\n",sirConturi[i]->numePers, sirConturi[i]->pnumePers);
            printf("\tSold: %.2f %s\n",sirConturi[i]->soldDisp,sirConturi[i]->moneda);
        }
        if(lungimeSir!=0){
            printf("Ce actiune doriti sa efectuati?\n");
            printf("1)Editare Cont\n");
            printf("2)Stergere Cont\n");
            printf("3)Inapoi la meniul principal\n");
            printf("4)Iesire\n");
            scanf("%d",&alegere);
            switch(alegere){
                case 1:
                    editareCont(sirConturi,lungimeSir);
                    break;
                case 2:
                    stergereCont(sirConturi,lungimeSir);
                    break;
                case 3:
                    meniuPrincipal(sirConturi,lungimeSir);
                    break;
                case 4:
                    exit(0);
                default:
                    printf("Alegere incorecta\n");
                    sleep(2);
            }
        }
        else{
            printf("Nu aveti niciun cont la banca noastra!");
            exit(0);
        }
    }while(alegere!=4);
}
void stergereCont(Cont **sirConturi,int lungimeSir)
{
    char numeDetinator[20],pnumeDetinator[20];
    int alegere,nrConturi=0;
    do{
        system("cls");
        printf("--------------------------------------\n");
        printf("|            Stergere cont           |\n");
        printf("--------------------------------------\n");
        int contAles;
        float sold;
        char iban[12],ibanVer[12],nume[20],pnume[20],moneda[20];
        char header[100],data[255],*p;
        for(int i=0;i<lungimeSir;i++){
            printf("Contul %d:\n",i+1);
            printf("\tIBAN: %s\n",sirConturi[i]->IBAN);
        }
        printf("Va rugam selectati contul pe care doriti sa il stergeti ('0' daca doriti sa reveniti la meniul principal):");
        scanf("%d",&contAles);
        fflush(stdin);
        if(contAles==0){
            printf("Se va reveni la meniul principal...");
            sleep(2);
            meniuPrincipal(sirConturi,lungimeSir);
        }
        if(contAles<=lungimeSir && contAles>0){
            contAles=contAles-1;
        }
        else{
            printf("Nu ati introdus numarul unui cont valid. Se va reveni la meniul principal...\n");
            sleep(3);
            meniuPrincipal(sirConturi,lungimeSir);
        }
        printf("Sunteti sigur ca doriti sa stergeti contul?\n");
        printf("1) Da\n");
        printf("2) Nu\n");
        scanf("%d",&alegere);
        switch(alegere){
            case 1:
                strcpy(iban,sirConturi[contAles]->IBAN);
                strcpy(numeDetinator,sirConturi[contAles]->numePers);
                strcpy(pnumeDetinator,sirConturi[contAles]->pnumePers);
                FILE *oldFile=fopen("data.csv","r");
                FILE *newFile=fopen("temp.csv","w");
                fgets(header,100,oldFile);
                header[strlen(header)-1]='\0';
                fprintf(newFile,"%s",header);
                while(!feof(oldFile))
                {
                    fgets(data,255,oldFile);
                    p=strtok(data,",");
                    strcpy(nume,p);
                    p=strtok(NULL,",");
                    strcpy(pnume,p);
                    p=strtok(NULL,",");
                    strcpy(ibanVer,p);
                    p=strtok(NULL,",");
                    strcpy(moneda,p);
                    p=strtok(NULL,",");
                    sold=atof(p);
                    if(strcmp(iban,ibanVer)!=0){
                        fprintf(newFile,"\n%s,%s,%s,%s,%.2f",nume,pnume,ibanVer,moneda,sold);
                    }
                    else nrConturi++;
                }
                fclose(oldFile);
                fclose(newFile);
                remove("data.csv");
                rename("temp.csv","data.csv");
                printf("Contul dumneavoastra a fost sters cu succes!");
                lungimeSir=lungimeSir-nrConturi;
                sirConturi=readFromCSV("data.csv",numeDetinator,pnumeDetinator,lungimeSir);
                sleep(3);
                meniuPrincipal(sirConturi,lungimeSir);
            case 2:
                meniuPrincipal(sirConturi,lungimeSir);
                break;
            default:
                printf("Alegere incorecta\n");
                sleep(2);
        }
    }while(alegere!=2);
}
void creareContDinMeniu(char *nume, char *prenume, Cont **sirConturi, int lungimeSir)
{
    Cont *cont;
    char monedaControl[4];
    char *nrIban;
    nrIban=(char*)malloc(10*sizeof(char));
    cont=(Cont*)malloc(sizeof(Cont));
    system("cls");
    printf("--------------------------------------\n");
    printf("|            Creare Cont             |\n");
    printf("--------------------------------------\n");
    strcpy(cont->numePers,nume);
    strcpy(cont->pnumePers,prenume);
    printf("Va rugam introduceti moneda (RON,EUR,USD, '0' pentru a reveni la meniul principal): ");
    scanf("%s",monedaControl);
    if(strcmp(monedaControl,"0")==0){
        printf("Se va reveni la meniul principal...\n");
        sleep(2);
        meniuPrincipal(sirConturi,lungimeSir);
    }
    char *monedaControlUpr;
    monedaControlUpr=strToUpr(monedaControl);
    strcpy(monedaControl,monedaControlUpr);
    if(strcmp(monedaControl,"RON")!=0 && strcmp(monedaControl,"EUR")!=0 && strcmp(monedaControl,"USD")!=0){
        while(strcmp(monedaControl,"RON")!=0 && strcmp(monedaControl,"EUR")!=0 && strcmp(monedaControl,"USD")!=0){
            printf("Moneda introdusa nu este acceptata, va rugam introduceti una din optiunile prezentate anterior!\n");
            scanf("%s", monedaControl);
            monedaControlUpr=strToUpr(monedaControl);
            strcpy(monedaControl,monedaControlUpr);
        }
    }
    nrIban=generareIban(monedaControl);
    strcpy(cont->IBAN,nrIban);
    free(nrIban);
    strcpy(cont->moneda,monedaControl);
    cont->soldDisp=0;
    writeToCSV("data.csv",cont);
    lungimeSir+=1;
    sirConturi=readFromCSV("data.csv",nume,prenume,lungimeSir);
    int alegere;
    printf("Contul a fost creat cu succes!\n");
    sleep(2);
    printf("1) Meniu principal\n");
    printf("2) Iesire\n");
    printf("Introduceti numarul actiunii pe care doriti sa o efectuati:");
    scanf("%d",&alegere);
    switch(alegere){
        case 1:
            meniuPrincipal(sirConturi,lungimeSir);
            break;
        case 2:
            exit(0);
    }
    free(cont);
}
void meniuPrincipal(Cont **sirConturi,int lungime)
{
    int alegere;
    if(lungime==0)
    {
        system("cls");
        printf("Nu aveti niciun cont la banca noastra!");
        exit(0);
    }
    do{
        system("cls");
        printf("-------------------------------------\n");
        printf("|         Bine ati revenit!         |\n");
        printf("-------------------------------------\n");
        printf("1) Efectuati o tranzactie\n");
        printf("2) Editare cont\n");
        printf("3) Vizualizare cont\n");
        printf("4) Stergere cont\n");
        printf("5) Creeaza un alt cont\n");
        printf("6) Inchidere\n");
        printf("Introduceti numarul actiunii pe care doriti sa o efectuati: ");
        scanf("%d",&alegere);
        switch(alegere)
        {
            case 1:
                tranzactie(sirConturi,lungime);
                break;
            case 2:
                editareCont(sirConturi,lungime);
                break;
            case 3:
                vizualizareCont(sirConturi,lungime);
                break;
            case 4:
                stergereCont(sirConturi,lungime);
                break;
            case 5:
                creareContDinMeniu(sirConturi[0]->numePers,sirConturi[0]->pnumePers,sirConturi,lungime);
            case 6:
                exit(0);
            default:
                printf("Alegere incorecta!\n");
                sleep(2);
        }
    } while(alegere!=5);
}
void editareIBAN(Cont *cont,Cont **sirConturi,int lungimeSir){
    char iban[12],numeDetinator[20],pnumeDetinator[20];
    char ibanTemp[12],nume[20],pnume[20],moneda[4];
    char header[100],data[255],*p;
    float sold;
    strcpy(iban,generareIban(cont->moneda));
    printf("Noul iban generat: %s",iban);
    strcpy(numeDetinator,cont->numePers);
    strcpy(pnumeDetinator,cont->pnumePers);
    FILE *oldFile=fopen("data.csv","r");
    FILE *newFile=fopen("temp.csv","w");
    fgets(header,100,oldFile);
    header[strlen(header)-1]='\0';
    fprintf(newFile,"%s",header);
    while(!feof(oldFile))
    {
        fgets(data,255,oldFile);
        p=strtok(data,",");
        strcpy(nume,p);
        p=strtok(NULL,",");
        strcpy(pnume,p);
        p=strtok(NULL,",");
        strcpy(ibanTemp,p);
        p=strtok(NULL,",");
        strcpy(moneda,p);
        p=strtok(NULL,",");
        sold=atof(p);
        if(strcmp(cont->IBAN,ibanTemp)==0)
            fprintf(newFile,"\n%s,%s,%s,%s,%.2f",nume,pnume,iban,moneda,sold);
        else
            fprintf(newFile,"\n%s,%s,%s,%s,%.2f",nume,pnume,ibanTemp,moneda,sold);
    }
    fclose(oldFile);
    fclose(newFile);
    remove("data.csv");
    rename("temp.csv","data.csv");
    sirConturi=readFromCSV("data.csv",numeDetinator,pnumeDetinator,lungimeSir);
    sleep(3);
    meniuPrincipal(sirConturi,lungimeSir);
}
void editareSold(Cont *cont,Cont **sirConturi,int lungimeSir){
    float suma;
    char numeDetinator[20],pnumeDetinator[20];
    float soldNou;
    char ibanTemp[12],nume[20],pnume[20],moneda[4];
    char header[100],data[255],*p;
    float sold;
    strcpy(numeDetinator,cont->numePers);
    strcpy(pnumeDetinator,cont->pnumePers);
    printf("Suma pe care doriti sa o adaugati: ");
    scanf("%f",&suma);
    soldNou=cont->soldDisp+suma;
    FILE *oldFile=fopen("data.csv","r");
    FILE *newFile=fopen("temp.csv","w");
    fgets(header,100,oldFile);
    header[strlen(header)-1]='\0';
    fprintf(newFile,"%s",header);
    while(!feof(oldFile))
    {
        fgets(data,255,oldFile);
        p=strtok(data,",");
        strcpy(nume,p);
        p=strtok(NULL,",");
        strcpy(pnume,p);
        p=strtok(NULL,",");
        strcpy(ibanTemp,p);
        p=strtok(NULL,",");
        strcpy(moneda,p);
        p=strtok(NULL,",");
        sold=atof(p);
        if(strcmp(cont->IBAN,ibanTemp)==0)
            fprintf(newFile,"\n%s,%s,%s,%s,%.2f",nume,pnume,ibanTemp,moneda,soldNou);
        else
            fprintf(newFile,"\n%s,%s,%s,%s,%.2f",nume,pnume,ibanTemp,moneda,sold);
    }
    fclose(oldFile);
    fclose(newFile);
    remove("data.csv");
    rename("temp.csv","data.csv");
    sirConturi=readFromCSV("data.csv",numeDetinator,pnumeDetinator,lungimeSir);
    printf("Suma a fost acceptata! Va multumim!\n");
    sleep(3);
    meniuPrincipal(sirConturi,lungimeSir);
}
void schimbareMonedaInFisier(Cont *cont,char *moneda,float soldNou)
{
    char iban[12],numeDetinator[20],pnumeDetinator[20];
    char ibanTemp[12],nume[20],pnume[20],monedaTemp[4];
    char header[100],data[255],*p;
    float sold;
    strcpy(iban,generareIban(moneda));
    strcpy(numeDetinator,cont->numePers);
    strcpy(pnumeDetinator,cont->pnumePers);
    FILE *oldFile=fopen("data.csv","r");
    FILE *newFile=fopen("temp.csv","w");
    fgets(header,100,oldFile);
    header[strlen(header)-1]='\0';
    fprintf(newFile,"%s",header);
    while(!feof(oldFile))
    {
        fgets(data,255,oldFile);
        p=strtok(data,",");
        strcpy(nume,p);
        p=strtok(NULL,",");
        strcpy(pnume,p);
        p=strtok(NULL,",");
        strcpy(ibanTemp,p);
        p=strtok(NULL,",");
        strcpy(monedaTemp,p);
        p=strtok(NULL,",");
        sold=atof(p);
        if(strcmp(cont->IBAN,ibanTemp)==0)
            fprintf(newFile,"\n%s,%s,%s,%s,%.2f",nume,pnume,iban,moneda,soldNou);
        else
            fprintf(newFile,"\n%s,%s,%s,%s,%.2f",nume,pnume,ibanTemp,monedaTemp,sold);
    }
    fclose(oldFile);
    fclose(newFile);
    remove("data.csv");
    rename("temp.csv","data.csv");
}
void editareMoneda(Cont *cont,Cont **sirConturi,int lungimeSir){
    char monedaControl[4];
    printf("Moneda contului dumneavoastra este: %s\n",cont->moneda);
    if(strcmp(cont->moneda,"USD")==0)
        printf("Puteti schimba in RON/EUR\n");
    else if(strcmp(cont->moneda,"EUR")==0)
        printf("Puteti schimba in RON/USD\n");
        else
            printf("Puteti schimba in EUR/USD\n");
    printf("Moneda in care doriti sa schimbati: ");
    scanf("%s",monedaControl);
    char *monedaControlUpr;
    monedaControlUpr=strToUpr(monedaControl);
    strcpy(monedaControl,monedaControlUpr);
    if(strcmp(monedaControl,cont->moneda)==0){
        printf("Contul dumneavoastra este deja in moneda %s, va rugam reincercati...\n",monedaControl);
        while(strcmp(monedaControl,cont->moneda)==0){
            printf("Moneda in care doriti sa schimbati: ");
            scanf("%s", monedaControl);
            monedaControlUpr=strToUpr(monedaControl);
            strcpy(monedaControl,monedaControlUpr);
            if(strcmp(monedaControl,cont->moneda)==0){
            printf("Contul dumneavoastra este deja in moneda %s, va rugam reincercati...\n",monedaControl);
            }
        }
        if(strcmp(monedaControl,"USD")==0){
            cont->soldDisp=conversieSuma(cont->moneda,monedaControl,cont->soldDisp);
            schimbareMonedaInFisier(cont,monedaControl,cont->soldDisp);
            sirConturi=readFromCSV("data.csv",cont->numePers,cont->pnumePers,lungimeSir);
        }
        else if(strcmp(monedaControl,"EUR")==0){
            cont->soldDisp=conversieSuma(cont->moneda,monedaControl,cont->soldDisp);
            schimbareMonedaInFisier(cont,monedaControl,cont->soldDisp);
            sirConturi=readFromCSV("data.csv",cont->numePers,cont->pnumePers,lungimeSir);
        }
            else if(strcmp(monedaControl,"RON")==0){
                cont->soldDisp=conversieSuma(cont->moneda,monedaControl,cont->soldDisp);
                schimbareMonedaInFisier(cont,monedaControl,cont->soldDisp);
                sirConturi=readFromCSV("data.csv",cont->numePers,cont->pnumePers,lungimeSir);
            }
        printf("Schimbarea de moneda s-a efectuat cu succes!\n");
        sleep(2);
    }
    else if(strcmp(monedaControl,"RON")!=0 && strcmp(monedaControl,"EUR")!=0 && strcmp(monedaControl,"USD")!=0){
        while(strcmp(monedaControl,"RON")!=0 && strcmp(monedaControl,"EUR")!=0 && strcmp(monedaControl,"USD")!=0){
            printf("Moneda introdusa nu este acceptata, va rugam introduceti una din optiunile prezentate anterior!\n");
            scanf("%s", monedaControl);
            monedaControlUpr=strToUpr(monedaControl);
            strcpy(monedaControl,monedaControlUpr);
        }
        if(strcmp(monedaControl,cont->moneda)==0){
            printf("Contul dumneavoastra este deja in moneda %s, va rugam reincercati...\n",monedaControl);
            while(strcmp(monedaControl,cont->moneda)==0){
                printf("Moneda in care doriti sa schimbati: ");
                scanf("%s", monedaControl);
                monedaControlUpr=strToUpr(monedaControl);
                strcpy(monedaControl,monedaControlUpr);
                if(strcmp(monedaControl,cont->moneda)==0){
                    printf("Contul dumneavoastra este deja in moneda %s, va rugam reincercati...\n",monedaControl);
                }
            }
        }
        if(strcmp(monedaControl,"USD")==0){
            cont->soldDisp=conversieSuma(cont->moneda,monedaControl,cont->soldDisp);
            schimbareMonedaInFisier(cont,monedaControl,cont->soldDisp);
            sirConturi=readFromCSV("data.csv",cont->numePers,cont->pnumePers,lungimeSir);
        }
        else if(strcmp(monedaControl,"EUR")==0){
            cont->soldDisp=conversieSuma(cont->moneda,monedaControl,cont->soldDisp);
            schimbareMonedaInFisier(cont,monedaControl,cont->soldDisp);
            sirConturi=readFromCSV("data.csv",cont->numePers,cont->pnumePers,lungimeSir);
        }
            else if(strcmp(monedaControl,"RON")==0){
                cont->soldDisp=conversieSuma(cont->moneda,monedaControl,cont->soldDisp);
                schimbareMonedaInFisier(cont,monedaControl,cont->soldDisp);
                sirConturi=readFromCSV("data.csv",cont->numePers,cont->pnumePers,lungimeSir);
            }
        printf("Schimbarea de moneda s-a efectuat cu succes!\n");
        sleep(2);
    }
    else{
        if(strcmp(monedaControl,"USD")==0){
            cont->soldDisp=conversieSuma(cont->moneda,monedaControl,cont->soldDisp);
            schimbareMonedaInFisier(cont,monedaControl,cont->soldDisp);
            sirConturi=readFromCSV("data.csv",cont->numePers,cont->pnumePers,lungimeSir);
        }
        else if(strcmp(monedaControl,"EUR")==0){
            cont->soldDisp=conversieSuma(cont->moneda,monedaControl,cont->soldDisp);
            schimbareMonedaInFisier(cont,monedaControl,cont->soldDisp);
            sirConturi=readFromCSV("data.csv",cont->numePers,cont->pnumePers,lungimeSir);
        }
            else if(strcmp(monedaControl,"RON")==0){
                cont->soldDisp=conversieSuma(cont->moneda,monedaControl,cont->soldDisp);
                schimbareMonedaInFisier(cont,monedaControl,cont->soldDisp);
                sirConturi=readFromCSV("data.csv",cont->numePers,cont->pnumePers,lungimeSir);
            }
        printf("Schimbarea de moneda s-a efectuat cu succes!\n");
        sleep(2);
    }
    meniuPrincipal(sirConturi,lungimeSir);
}
