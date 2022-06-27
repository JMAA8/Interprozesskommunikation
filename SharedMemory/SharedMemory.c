#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#include <sys/sem.h>
#include <semaphore.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

#include <signal.h>

#define MAXMEMSIZE 20
#define SEM_Producer_FNAME
#define SEM_Consumer_FNAME

#define errExit(msg)        \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

static void sigintHandler(int sig)
{

    // Shared Memory Löschen

    int ConvLog_shmget = shmget(12345893, MAXMEMSIZE, IPC_CREAT | 0600);

    shmctl(ConvLog_shmget, IPC_RMID, NULL);

    int ConStat_shmget = shmget(678902435, MAXMEMSIZE, IPC_CREAT | 0600);

    shmctl(ConStat_shmget, IPC_RMID, NULL);

    int StatS_shmget = shmget(1246863649, MAXMEMSIZE, IPC_CREAT | 0600);
    shmctl(StatS_shmget, IPC_RMID, NULL);

    int StatM_shmget = shmget(13579843, MAXMEMSIZE, IPC_CREAT | 0600);
    shmctl(StatM_shmget, IPC_RMID, NULL);

    // Semaphoren löschen
    int sm1 = semget(123450234, 1, IPC_STAT);
    semctl(sm1, 1, IPC_RMID);

    int sm2 = semget(432134611, 1, IPC_STAT);
    semctl(sm2, 1, IPC_RMID);

    int sm3 = semget(123459355, 1, IPC_STAT);
    semctl(sm3, 1, IPC_RMID);

    int sm4 = semget(432109984, 1, IPC_STAT);
    semctl(sm4, 1, IPC_RMID);

    int sm5 = semget(345232100, 1, IPC_STAT);
    semctl(sm5, 1, IPC_RMID);

    int sm6 = semget(238123882, 1, IPC_STAT);
    semctl(sm6, 1, IPC_RMID);

    write(STDERR_FILENO, "Prozess beendet\n", 20);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{

    if (signal(SIGINT, sigintHandler) == SIG_ERR)
        errExit("signal SIGINT");

    // Shared Memory

    // Conv - Log
    int ConvLogKey = 12345893;
    int ConvLog_shmget;
    int ConvLog_shmdt;
    int ConvLog_sprintf;
    int ConvLog_printf;
    int ConvLog_shmctl;
    char *ConvLogpointer;

    // Gemeinsames Speichersegment erzeugen
    // IPC_CREAT = Segment erzeugen, wenn es noch nicht existiert
    // 0600 = Zugriffsrechte auf das neue Segment
    ConvLog_shmget = shmget(ConvLogKey,
                            MAXMEMSIZE,
                            IPC_CREAT | 0600);
    if (ConvLog_shmget < 0)
    {
        printf("Das Segment konnte nicht erstellt werden.\n");
        perror("shmget");
        exit(1);
    }
    else
    {
        // printf("Das Segment wurde erstellt.\n");
    }

    // Gemeinsames Speichersegment anhängen
    ConvLogpointer = shmat(ConvLog_shmget, 0, 0);
    if (ConvLogpointer == (char *)-1)
    {
        printf("Das Segment konnte nicht angehängt werden.\n");
        perror("shmat");
        exit(1);
    }
    else
    {
        // printf("Das Segment wurde angehängt.\n");
    }

    // Conv - Stat
    int ConvStatKey = 678902435;
    int ConvStat_shmget;
    int ConvStat_shmdt;
    int ConvStat_sprintf;
    int ConvStat_printf;
    int ConvStat_shmctl;
    char *ConvStatpointer;

    // Gemeinsames Speichersegment erzeugen
    // IPC_CREAT = Segment erzeugen, wenn es noch nicht existiert
    // 0600 = Zugriffsrechte auf das neue Segment
    ConvStat_shmget = shmget(ConvStatKey,
                             MAXMEMSIZE,
                             IPC_CREAT | 0600);
    if (ConvStat_shmget < 0)
    {
        printf("Das Segment konnte nicht erstellt werden.\n");
        perror("shmget");
        exit(1);
    }
    else
    {
        // printf("Das Segment wurde erstellt.\n");
    }

    // Gemeinsames Speichersegment anhängen
    ConvStatpointer = shmat(ConvStat_shmget, 0, 0);
    if (ConvStatpointer == (char *)-1)
    {
        printf("Das Segment konnte nicht angehängt werden.\n");
        perror("shmat");
        exit(1);
    }
    else
    {
        // printf("Das Segment wurde angehängt.\n");
    }

    // Stat - Report: Summe
    int StatReportSKey = 1246863649;
    int StatReportS_shmget;
    int StatReportS_shmdt;
    int StatReportS_sprintf;
    int StatReportS_printf;
    int StatReportS_shmctl;
    char *StatReportSpointer;

    // Gemeinsames Speichersegment erzeugen
    // IPC_CREAT = Segment erzeugen, wenn es noch nicht existiert
    // 0600 = Zugriffsrechte auf das neue Segment
    StatReportS_shmget = shmget(StatReportSKey,
                                MAXMEMSIZE,
                                IPC_CREAT | 0600);
    if (StatReportS_shmget < 0)
    {
        printf("Das Segment konnte nicht erstellt werden.\n");
        perror("shmget");
        exit(1);
    }
    else
    {
        // printf("Das Segment wurde erstellt.\n");
    }

    // Gemeinsames Speichersegment anhängen
    StatReportSpointer = shmat(StatReportS_shmget, 0, 0);
    if (StatReportSpointer == (char *)-1)
    {
        printf("Das Segment konnte nicht angehängt werden.\n");
        perror("shmat");
        exit(1);
    }
    else
    {
        // printf("Das Segment wurde angehängt.\n");
    }

    // Stat - Report: Mittelwert
    int StatReportMKey = 13579843;
    int StatReportM_shmget;
    int StatReportM_shmdt;
    int StatReportM_sprintf;
    int StatReportM_printf;
    int StatReportM_shmctl;
    char *StatReportMpointer;

    // Gemeinsames Speichersegment erzeugen
    // IPC_CREAT = Segment erzeugen, wenn es noch nicht existiert
    // 0600 = Zugriffsrechte auf das neue Segment
    StatReportM_shmget = shmget(StatReportMKey,
                                MAXMEMSIZE,
                                IPC_CREAT | 0600);
    if (StatReportM_shmget < 0)
    {
        printf("Das Segment konnte nicht erstellt werden.\n");
        perror("shmget");
        exit(1);
    }
    else
    {
        // printf("Das Segment wurde erstellt.\n");
    }

    // Gemeinsames Speichersegment anhängen
    StatReportMpointer = shmat(StatReportM_shmget, 0, 0);
    if (StatReportMpointer == (char *)-1)
    {
        printf("Das Segment konnte nicht angehängt werden.\n");
        perror("shmat");
        exit(1);
    }
    else
    {
        // printf("Das Segment wurde angehängt.\n");
    }

    // Erstellen von Semaphorpaaren

    // Conv & Log

    int ConvLog_key1 = 123450234;
    int ConvLog_key2 = 432134611;
    int ConvLog_semget1, ConvLog_semget2, ConvLog_semctl;
    int output;

    ConvLog_semctl = semctl(ConvLog_semget1, 0, IPC_RMID, 0);
    ConvLog_semctl = semctl(ConvLog_semget2, 0, IPC_RMID, 0);

    setbuf(stdout, NULL); // Das Puffern Standardausgabe (stdout) unterbinden 15
    // Neue Semaphorgruppe 12345 mit einer Semaphore erstellen
    // IPC_CREAT = Semaphore erzeugen, wenn Sie noch nicht existiert

    ConvLog_semget1 = semget(ConvLog_key1, 1, IPC_CREAT | 0600);
    if (ConvLog_semget1 < 0)
    {
        printf("Die Semaphorgruppe %i konnte nicht erstellt werden.\n", ConvLog_key1);
        perror("semget");
        exit(1);
    }

    // Neue Semaphorgruppe 54321 mit einer Semaphore erstellen
    ConvLog_semget2 = semget(ConvLog_key2, 1, IPC_CREAT | 0600);
    if (ConvLog_semget2 < 0)
    {
        printf("Die Semaphorgruppe %i konnte nicht erstellt werden.\n", ConvLog_key2);
        perror("semget");
        exit(1);
    }

    // P-Operation definieren. Wert der Semaphore um eins dekrementieren
    struct sembuf p_operation = {0, -1, 0};

    // V-Operation definieren. Wert der Semaphore um eins inkrementieren
    struct sembuf v_operation = {0, 1, 0};

    // Erste Semaphore der Semaphorgruppe 12345 initial auf Wert 1 setzen
    ConvLog_semctl = semctl(ConvLog_semget1, 0, SETVAL, 1);

    // Erste Semaphore der Semaphorgruppe 54321 initial auf Wert 0 setzen
    ConvLog_semctl = semctl(ConvLog_semget2, 0, SETVAL, 0);

    // // Initialen Wert der ersten Semaphore der Semaphorgruppe 12345 zur Kontrolle ausgeben
    // output = semctl(ConvLog_semget1, 0, GETVAL, 0);
    // printf("Wert der Semaphore mit ID %i und Key %i: %i\n", ConvLog_semget1, ConvLog_key1, output);

    // // Initialen Wert der ersten Semaphore der Semaphorgruppe 54321 zur Kontrolle ausgeben
    // output = semctl(ConvLog_semget2, 0, GETVAL, 0);
    // printf("Wert der Semaphore mit ID %i und Key %i: %i\n", ConvLog_semget2, ConvLog_key2, output);

    // Conv & Stat

    int ConvStat_key1 = 123459355;
    int ConvStat_key2 = 432109984;
    int ConvStat_semget1, ConvStat_semget2, ConvStat_semctl;
    int ConvStatoutput;

    ConvStat_semctl = semctl(ConvStat_semget1, 0, IPC_RMID, 0);
    ConvStat_semctl = semctl(ConvStat_semget2, 0, IPC_RMID, 0);

    setbuf(stdout, NULL); // Das Puffern Standardausgabe (stdout) unterbinden 15
    // Neue Semaphorgruppe 12345 mit einer Semaphore erstellen
    // IPC_CREAT = Semaphore erzeugen, wenn Sie noch nicht existiert

    ConvStat_semget1 = semget(ConvStat_key1, 1, IPC_CREAT | 0600);
    if (ConvStat_semget1 < 0)
    {
        printf("Die Semaphorgruppe %i konnte nicht erstellt werden.\n", ConvStat_key1);
        perror("semget");
        exit(1);
    }

    // Neue Semaphorgruppe 54321 mit einer Semaphore erstellen
    ConvStat_semget2 = semget(ConvStat_key2, 1, IPC_CREAT | 0600);
    if (ConvStat_semget2 < 0)
    {
        printf("Die Semaphorgruppe %i konnte nicht erstellt werden.\n", ConvStat_key2);
        perror("semget");
        exit(1);
    }

    // Erste Semaphore der Semaphorgruppe 123459 initial auf Wert 1 setzen
    ConvStat_semctl = semctl(ConvStat_semget1, 0, SETVAL, 1);

    // Erste Semaphore der Semaphorgruppe 543210 initial auf Wert 0 setzen
    ConvStat_semctl = semctl(ConvStat_semget2, 0, SETVAL, 0);

    // // Initialen Wert der ersten Semaphore der Semaphorgruppe 12345 zur Kontrolle ausgeben
    // ConvStatoutput = semctl(ConvStat_semget1, 0, GETVAL, 0);
    // printf("Wert der Semaphore mit ID %i und Key %i: %i\n", ConvStat_semget1, ConvStat_key1, ConvStatoutput);

    // // Initialen Wert der ersten Semaphore der Semaphorgruppe 54321 zur Kontrolle ausgeben
    // ConvStatoutput = semctl(ConvStat_semget2, 0, GETVAL, 0);
    // printf("Wert der Semaphore mit ID %i und Key %i: %i\n", ConvStat_semget2, ConvStat_key2, ConvStatoutput);

    // Stat & Report

    int StatReport_key1 = 345232100;
    int StatReport_key2 = 238123882;
    int StatReport_semget1, StatReport_semget2, StatReport_semctl;
    int StatReportoutput;

    StatReport_semctl = semctl(StatReport_semget1, 0, IPC_RMID, 0);
    StatReport_semctl = semctl(StatReport_semget2, 0, IPC_RMID, 0);

    setbuf(stdout, NULL); // Das Puffern Standardausgabe (stdout) unterbinden 15
    // Neue Semaphorgruppe 12345 mit einer Semaphore erstellen
    // IPC_CREAT = Semaphore erzeugen, wenn Sie noch nicht existiert

    StatReport_semget1 = semget(StatReport_key1, 1, IPC_CREAT | 0600);
    if (StatReport_semget1 < 0)
    {
        printf("Die Semaphorgruppe %i konnte nicht erstellt werden.\n", StatReport_key1);
        perror("semget");
        exit(1);
    }

    // Neue Semaphorgruppe 54321 mit einer Semaphore erstellen
    StatReport_semget2 = semget(StatReport_key2, 1, IPC_CREAT | 0600);
    if (StatReport_semget2 < 0)
    {
        printf("Die Semaphorgruppe %i konnte nicht erstellt werden.\n", StatReport_key2);
        perror("semget");
        exit(1);
    }

    // Erste Semaphore der Semaphorgruppe 12345 initial auf Wert 1 setzen
    StatReport_semctl = semctl(StatReport_semget1, 0, SETVAL, 1);

    // Erste Semaphore der Semaphorgruppe 54321 initial auf Wert 0 setzen
    StatReport_semctl = semctl(StatReport_semget2, 0, SETVAL, 0);

    // // Initialen Wert der ersten Semaphore der Semaphorgruppe 12345 zur Kontrolle ausgeben
    // StatReportoutput = semctl(StatReport_semget1, 0, GETVAL, 0);
    // printf("Wert der Semaphore mit ID %i und Key %i: %i\n", StatReport_semget1, StatReport_key1, StatReportoutput);

    // // Initialen Wert der ersten Semaphore der Semaphorgruppe 54321 zur Kontrolle ausgeben
    // StatReportoutput = semctl(StatReport_semget2, 0, GETVAL, 0);
    // printf("Wert der Semaphore mit ID %i und Key %i: %i\n", StatReport_semget2, StatReport_key2, StatReportoutput);

    // Variable zum zählen der Schleifen
    int Schleifenzahl = 1;

    // Summe Mittelwert
    int Summe2 = 0;
    int Mittelwert2 = 0;

    // Erstellen von vier Prozessen
    int id1 = fork();
    int id2 = fork();

    // printf("Fork1: %d\n", id1);
    // printf("Fork2: %d\n", id2);

    // Zugriff auf die einzelnen Prozesse
    if (id1 == 0)
    {

        if (id2 == 0)
        {

            // Conv

            while (1)
            {

                semop(ConvLog_semget1, &p_operation, 1);

                semop(ConvStat_semget1, &p_operation, 1);

                time_t t;
                srand(time(&t));
                int zufallszahl = rand() % 101;

                // printf("Zufallszahl aus Conv: %d\n", zufallszahl);

                ConvLog_sprintf = sprintf(ConvLogpointer, "%d", zufallszahl);
                if (ConvLog_sprintf < 0)
                {
                    printf("Der Schreibzugriff ist fehlgeschlagen.\n");
                    exit(1);
                }
                else
                {
                    // printf("%i Zeichen wurden in Log geschrieben.\n", ConvLog_sprintf);
                }

                ConvStat_sprintf = sprintf(ConvStatpointer, "%d", zufallszahl);
                if (ConvStat_sprintf < 0)
                {
                    printf("Der Schreibzugriff ist fehlgeschlagen.\n");
                    exit(1);
                }
                else
                {
                    // printf("%i Zeichen wurden Stat geschrieben.\n", ConvStat_sprintf);

                    sleep(1);

                    semop(ConvLog_semget2, &v_operation, 1);
                    semop(ConvStat_semget2, &v_operation, 1);
                }
            }
        }
        else
        {
            // Log

            while (1)
            {

                semop(ConvLog_semget2, &p_operation, 1);

                FILE *ed;
                ed = fopen("ZufallszahlenSharedMemory", "a");
                if (ed == 0)
                {
                    printf("Datei kann nicht geöffnet werden!");
                }
                else
                {
                    fprintf(ed, "%s\n", ConvLogpointer);
                    // printf("Zahl wurde in File geschrieben\n");
                }
                fclose(ed);

                semop(ConvLog_semget1, &v_operation, 1);
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

                semop(ConvStat_semget2, &p_operation, 1);
                semop(StatReport_semget1, &p_operation, 1);

                int zahl = atoi(ConvStatpointer);

                // printf("Zufallszahl aus Stat: %d\n", zahl);

                Summe2 += zahl;

                // printf("Summe aus Stat: %d\n", Summe2);

                StatReportS_sprintf = sprintf(StatReportSpointer, "%d", Summe2);
                if (StatReportS_sprintf < 0)
                {
                    printf("Der Schreibzugriff ist fehlgeschlagen.\n");
                    exit(1);
                }
                else
                {
                    // printf("%i Zeichen wurden Summe Report geschrieben.\n", StatReportS_sprintf);
                }

                Mittelwert2 = Summe2 / Schleifenzahl;

                // printf("Mittelwert aus Stat: %d\n", Mittelwert2);

                StatReportM_sprintf = sprintf(StatReportMpointer, "%d", Mittelwert2);
                if (StatReportM_sprintf < 0)
                {
                    printf("Der Schreibzugriff ist fehlgeschlagen.\n");
                    exit(1);
                }
                else
                {
                    // printf("%i Zeichen wurden Mittelwert Report geschrieben.\n", StatReportM_sprintf);
                }

                semop(ConvStat_semget1, &v_operation, 1);
                semop(StatReport_semget2, &v_operation, 1);

                Schleifenzahl++;
            }
        }
        else
        {
            // Report

            while (1)
            {

                semop(StatReport_semget2, &p_operation, 1);

                if (printf("Summe: %s\n", StatReportSpointer) < 0)
                {
                    printf("Der Lesezugriff ist fehlgeschlagen.\n");
                    exit(1);
                }

                if (printf("Mittelwert: %s\n", StatReportMpointer) < 0)
                {
                    printf("Der Lesezugriff ist fehlgeschlagen.\n");
                    exit(1);
                }

                semop(StatReport_semget1, &v_operation, 1);
            }
        }
    }

    return 0;
}