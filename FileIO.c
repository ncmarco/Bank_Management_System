#include "fileIO.h"
#include "menuFunc.h"

int countConturi(const char *filePath, const char *argPrimit1, const char *argPrimit2){
    int nrConturi=0;
    char numePers[20],pnumePers[20];
    FILE* file=openFile(filePath, "r");
    char *p;
    char header[100];
	char data[255];
	fgets(header,100,file);
    while (!feof(file)){
        fgets(data, 255, file);
		p=strtok(data,",");
		strcpy(numePers,p);
		p=strtok(NULL,",");
		strcpy(pnumePers,p);
        if(strcmp(numePers,argPrimit1)==0 && strcmp(pnumePers,argPrimit2)==0)
            nrConturi++;
    }
    fclose(file);
    return nrConturi;
}

Cont **readFromCSV(const char *filePath, const char *argPrimit1, const char *argPrimit2, int nrConturi){
    Cont *cont;
    Cont **sirConturi;
    cont=(Cont*)malloc(sizeof(Cont));
    sirConturi=(Cont**)malloc(nrConturi*sizeof(Cont*));
    for(int j=0;j<nrConturi;j++)
        sirConturi[j]=(Cont*)malloc(sizeof(Cont));
    FILE* file=openFile(filePath, "r");
    int i=0;
    if(file==NULL)
		return;
    char header[100];
	char data[255];
	char *p;
	fgets(header,100,file);
	while (!feof(file))
	{
		fgets(data, 255, file);
		p=strtok(data,",");
		strcpy(cont->numePers,p);
		p=strtok(NULL,",");
		strcpy(cont->pnumePers,p);
		if(strcmp(cont->numePers,argPrimit1)==0 && strcmp(cont->pnumePers,argPrimit2)==0)
        {
            p=strtok(NULL,",");
            strcpy(cont->IBAN,p);
            p=strtok(NULL,",");
            strcpy(cont->moneda,p);
            p=strtok(NULL,",");
            cont->soldDisp=atof(p);
            strcpy(sirConturi[i]->numePers,cont->numePers);
            strcpy(sirConturi[i]->pnumePers,cont->pnumePers);
            strcpy(sirConturi[i]->IBAN,cont->IBAN);
            strcpy(sirConturi[i]->moneda,cont->moneda);
            sirConturi[i]->soldDisp=cont->soldDisp;
            i++;
        }
	}
	fclose(file);
	return sirConturi;
}
void writeToCSV(const char *filePath, Cont *cont){
	FILE* file=openFile(filePath,"a");
	if (file==NULL)
		return;
	fprintf(file,"\n%s,%s,%s,%s,%.2f",cont->numePers,cont->pnumePers,cont->IBAN,cont->moneda,cont->soldDisp);
	fclose(file);
}
FILE *openFile(const char* filePath, const char* mode){
    if (filePath==NULL || strlen(filePath)==0 ||
		mode==NULL || strlen(mode)==0)
		return NULL;

	FILE* file=NULL;

	file=fopen(filePath,mode);
	return file;
}


