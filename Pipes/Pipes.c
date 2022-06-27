#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

int main(int argc, char *argv[])
{
    // Conv -> Log
    int pip1[2];
    if (pipe(pip1) == -1)
    {
        printf("ERROR with Pipe 1\n");
    }

    // Conv -> Stat
    int pip2[2];
    if (pipe(pip2) == -1)
    {
        printf("ERROR with Pipe 2\n");
    }

    // Stat -> Report
    // Summe
    int pipS[2];
    if (pipe(pipS) == -1)
    {
        printf("ERROR with Pipe S\n");
    }
    // Mittelwert
    int pipM[2];
    if (pipe(pipM) == -1)
    {
        printf("ERROR with Pipe M\n");
    }

    // Speicherort für Stat
    /* int *speicher = malloc(sizeof(int));
                 if (speicher == NULL){
                     return 1;
                 }*/

    // Variable zum zählen der Schleifen und reservieren des Speicherplatzes
    int Schleifenzahl = 1;

    // Summe Mittelwert
    int Summe2 = 0;
    int Mittelwert2 = 0;

    // Erstellen von vier Prozessen
    int id1 = fork();
    int id2 = fork();

    // printf("%d\n", id1);
    // printf("%d\n", id2);

    // Zugriff auf die einzelnen Prozesse
    if (id1 == 0)
    {
        if (id2 == 0)
        {
            // Conv
            close(pipS[0]);
            close(pipM[0]);
            close(pipS[1]);
            close(pipM[1]);
            close(pip1[0]);
            close(pip2[0]);

            while (1)
            {

                time_t t;
                srand(time(&t));
                int zufallszahl = rand() % 101;

                write(pip1[1], &zufallszahl, sizeof(int));

                write(pip2[1], &zufallszahl, sizeof(int));

                // printf("Zahl: %d\n", zufallszahl);

                sleep(1);
                // Schleifenzahl++;
            }
        }
        else
        {
            // Log
            close(pipS[0]);
            close(pipM[0]);
            close(pipS[1]);
            close(pipM[1]);
            close(pip2[0]);
            close(pip2[1]);

            close(pip1[1]);
            while (1)
            {

                int y;
                read(pip1[0], &y, sizeof(int));
                // printf("Schreibe File %d\n", y);

                FILE *ed;
                ed = fopen("ZufallszahlenPipes", "a");
                if (ed == 0)
                {
                    printf("Datei kann nicht geöffnet werden!");
                }
                else
                {
                    fprintf(ed, "%d\n", y);
                }

                fclose(ed);
                // printf("Es hat funktioniert: %d\n", y);

                // Schleifenzahl++;
            }
        }
    }
    else
    {
        if (id2 == 0)
        {
            // Stat
            close(pipS[0]);
            close(pipM[0]);
            close(pip1[0]);
            close(pip1[1]);
            close(pip2[1]);
            while (1)
            {

                int zVonConv;
                read(pip2[0], &zVonConv, sizeof(int));

                Summe2 += zVonConv;
                Mittelwert2 = Summe2 / Schleifenzahl;

                // printf("Summe aus Weg 2: %d\n", Summe2);
                // printf("Mittelwert aus Weg 2: %d\n", Mittelwert2);

                /* if(Schleifenzahl == 1){

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
                 Mittelwert = Summe / Schleifenzahl; */

                write(pipS[1], &Summe2, sizeof(int));
                write(pipM[1], &Mittelwert2, sizeof(int));

                // printf("Kontrolle: %d\n", Summe2);

                Schleifenzahl++;
            }
        }
        else
        {
            // Report
            close(pip2[0]);
            close(pip2[1]);
            close(pip1[0]);
            close(pip1[1]);
            close(pipS[1]);
            close(pipM[1]);
            while (1)
            {

                int SummeAusS;
                int MittelwertAusS;

                read(pipS[0], &SummeAusS, sizeof(int));

                read(pipM[0], &MittelwertAusS, sizeof(int));

                printf("Akutelle Summe: %d\n", SummeAusS);
                printf("Aktueller Mittelwert: %d\n", MittelwertAusS);

                // Schleifenzahl++;
            }
        }
    }

    // free(speicher);
    return 0;
}