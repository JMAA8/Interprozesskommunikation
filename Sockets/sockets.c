#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<time.h>
#include<string.h>
#include<sys/wait.h>

int main(int argc, char* argv[]){
    int sv1[2];
    int sv2[2];
    int sv3[2];
    int sv4[2];
    int buf;
    

//conf -----> log
 if (socketpair(AF_UNIX,SOCK_STREAM,0, sv1)==-1){
        perror("socketpair 1 ");
        exit(1);
    }

//conf ----> stat
 if (socketpair(AF_UNIX,SOCK_STREAM,0, sv2)==-1){
        perror("socketpair 2");
        exit(1);
    }

//stat(Summe) ----> report
 if (socketpair(AF_UNIX,SOCK_STREAM,0, sv3)==-1){
        perror("socketpair 3");
        exit(1);
    }

//stat(Mittelwert) ----> report
 if (socketpair(AF_UNIX,SOCK_STREAM,0, sv4)==-1){
        perror("socketpair");
        exit(1);
    }



//Variable zum zählen der Schleifen und reservieren des Speicherplatzes
    int Schleifenzahl = 1;

    //Summe Mittelwert
    int Summe2 = 0;
    int Mittelwert2 = 0;

    //Erstellen von vier Prozessen
    int id1 = fork();
    int id2 = fork();

if (id1 == 0){
        if (id2 == 0){
            //conf
            close(sv1[0]);
            close(sv2[0]);
            close(sv3[0]);
            close(sv3[1]);
            close(sv4[0]);
            close(sv4[1]);

            while (1)
            {
            
            sleep(2);
            time_t zeit = time(NULL);
            srand(zeit);
            buf = rand() %11;
            write(sv1[1],&buf, 1);
            write(sv2[1],&buf, 1);
            //printf("Zahl: %d\n", buf);
            }
        
            
        }else{
            //log
            close(sv1[1]);
            close(sv2[0]);
            close(sv2[1]);
            close(sv3[0]);
            close(sv3[1]);
            close(sv4[0]);
            close(sv4[1]);

            while (1)
            {
                
                read(sv1[0], &buf, 1);
                
                

                FILE *ed;
                ed = fopen("ZufallSockets", "a");
                if(ed == 0){
                printf("Datei kann nicht geöffnet werden!");
                }else{
                fprintf(ed, "%d\n", buf);
                fclose(ed);
                }

            }
            


        }





    }else{
        if (id2 == 0)
        {
            //stat
            close(sv1[1]);
            close(sv1[0]);
            close(sv2[1]);
            close(sv3[0]);
            close(sv4[0]);
            

            while (1)
            {
            int zVonConv;
            read(sv2[0], &zVonConv, 1);

            Summe2 += zVonConv;
            Mittelwert2 = Summe2 / Schleifenzahl;

            //printf("Summe aus Weg 2: %d\n", Summe2);
            //printf("Mittelwert aus Weg 2: %d\n", Mittelwert2);


            write(sv3[1], &Summe2, 1);
            write(sv4[1], &Mittelwert2, 1);
            
            Schleifenzahl++;
            }
            
            
        }else{
            //report
            int SummeAusS;
                int MittelwertAusS;


            close(sv1[1]);
            close(sv1[0]);
            close(sv2[1]);
            close(sv2[0]);
            close(sv3[1]);
            close(sv4[1]);

            while (1)
            {
                

                read(sv3[0], &SummeAusS, 1);
                read(sv4[0], &MittelwertAusS, 1);
                

                printf("Akutelle Summe: %d\n", SummeAusS);
                printf("Aktueller Mittelwert: %d\n", MittelwertAusS);
            }
            

            

        }
        
    }

}