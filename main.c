#include "fileIO.h"
#include "menuFunc.h"
int main(int argc, char **argv)
{
    if(argc==4){
        if(strcmp(argv[1],"login")==0){
            if(cautareCont("data.csv",argv[2],argv[3])!=0){
                Cont **sirConturi;
                int n=countConturi("data.csv",argv[2],argv[3]);
                sirConturi=readFromCSV("data.csv",argv[2],argv[3],n);
                meniuPrincipal(sirConturi,n);
            }
            else{
                printf("Buna ziua! Se pare ca e prima oara cand accesati aplicatia noastra...\nDoriti sa creati un cont nou?\n1)Da\n2)Nu\n");
                int alegere;
                scanf("%d",&alegere);
                if(alegere==1) {
                        creareCont(argv[2],argv[3]);
                        printf("Contul dumneavoastra a fost creat cu succes!");
                        sleep(2);
                }
                else if(alegere==2) exit(0);
                    else{
                        printf("Alegere incorecta, programul se va inchide!");
                        exit(0);
                    }
            }
        }
        else{
            printf("Nu ati folosit cuvantul 'login', va rugam sa incercati din nou");
            exit(0);
        }
    }
    else{
        printf("Nu ati introdus suficiente argumente, va rugam incercati din nou");
        exit(0);
    }
    return 0;
}
