#pragma once
#ifndef MENUFUNC_H_INCLUDED
#define MENUFUNC_H_INCLUDED

typedef struct Cont{
    char numePers[20];
    char pnumePers[20];
    char IBAN[12];
    char moneda[4];
    float soldDisp;
}Cont;

float conversieSuma(const char *moneda1, const char *moneda2, float suma);
char *generareIban(char *moneda);
int verificaIban(const char *filePath, const char *ibanToVerify);
char *strToLow(char *str);
char *strToUpr(char *str);
int cautareCont(const char *filePath, const char *argPrimit1, const char *argPrimit2);
void creareCont(const char *nume, const char *prenume);
void trimiteBani(Cont *contSursa, Cont *contDestinatar, float suma);
void creareContDinMeniu(char *nume, char *prenume, Cont **sirConturi, int lungimeSir);
void tranzactie(Cont **sirConturi,int lungimeSir);
void editareCont(Cont **sirConturi, int lungimeSir);
void vizualizareCont(Cont **sirConturi,int lungime);
void stergereCont(Cont **sirConturi,int lungimeSir);
void meniuPrincipal(Cont **sirConturi, int lungime);
void editareIBAN(Cont *cont,Cont **sirConturi,int lungimeSir);
void editareSold(Cont *cont,Cont **sirConturi,int lungimeSir);
void schimbareMonedaInFisier(Cont *cont,char *moneda, float soldNou);
void editareMoneda(Cont *cont,Cont **sirConturi,int lungimeSir);

#endif // MENUFUNC_H_INCLUDED
