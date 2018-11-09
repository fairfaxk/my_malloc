#include <unistd.h>

void *my_malloc(size_t size);
void my_free(void *ptr);

typedef struct freenode
{
  size_t size;
  struct freenode *next;

} freenode;

size_t page_size = getpagesize();

freenode *head = NULL;

void *my_malloc(size_t size){
	//Has to allocate a positive amount of space
	if(size<1){return NULL;}	

	void* ptr;

	size_t padded_size = size + 8;

	if(padded_size < 32) { padded_size = 32; }

	else if(padded_size%16 != 0) { padded_size = ((padded_size/16)+1)*16; }

	if(head == NULL){
		head = (freenode *) sbrk(page_size);
		if(head == (void*)-1){ return NULL; }

		char* addr = (char*)head;
		addr+=8;

		head = (freenode*)addr;
		head->size = page_size-8;
		head->next = NULL;
	}

	//Head can never be null here
	freenode* FN = head;
	freenode* prev = NULL;
	//Loop through entire list looking for node with enough size. FN is null if we don't find what we need
	while(FN!=NULL){
		if(FN->size>=padded_size){
			break;
		}
		else{
			prev = FN;
			FN = FN->next;
		}
	}
	//If we didn't have enough space in our list, create a new block with enough memory
	if(FN==NULL){
		int numPages = padded_size/page_size;
		
		if(padded_size%page_size!=0){
			numPages += 1;
		}
		
		FN = (freenode *) sbrk(numPages * page_size);
		
		if(FN == (void*)-1){return NULL;}

		char* addr = (char*) FN;
		addr+=8;

		FN = (freenode*) addr;

		FN->size = ((numPages * page_size)-8);
		FN->next = head;
		
		prev = NULL;
		head=FN;
	}
	
	int remSize = (FN->size - padded_size);

	if(remSize<32){
		/*
		  Remove FN from list
		*/
		//freenode *temp = head;
		
		//If head is the only node then make the list empty
		if(head->next==NULL){
			head=NULL;
		}
		else if(prev==NULL){
			head = head->next;
		}
		//Elsewise find FN and remove the pointer to it
		else{
			prev->next = FN->next;
			/*while(temp->next!=NULL){
				if(temp->next==FN){
					temp->next=FN->next;
					break;
				}
				else{
					temp = temp->next;
				}
			}*/
		}
	}
	else{
		/*
		  Remove remaining space from FN (subtract from size)
		  Add a new node with that much size
		  remove FN from freelist
		*/
		
		//Remove saved bytes from FN
		FN->size = padded_size;
		
		//Create a new freenode with those bits
		char* addr = (char*)FN;
		addr = addr+padded_size;

		freenode *temp = (freenode*)addr;
		temp->size = remSize;
		temp->next = FN->next;

		//freenode *curr = head;
		
		//If only the head is in the freelist, then change freelist to be the remaining bytes
		if(head->next==NULL){
			head = temp;
		}
		else if(prev==NULL){
			head = head->next;
		}
		else{
			prev->next = temp;
			/*//Add remaining bytes and remove assigned bytes
			while(curr->next!=NULL){
				if(curr->next==FN){
					curr->next=temp;
					break;
				}
				else{
					curr = curr->next;
				}
			}*/
		}
	}

	/*
	  Assign FN to user
	*/
	char* addr = (char*) FN;
	ptr = addr + 8;

	return ptr;
}

void my_free(void* ptr){
	char* addr = (char*)ptr;
	addr-=8;
	freenode* FN = (freenode*)addr;
	
	if(FN==head){//do nothing
	}
	else{
		FN->next = head;
		head = FN;
	}
	
	return;
}


