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

finalKeyValueDS *tempNode = NULL;
while (root != NULL){
tempNode = root;
root = root -> next;
free(tempNode);
}
}

// reduce function
void reduce(char *key) {
	FILE* word = fopen(key,"r");
	char buff[255];
	char fileName[100];
	int count=0;
	int c;

	//fscanf gets first word of file
	fscanf(word, "%s", fileName);
  //printf("%s\n",fileName);
  
	while(c = fgetc(word) != EOF)	{ //count numer of 1s in text file
		if(c==1) {
			++count;
		}
	}
	rootReduce = insertNewKeyValue(rootReduce, fileName, count);
  fclose(word);
}

// write the contents of the final intermediate structure
// to output/ReduceOut/Reduce_reducerID.txt
void writeFinalDS(int reducerID){
  
  	finalKeyValueDS *tempNode = rootReduce;
		if(tempNode == NULL){
       return;
    }
    char dir[100] = "output/ReduceOut/";
    char id[100];
    char numOfRepeats[100];
    char tempName[100]; 
    
    sprintf(id, "%d", reducerID);
    FILE* word = fopen(strcat(strcat(dir, "Reducer_"),strcat(id,".txt")),"w");
     
		while (tempNode != NULL){
      fputs(tempNode->key,word); //write word to final text file
      fputs(" ", word);
      sprintf(numOfRepeats,"%d",tempNode->value); 
      fputs(numOfRepeats,word); //write final count to text file
      fputs("\n",word);
			tempNode = tempNode -> next;
		}
    fclose(word);
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
  freeFinalDS(rootReduce);
	return 0;
}
