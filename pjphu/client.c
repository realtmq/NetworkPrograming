#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 1000
#define MAX 100

typedef struct
{
    char username[MAX];
    char password[MAX];
    int number_act;
    int number_sign;
    char homepage[MAX];
} DT;

void main(int argc, char *argv[])
{
    // catch wrong input
    if (argc != 3)
    {
        printf("Please input IP address and port number\n");
        return 0;
    }
    // ip_address of server
    // port number
    char *ip_address = argv[1];
    char *port_number = argv[2];
    int port = atoi(port_number);
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Try catch false when connecting
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, ip_address, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    //-----------------connected to server------------------------------

    menuLoginRegister(sock);

    //-----------------disconnect to server----------------------------------
    close(sock);
    return;
}

//in menu khi chua dang nhap
void menuLoginRegister(int sock)
{
    int choose;
    char status[MAX];
    printf("1.Singup\n2.Log in\n3.Quit\nYour choice:");
    scanf("%d", &choose);
    switch (choose)
    {
    case 1:
        strcpy(status, "signup");
        send(sock, (char *)&status, sizeof(status), 0);
        registerUser(sock);
        break;
    case 2:
        strcpy(status, "login");
        send(sock, (char *)&status, sizeof(status), 0);
        loginUser(sock);
        break;
    case 3:
        return;
        break;
    default:
        return;
        break;
    }
}

void registerUser(int sock)
{
    while (1)
    {
        DT account;
        char buffer[MAX];
        char confirmPassword[MAX];
        char status[MAX];
        int g = 0;
        printf("Input username: ");
        g = scanf("%s", account.username);
        getchar();
        printf("Input password: ");
        g = scanf("%s", account.password);
        getchar();
        printf("Confirm password: ");
        g = scanf("%s", confirmPassword);
        getchar();
        if (strcmp(account.password, confirmPassword) != 0)
        {
            printf("Mat khau khong khop\n");
            continue;
        }
        else
        {
            //Gui tai khoan muon dang ky len server
            send(sock, (struct DT *)&account, sizeof(account), 0);
            //nhan thong bao tu server
            recv(sock, status, sizeof(status), 0);
            if (strcmp(status, "ok") == 0)
            {
                puts("dk thanh cong");
            }
            if (strcmp(status, "accountExist") == 0)
            {
                puts(status);
            }
        }
    }
}

void loginUser(int sock)
{
    while (1)
    {
        DT account;
        char buffer[100];
        char password[20];
        char status[100];
        int g = 0;
        printf("Input username: ");
        g = scanf("%s", account.username);
        getchar();
        printf("Input password: ");
        g = scanf("%s", account.password);
        getchar();
        send(sock, (struct DT *)&account, sizeof(account), 0);

        // waiting for response
        recv(sock, status, sizeof(status), 0);
        puts(status);
        //Neu sai mat khau
        if (strcmp(status, "wrongPassword") == 0)
        {
            /* code */
        }
        if (strcmp(status, "accountPassword") == 0)
        {
            /*code*/
        }

        //neu dang nhap thanh cong
        if (strcmp(status, "ok") == 0)
        {
            DT currentUser = account;
            loggedInMenu(sock, currentUser);
        }

        puts("-------------------------");
    }
}

//in menu khi da dang nhap
void loggedInMenu(int sock, DT currentUser)
{
    printf("hello %s\n", currentUser.username);
    printf("1.Show all user\n2.Change password\n4.Create file\n5.Find file by name\n6.Find file by username\n7.Download file\n8.Help\n9.Quit\nYour choice:");
    int choice;
    char status[MAX];
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        strcpy(status, "userCreateFile");
        send(sock, (char *)&status, sizeof(status), 0);
        userCreateFile(sock, currentUser);
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    default:
        break;
    }
}

//dinh dang file
typedef struct Data
{
    char fileName[MAX];
    char content[MAX];
    DT sender;
} Data;

//nguoi dung gui file len sever
void userCreateFile(int sock, DT currentUser)
{
    Data sentData;
    printf("Input file name:\n");
    scanf("%s", sentData.fileName);
    getchar();
    printf("Input content:\n");
    scanf("%s", sentData.content);
    getchar();
    sentData.sender = currentUser;
    send(sock, (struct Data *) & sentData, sizeof(sentData), 0);
}