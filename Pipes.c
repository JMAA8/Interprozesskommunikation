#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

int main(int argc, char* argv[]) {
    //Conv -> Log
    int pip1[2];
    if (pipe(pip1) == -1){
        printf("ERROR with Pipe 1\n");
    }

    //Conv -> Stat
    int pip2[2];
    if (pipe(pip2) == -1){
        printf("ERROR with Pipe 2\n");
    }

    //Stat -> Report
    //Summe
    int pipS[2];
    if (pipe(pipS) == -1){
        printf("ERROR with Pipe S\n");
    }
    //Mittelwert
    int pipM[2];
    if (pipe(pipM) == -1){
        printf("ERROR with Pipe M\n");
    }

    //Report -> Conv.
    int pipR[2];
    if (pipe(pipR) == -1){
        printf("ERROR with Pipe S\n");
    }
    
    //Boolscher Wert, Programm weiter fortgeführt werden soll
    int weiter = 3;

    //Speicherort für Stat (Ich denke, wir dürfen nicht aus File auslesen wegen den Synchronisationsbedingungen)
    int *speicher = malloc(sizeof(int));
                if (speicher == NULL){
                    return 1;
                }

    //Variable zum zählen der Schleifen und reservieren des Speicherplatzes
    int Schleifenzahl = 1;

    //Für Conv
    int anzahlSchleife = 0;

    //Erstellen von vier Prozessen
    int id1 = fork();
    int id2 = fork();

    
    while (1){
    printf("erste\n");
    //Zugriff auf die einzelnen Prozesse
    if (id1 == 0){
        if (id2 == 0){
            //Conv
            //close(pipR[1]);
            
            int warteR;
           
            while (anzahlSchleife < 1){
            printf("Funktioniert\n");
              
            //close(pipR[0]);
               close(pip1[0]);
            close(pip2[0]);

            time_t t;
            srand(time (&t));
            int zufallszahl = rand();
        
            write(pip1[1], &zufallszahl, sizeof(int));
            close(pip1[1]);

            write(pip2[1], &zufallszahl, sizeof(int));
            close(pip2[1]);

            printf("Zahl: %d\n", zufallszahl);

            anzahlSchleife++;
            }
          // }

           
            
        }else{
            //Log
            
            wait(NULL);
            close(pip1[1]);
            int y;
            read(pip1[0], &y, sizeof(int));
            close(pip1[0]);

            FILE *ed;
            ed = fopen("Zufallszahlen", "a");
            if(ed == 0){
                printf("Datei kann nicht geöffnet werden!");
            }else{
                fprintf(ed, "%d\n",y);
            }


            printf("Es hat funktioniert: %d\n", y);
        }
        
    }else{
        if (id2 == 0){
            //Stat

            wait(NULL);

            close(pip2[1]);
            int zVonConv;
            read(pip2[0], &zVonConv, sizeof(int));
            close(pip2[0]);
            if(Schleifenzahl == 1){
                
               *speicher = zVonConv; 
            }else{
                speicher = realloc(speicher, sizeof(int)*Schleifenzahl);
                speicher[Schleifenzahl-1]=zVonConv;

            }
            int Summe;
            int Mittelwert;
            for (int x = 0; x<sizeof(speicher); x++){
                Summe += speicher[x];
            }
            Mittelwert = Summe / Schleifenzahl;

            int Summe2 = 0;
            int Mittelwert2 = 0;

            Summe2 += zVonConv;
            Mittelwert2 = Summe2 / Schleifenzahl;

            printf("Summe aus Weg 2: %d\n", Summe2);
            printf("Mittelwert aus Weg 2: %d\n", Mittelwert2);

            close(pipS[0]);
            close(pipM[0]);

            write(pipS[1], &Summe, sizeof(int));
            close(pipS[1]);

            write(pipM[1], &Mittelwert, sizeof(int));
            close(pipM[1]);
           
        }else{
            //Report
            while(wait(NULL) != -1 || errno != ECHILD){
                

        }

        close(pipS[1]);
        close(pipM[1]);

        int SummeAusS;
        int MittelwertAusS;

        read(pipS[0], &SummeAusS, sizeof(int));
            close(pipS[0]);

        read(pipM[0], &MittelwertAusS, sizeof(int));
            close(pipM[0]);

        printf("Akutelle Summe: %d\n", SummeAusS);
        printf("Aktueller Mittelwert: %d\n", MittelwertAusS);

        printf("Programm weiter ausführen?");
                scanf("%d", &weiter);

                
    }
    
    }
    Schleifenzahl++;
    }
   free(speicher); 
return 0;
}