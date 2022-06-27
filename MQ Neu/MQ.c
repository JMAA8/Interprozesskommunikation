#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

typedef struct msgbuf
{
    int mtype;
    int Zahl;
} msg;

int main(int argc, char const *argv[])
{
    // Summe und Mittelwert
    int Summe = 0;
    int Mittelwert = 0;
    // Variable zum zählen der Schleifen
    int Schleifenanzahl = 1;

    // Conv -> Log
    int mq_key1 = 1; // MQ Key für Cov -> Log
    int rc_msgget1;  // Return Code Conv -> Log

    // Conv -> Stat
    int mq_key2 = 2; // MQ Key für Conv -> Stat
    int rc_msgget2;  // Return Code Conv -> Stat

    // Stat (Summe) -> Report
    int mq_key3 = 3; // MQ Key für Stat (Summe) -> Report
    int rc_msgget3;  // Return Code Stat (Summe) -> Report

    // Stat(Mittelwert) -> Report
    int mq_key4 = 4; // MQ Key für Stat (Mittelwert) -> Report
    int rc_msgget4;  // Return Code Stat (Mittelwert) -> Report

    msg sendebuffer, recievebuffer; // Empfang und Sendepuffer

    rc_msgget1 = msgget(mq_key1, IPC_CREAT);
    rc_msgget2 = msgget(mq_key2, IPC_CREAT);
    rc_msgget3 = msgget(mq_key3, IPC_CREAT);
    rc_msgget4 = msgget(mq_key3, IPC_CREAT);
    
    sendebuffer.mtype = 1;

    int id1 = fork();
    int id2 = fork();

    if (id1 == 0)
    {
        if (id2 == 0)
        {
            // Conv

            while (1)
            {
                // printf("Conv startet \n");
                int zufallszahl = 0;
                time_t t;
                srand(time(&t));
                zufallszahl = rand() % 101;

                // Zahl ausgabe
                // printf("Zahl: %d \n", zufallszahl);

                // Zahl in sendebuffer speichern
                sendebuffer.Zahl = zufallszahl;

                // Conv -> Log
                if (msgsnd(rc_msgget1, &sendebuffer, sizeof(sendebuffer.Zahl), 0) == -1)
                {
                    printf("Das Senden der Zahl an Log hat nicht funktioniert\n");
                }
                else
                {
                    // printf("Geschickte Zahl an Log: %d\n", sendebuffer.Zahl);
                }
                // Con -> Stat
                if (msgsnd(rc_msgget2, &sendebuffer, sizeof(sendebuffer.Zahl), 0) == -1)
                {
                    printf("Das Senden der Zahl an Stat hat nicht funktioniert\n");
                }
                else
                {
                    // printf("Geschickte Zahl an Stat: %d\n", sendebuffer.Zahl);
                }
                sleep(1);
            }
        }
        else
        {
            // Log

            while (1)
            {
                // Warten auf Zahl
                while (msgrcv < 0)
                {
                    wait(NULL);
                }

                // Empfangen der Zahl
                msgrcv(rc_msgget1, &recievebuffer, sizeof(recievebuffer.Zahl), 0, 0);
                // printf("Erhaltene Zahl von Conv an Log: %d \n", recievebuffer.Zahl);

                // Zahl in Variable speichern
                int Zahl = 0;
                Zahl = recievebuffer.Zahl;

                // Zahl in Dokument schreiben
                FILE *ed;
                ed = fopen("ZufallszahlenMessageQueues", "a");
                if (ed == 0)
                {
                    printf("Datei kann nicht geöffnet werden! \n");
                }
                else
                {
                    fprintf(ed, "%d\n", Zahl);
                }
                fclose(ed);
            }
        }
    }
    else
    {
        if (id2 == 0)
        {
            // Stat
            while (1)
            {
                // Warten auf Zahl
                while (msgrcv < 0)
                {
                    wait(NULL);
                }
                // Zahl Empfangen
                msgrcv(rc_msgget2, &recievebuffer, sizeof(recievebuffer.Zahl), 0, 0);
                // printf("Erhaltene Zahl von Conv an Stat: %d \n", recievebuffer.Zahl);

                // Zahl in Variable speichern
                int StatZahl = 0;
                StatZahl = recievebuffer.Zahl;

                // Summe ausrechen
                Summe += StatZahl;
                // Mittelwert ausrechnen
                Mittelwert = Summe / Schleifenanzahl;

                Schleifenanzahl++;

                // Summe -> Report schicken
                sendebuffer.Zahl = Summe;
                if (msgsnd(rc_msgget3, &sendebuffer, sizeof(sendebuffer.Zahl), 0) == -1)
                {
                    printf("Das Senden der Summe an Report hat nicht funktioniert\n");
                }
                else
                {
                    // printf("Geschickte Zahl (Summe) an Report: %d\n", sendebuffer.Zahl);
                }

                // Mittelwert -> Report schicken
                sendebuffer.Zahl = Mittelwert;
                if (msgsnd(rc_msgget4, &sendebuffer, sizeof(sendebuffer.Zahl), 0) == -1)
                {
                    printf("Das Senden der Zahl hat nicht funktioniert\n");
                }
                else
                {
                    // printf("Geschickte Zahl (Mittelwert) an Report: %d\n", sendebuffer.Zahl);
                }
            }
        }
        else
        {
            // Report
            while (1)
            {

                // Warten auf Summe
                while (msgrcv < 0)
                {
                    wait(NULL);
                }
                // Summe empfangen
                msgrcv(rc_msgget3, &recievebuffer, sizeof(recievebuffer.Zahl), 0, 0);

                // printf("Erhaltene Summe von Stat: %d \n", recievebuffer.Zahl);
                int SummeEmpfang;
                SummeEmpfang = recievebuffer.Zahl;

                // Mittwert empfangen
                int MittelwertEmpfang;
                msgrcv(rc_msgget4, &recievebuffer, sizeof(recievebuffer.Zahl), 0, 0);
                // printf("Erhaltener Mittelwert von Stat: %d \n", recievebuffer.Zahl);

                MittelwertEmpfang = recievebuffer.Zahl;

                // Summe und Mittelwert ausgaben
                printf("Summe: %d \n", SummeEmpfang);
                printf("Mittelwert: %d \n", MittelwertEmpfang);
                printf("\n");
            }
        }
    }

    return 0;
}