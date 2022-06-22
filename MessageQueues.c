#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <time.h>

typedef struct msgbuf
{
  long mtype;
  int Zahl;
} msg;

int main(int argc, char const *argv[])
{

  // Return Codes für MQ
  int mq_key = 1;                 // MQ Key
  int rc_msgget;                  // Return Code von msgget
  int rc_msgctl;                  // Return Code von msgctl
  int rc_msgrcv;                  // Return Code von msgrcv
  msg sendebuffer, recievebuffer; // Empfang und Sendepuffer

  // Message Queue erstellen
  rc_msgget = msgget(mq_key, IPC_CREAT | 0600);

  // Prüffen ob MQ erstellt wurde
  if (rc_msgget < 0)
  {
    printf("Die Message Queue wurde nicht erstellt. \n");
    perror("msgget");
  }
  else
  {
    printf("Die Message Queue %i mit der ID %i wurde erstellt. \n", mq_key, rc_msgget);
  }

  // Conv
  // Zufallszahl erstellen

  time_t t;
  srand(time(&t));
  int zufallszahl;
  zufallszahl = rand();

  // Zahl ausgabe
  printf("Zahl: %d \n", zufallszahl);

  // Zahl in MQ senden

sendebuffer.Zahl = zufallszahl;
  if (msgsnd(rc_msgget, &sendebuffer, sizeof(sendebuffer.Zahl), IPC_NOWAIT) == -1)
  {
    printf("Die Zahl konnte nicht gesendet werden. \n");
    perror(msgsnd);
    exit(1);
  }
  else
  {
    printf("Geschickte Zahl: %d \n", sendebuffer.Zahl);
  }

  // Log

  // Nachricht empfangen

  rc_msgrcv = msgrcv(rc_msgget, &recievebuffer, sizeof(recievebuffer.Zahl), recievebuffer.mtype, MSG_NOERROR | IPC_NOWAIT);

  if (rc_msgrcv < 0)
  {
    printf("Die Zahl konnte nicht gelesen werden. \n");
    perror(msgrcv);
    exit(1);
  }
  else
  {
    printf("Die empfangene Zahl: %d \n", recievebuffer.Zahl);
  }
  // Zahl in Dokument schreiben
  FILE *ed;
  ed = fopen("Message_queue_File", "a");
  if (ed == 0)
  {
    printf("Datei kann nicht geöffnet werden!");
  }
  else
  {
    fprintf(ed, "%d\n", recievebuffer.Zahl);
  }

  // Stat
  int zvonConv;
  int *speicher = malloc(sizeof(int));
  if (speicher == NULL)
  {
    return 1;
  }

  int schleifenanzahl = 1;
  if (schleifenanzahl == 1)
  {
    *speicher = zufallszahl;
  }
  else
  {
    speicher = realloc(speicher, sizeof(int) + schleifenanzahl);
    speicher[schleifenanzahl - 1] = zvonConv;
  }

  int Summe;
  int Mittelwert;

  for (int x = 0; x < sizeof(speicher); x++)
  {
    Summe += speicher[x];
  }
  Mittelwert = Summe / schleifenanzahl;

  int Summe2 = 0;
  int Mittelwert2 = 0;

  Summe2 += zvonConv;
  Mittelwert2 = Summe2 / schleifenanzahl;

  printf("Summe aus Weg 2: %d\n", Summe2);
  printf("Mittelwert aus Weg 2: %d\n", Mittelwert2);

  rc_msgctl = msgctl(rc_msgget, IPC_RMID, 0);
  if (rc_msgctl < 0) {
    printf("Die Warteschlange konnte nicht gelÃ¶scht werden.\n");
    perror("msgctl");
    exit(1);
  } else { 
    printf("%i mit ID %i wurde gelÃ¶scht.\n", mq_key, rc_msgget);
  }

  return 0;
}
