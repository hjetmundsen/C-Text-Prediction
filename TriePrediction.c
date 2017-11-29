#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TriePrediction.h"

//The printTrieHelper function was written entirely by Dr. Szumlanski
void printTrieHelper(TrieNode *root, char *buffer, int k) {
	int i;

	if (root == NULL)
		return;

	if (root->count > 0)
		printf("%s (%d)\n", buffer, root->count);

	buffer[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		printTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

//The printTrie function was written entirely by Dr. Szumlanski
void printTrie(TrieNode *root, int useSubtrieFormatting){
	char buffer[1026];

	if (useSubtrieFormatting)
	{
		strcpy(buffer, "- ");
		printTrieHelper(root, buffer, 2);
	}
	else
	{
		strcpy(buffer, "");
		printTrieHelper(root, buffer, 0);
	}
}

void countStrings(TrieNode *root, int *numStrings){
	int i;

    if(root == NULL)
        return;

    for(i = 0; i < 26; i++){
        countStrings(root -> children[i], numStrings);
    }

    if((root -> count) > 0)
        (*numStrings) = (*numStrings) + (root -> count);
}

TrieNode *createNode(){
    int i;
    TrieNode *root = calloc(1, sizeof(TrieNode));
    return root;
}

void largestCount(TrieNode *root, int *max){
    int i;

    if(root == NULL)
        return;

    if(root -> count > (*max))
        *max = root -> count;

    for(i = 0; i < 26; i++){
        largestCount(root -> children[i], max);
    }

}

void stripPunctuation(char *str, char *temp, int *end){
	int i;
	int j = 0;
    int index = strlen(str);

	for(i = 0; i < index; i++){
		if(isalpha(str[i])){
			temp[j] = tolower(str[i]);
			j++;
		}

		else if(str[i] == '.' || str[i] == '?' || str[i] == '!')
			*end = 1;
	}

    strcpy(str, temp);
}

void insertString(TrieNode *root, char *str){
    int i, index, len = strlen(str);
    TrieNode *temp = root;

    for(i = 0; i < len; i++){
        index = tolower(str[i]) - 'a';

        if(temp -> children[index] == NULL)
            temp -> children[index] = createNode();

        temp = temp -> children[index];
    }

    temp -> count++;
    if(temp -> subtrie == NULL)
        temp -> subtrie = createNode();
}

void insertSubtrie(TrieNode *root, char *str){
    int i, index, len = strlen(str);
    TrieNode *temp = root;

    for(i = 0; i < len; i++){
        index = tolower(str[i]) - 'a';

        if(temp -> children[index] == NULL)
            temp -> children[index] = createNode();

        temp = temp -> children[index];
    }

    temp -> count++;

}

TrieNode *buildTrie(char *filename) {
    FILE *fp = NULL;
    int end;
    char *buffer = calloc(MAX_CHARACTERS_PER_WORD + 1, sizeof(char));
    TrieNode *root = NULL;
    char *temp;
    TrieNode *sub = NULL;

    root = createNode();

    fp = fopen(filename, "r");

    while(fscanf(fp, "%s", buffer) != EOF){
        end = 0;
		temp = calloc(MAX_CHARACTERS_PER_WORD + 1, sizeof(char));
        stripPunctuation(buffer, temp, &end);

        if(sub){
            insertSubtrie(sub, buffer);
        }

        insertString(root, buffer);

        if(end != 1){
            sub = getNode(root, buffer);
            sub = sub -> subtrie;
        } else {
            sub = NULL;
        }

		free(temp);
    }

    free(buffer);
    fclose(fp);

    return root;
}

TrieNode *destroyTrie(TrieNode *root) {
    if(root == NULL)
        return NULL;

    if(root -> subtrie != NULL)
        destroyTrie(root -> subtrie);

    for(int i = 0; i < 26; i++){
        destroyTrie(root -> children[i]);
    }

    free(root);
    root = NULL;
    return root;
}

TrieNode *getNode(TrieNode *root, char *str) {
    int num, i;
    TrieNode *temp = root;

    if(root == NULL)
        return root;

    for(i = 0; i < strlen(str); i++){
        num = tolower(str[i]) - 'a';

        temp = temp -> children[num];

        if(temp == NULL)
            return temp;
    }

    if(temp -> count < 1)
        return NULL;

    return temp;

}

void frequentString(TrieNode *root, char *temp, char* temp2, int *incrementer, int *max){
    int i;

    if(root == NULL){
        return;
    }

    if(root -> count == (*max)){
        if(temp[0] == '\0')
            strcpy(temp, temp2);
    }

    for(i = 0; i < 26; i++){
        if(root -> children[i] != NULL){
            temp2[*incrementer] = 'a' + i;
            (*incrementer)++;
        }

        frequentString(root -> children[i], temp, temp2, incrementer, max);

    }

    temp2[*incrementer] = '\0';
    (*incrementer)--;
}

void getMostFrequentWord(TrieNode *root, char *str) {
    int max = 0;
    int incrementer = 0;
    char *temp = calloc(MAX_CHARACTERS_PER_WORD + 1, sizeof(char));
    char *temp2 = calloc(MAX_CHARACTERS_PER_WORD + 1, sizeof(char));

    largestCount(root, &max);

    frequentString(root, temp, temp2, &incrementer, &max);

    strcpy(str, temp);
    free(temp);
    free(temp2);
}

int containsWord(TrieNode *root, char *str) {
    return (getNode(root, str) == NULL) ? 0 : 1;
}

int prefixCount(TrieNode *root, char *str) {
    TrieNode *temp = root;
    int *numStrings = malloc(sizeof(int));
    int result, i;
    *numStrings = 0;

    for(i = 0; i < strlen(str); i++){
        if(temp -> children[str[i] - 'a'] == NULL){
            free(numStrings);
            return 0;
        }

        temp = temp -> children[str[i] - 'a'];
    }

    countStrings(temp, numStrings);
    result = *numStrings;
    free(numStrings);
    return result;
}

double difficultyRating(void) {
    return 4.5;
}

double hoursSpent(void) {
    return 10.0;
}

int main(int argc, char **argv){
    FILE* fp = NULL;
    char *filename = argv[1];
    char *input = argv[2];
    int n = 0, i;
    TrieNode *temp = NULL;
    TrieNode* root = buildTrie(filename);
    int incrementer = 0;
    int max = 0;
    char *str = NULL;
    char *buffer = calloc(MAX_CHARACTERS_PER_WORD + 1, sizeof(char));

    fp = fopen(input, "r");

    while(fscanf(fp, "%s", buffer) != EOF){
        max = 0;
		str = calloc(MAX_CHARACTERS_PER_WORD + 1, sizeof(char));

        if(strcmp(buffer, "!") == 0){
            printTrie(root, 0);
		}

        else if(strcmp(buffer, "@") == 0){
            fscanf(fp, "%s", str);
            fscanf(fp, "%d", &n);

            for(i = 0; i <= n; i++){
                printf("%s", str);

                temp = getNode(root, str);

                if(temp == NULL || temp -> subtrie == NULL)
                    break;

                getMostFrequentWord(temp -> subtrie, str);
                if(i < n && str[0] != '\0')
                    printf(" ");

            }

            printf("\n");
        }

        else {
            temp = getNode(root, buffer);
            printf("%s\n", buffer);

            if(temp != NULL)
                countStrings(temp -> subtrie, &max);

            if(temp == NULL){
                printf("(INVALID STRING)\n");
            }

            else if(max == 0){
                printf("(EMPTY)\n");
            }

            else {
                printTrie(temp -> subtrie, 1);
            }
        }

		free(str);
    }

    fclose(fp);
	free(buffer);
	root = destroyTrie(root);

    return 0;
}
