#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

int n, r;

struct frame {
    char ack;
    int data;
} frm[10];

int sender(void);
void recvack(void);
//void resend(void);
void resend1(void);
void goback(void);
void selective(void);

int main() {
    int c;

    do {
        printf("\n\n1. Go-Back-N ARQ\n2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &c);

        switch (c) {
            case 1:
                goback();
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

void goback() {
    sender();
    recvack();
    resend1();
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

    srand(time_t(NULL));
    r = rand() % n;

    frm[r].ack = 'n';

    for (i = 1; i <= n; i++) {
        if (frm[i].ack == 'n') {
            printf("The packet number %d is not received.\n", r);
            break;
        }
    }
}


void resend1() { // GO-BACK-N
    int i;

    printf("Resending from packet %d.", r);

    for (i = r; i <= n; i++) {
        sleep(2);
        frm[i].ack = 'y';
        printf("Received data of packet %d is %d.\n", i, frm[i].data);
    }
}

