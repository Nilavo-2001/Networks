#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

int n, r;

struct frame {
    char ack;
    int data;
} frm[10];

int sender(void);
void recvack(void);
void resend(void);
//void resend1(void);
void goback(void);
void selective(void);

int main() {
    int c;

    do {
        printf("\n\n1. Selective repeat ARQ\n2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &c);

        switch (c) {
            case 1:
                selective();
                break;

            case 2:
                exit(0);
                break;

            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (c != 2);

    return 0;
}


void selective() {
    sender();
    recvack();
    resend();
    printf("\nAll packets sent successfully.\n");
}

int sender() {
    int i;

    printf("Enter the number of packets to be sent: ");
    scanf("%d", &n);

    for (i = 1; i <= n; i++) {
        printf("Enter data for packet [%d]: ", i);
        scanf("%d", &frm[i].data);
        frm[i].ack = 'y';
    }

    return 0;
}

void recvack() {
    int i;

    srand(time(NULL));
    r = rand() % n;

    frm[r].ack = 'n';

    for (i = 1; i <= n; i++) {
        if (frm[i].ack == 'n') {
            printf("The packet number %d is not received.\n", r);
            break;
        }
    }
}

void resend() {  // SELECTIVE REPEAT
    printf("Resending packet %d.", r);
    sleep(2);
    frm[r].ack = 'y';
    printf("The received packet is %d.\n", frm[r].data);
}

