#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
void simple_hash(char *input, char *output)
{
    unsigned int hash = 0;
    for (int i = 0; i < strlen(input); i++)
    {
        hash = hash * 31 + input[i];
    }
    sprintf(output, "%u", hash);
}

typedef struct Block
{
    int index;
    char timestamp[100];
    char data[256];
    char previousHash[256];
    char hash[256];
    struct Block *next;
} Block;
void get_timestamp(char *buffer)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, 100, "%Y-%m-%d %H:%M:%S", t);
}
Block *create_block(int index, char *data, char *previousHash)
{
    Block *newBlock = (Block *)malloc(sizeof(Block));
    newBlock->index = index;
    strcpy(newBlock->data, data);
    get_timestamp(newBlock->timestamp);
    strcpy(newBlock->previousHash, previousHash);
    char toHash[1024];
    snprintf(toHash, sizeof(toHash), "%d%s%s%s", index, newBlock->timestamp, data, previousHash);
    simple_hash(toHash, newBlock->hash);

    newBlock->next = NULL;
    return newBlock;
}

void add_block(Block **head, char *data)
{
    if (*head == NULL)
    {
        *head = create_block(0, data, "0");
        return;
    }

    Block *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    Block *newBlock = create_block(temp->index + 1, data, temp->hash);
    temp->next = newBlock;
}

void print_chain(Block *head)
{
    Block *temp = head;
    while (temp != NULL)
    {
        printf("Block #%d\n", temp->index);
        printf("Timestamp: %s\n", temp->timestamp);
        printf("Data: %s\n", temp->data);
        printf("Previous Hash: %s\n", temp->previousHash);
        printf("Hash: %s\n", temp->hash);
        printf("-------------------------------\n");
        temp = temp->next;
    }
}
void free_chain(Block *head)
{
    Block *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main()
{
    Block *blockchain = NULL;

    add_block(&blockchain, "Genesis Block");
    add_block(&blockchain, "Alice pays Bob 10 coins");
    add_block(&blockchain, "Bob pays Charlie 5 coins");
    print_chain(blockchain);
    free_chain(blockchain);
    return 0;
}
