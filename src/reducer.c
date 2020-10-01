#include "reducer.h"
finalKeyValueDS *rootReduce;
// create a key value node
finalKeyValueDS *createFinalKeyValueNode(char *word, int count){
	finalKeyValueDS *newNode = (finalKeyValueDS *)malloc (sizeof(finalKeyValueDS));
	strcpy(newNode -> key, word);
	newNode -> value = count;
	newNode -> next = NULL;
	return newNode;
}

// insert or update an key value
finalKeyValueDS *insertNewKeyValue(finalKeyValueDS *root, char *word, int count){
	finalKeyValueDS *tempNode = root;
	if(root == NULL)
		return createFinalKeyValueNode(word, count);
	while(tempNode -> next != NULL){
		if(strcmp(tempNode -> key, word) == 0){
			tempNode -> value += count;
			return root;
		}
		tempNode = tempNode -> next;
	}
	if(strcmp(tempNode -> key, word) == 0){
		tempNode -> value += count;
	} else{
		tempNode -> next = createFinalKeyValueNode(word, count);
	}
	return root;
}

// free the DS after usage. Call this once you are done with the writing of DS into file
void freeFinalDS(finalKeyValueDS *root) {
	if(root == NULL) return;

	finalKeyValueDS *tempNode = root -> next;;
	while (tempNode != NULL){
		free(root);
		root = tempNode;
		tempNode = tempNode -> next;
	}
}

// reduce function
void reduce(char *key) {
	FILE* word = fopen(key,"r");
	char buff[255];
	char fileName[100];
	int count=0;
	char *c;
	//fscanf gets first word of file
	fscanf(word, "%s", fileName);

	while(*c = fgetc(word) != EOF)
	{
		if(strcmp(c,"1")==0)
		{
			++count;
		}
	}
	insertNewKeyValue(rootReduce, fileName, count);
}

// write the contents of the final intermediate structure
// to output/ReduceOut/Reduce_reducerID.txt
void writeFinalDS(int reducerID){
		if(rootReduce == NULL) return;

		finalKeyValueDS *tempNode = rootReduce -> next;
		while (tempNode != NULL){
	    char dir[100] = "output/ReduceOut/Reduce_";
	    char id[100];
			char key[100];
	    //itoa(mapperID, id, 10);
	    sprintf(id, "%d", reducerID);
			FILE* word = fopen(strcat(strcat(dir, id),strcat(tempNode->key,".txt")),"w");
				fputs(tempNode->key,word);
				sprintf(key,"%d",tempNode->value);
	    	fclose(word);
			rootReduce = tempNode;
			tempNode = tempNode -> next;
		}

}

int main(int argc, char *argv[]) {

	if(argc < 2){
		printf("Less number of arguments.\n");
		printf("./reducer reducerID");
	}

	// ###### DO NOT REMOVE ######
	// initialize
	int reducerID = strtol(argv[1], NULL, 10);

	// ###### DO NOT REMOVE ######
	// master will continuously send the word.txt files
	// alloted to the reducer
	char key[MAXKEYSZ];
	while(getInterData(key, reducerID))
		reduce(key);

	// You may write this logic. You can somehow store the
	// <key, value> count and write to Reduce_reducerID.txt file
	// So you may delete this function and add your logic
	writeFinalDS(reducerID);

	return 0;
}
