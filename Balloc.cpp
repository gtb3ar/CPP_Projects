#include <iostream>
#include <cassert>

class MemBlock {
// Variables
public:
	void* userDataPtr;  // Ptr to the user data
	size_t size;        // Size of the user Data
	MemBlock* next;     // Next memory block in the linked list
	int id;             // Used to recognise if the block is free or not, id:0 means the block is free. id>0 means the block is allocated
	static const unsigned int GuardBandSizeBytes = 16; // How many bytes of the guardbands should be	
private:
	static const unsigned char guardBandChar = 0b11001011;
		

// Functions
public:
	size_t GetTotalMemblockSize() {
		return sizeof(MemBlock) + sizeof(char) * GuardBandSizeBytes + size;
	}

	static size_t GetEmptyMemblockSize() {
		return sizeof(MemBlock) + sizeof(char) * GuardBandSizeBytes;
	}

	size_t GetRemainingMemory(size_t requiredMemory) {
		return size - requiredMemory;
	}

	MemBlock* GetPredictedNextBlock() {
		return reinterpret_cast<MemBlock*>(reinterpret_cast<char*>(this) + sizeof(MemBlock) + sizeof(char) * GuardBandSizeBytes * 2 + size);
	}

	//Initialiser for a memory block
	void Init(size_t Size, MemBlock* Next, int ID) {
		size = Size;
		next = Next;
		id = ID;
		userDataPtr = reinterpret_cast<char*>(this) + sizeof(MemBlock) + sizeof(unsigned char) * GuardBandSizeBytes;
		if (id > 0) {
			ConstructGuardBands();
		}
	}

	// Checks bands are intact
	bool ValidateGuardBandConsistancy() {
		unsigned char* leftBandPtr = reinterpret_cast<unsigned char*>(userDataPtr) - GuardBandSizeBytes;
		unsigned char* rightBandPtr = reinterpret_cast<unsigned char*>(userDataPtr) + size;
		for (int current = 0; current < GuardBandSizeBytes; current++) {
			if (*(leftBandPtr - current) != guardBandChar || *(rightBandPtr + current) != guardBandChar) {   
				return false;
			}
		}
		return true;
	}

private:
	// Apply guardbands around the block        
	void ConstructGuardBands() { 
		unsigned char* leftBandPtr = reinterpret_cast<unsigned char*>(userDataPtr) - GuardBandSizeBytes;
		unsigned char* rightBandPtr = reinterpret_cast<unsigned char*>(userDataPtr) + size;
		for (int current = 0; current < GuardBandSizeBytes; current++) {
			*(leftBandPtr - current) = guardBandChar;
			*(rightBandPtr + current) = guardBandChar;
		}
	}
};


class Allocator {
private:

	void* mallocPtr;    // Ptr to the malloc allocation where the memory blocks are stored, required to release
	MemBlock* headBlock;

	// Function to find a freespace big enough for an allocation, worst fit
	MemBlock* FindFreeSpace(size_t size)
	{
		MemBlock* chosenBlock = nullptr;
		if (size > 0) { // Checks size is greater than 0
			MemBlock* currentBlock = headBlock;

			while (currentBlock != nullptr) {
				// Check if block is free
				if (currentBlock->id == 0) {		
					if (chosenBlock == nullptr && currentBlock->size >= size) {
						// First valid free block found
						chosenBlock = currentBlock;
					}
					else {
						if (chosenBlock != nullptr && currentBlock->size > chosenBlock->size) {
							// Better choice of free block found
							chosenBlock = currentBlock;
						}
					}
				}
				currentBlock = currentBlock->next; // Move to next MemBlock
			}
		}
		return chosenBlock;
	}

	// Merges a free block (second) onto another free block(first)
	void MergeFreeBlock(MemBlock* first, MemBlock* second) { 
		assert(first->next == second);
		size_t tempSize = second->GetTotalMemblockSize();
		MemBlock* tempNext = second->next;
		release(second);
		first->size = first->size + tempSize;	
		first->next = tempNext;
	}

public:
	// Set up the class for allocating
	void Init(size_t TotalSize) {
		mallocPtr = malloc(TotalSize);
		if (mallocPtr != nullptr) {
			headBlock = reinterpret_cast<MemBlock*>(mallocPtr);
			headBlock->Init(TotalSize - sizeof(MemBlock), nullptr, 0);
		}
	}

	~Allocator() { // On deconstruction, free malloc
		if (mallocPtr != nullptr) {
			free(mallocPtr);
		}
	}

	/* this function will find free space within the allocated momeory,
	then replace said space with an alloc, giving a pointer to the next free space after*/
	void* alloc(size_t size) {
		if (size == 0) {
			return nullptr;
		}
		MemBlock* block1 = FindFreeSpace(size + MemBlock::GetEmptyMemblockSize()); //Finding space for the allocation
		size_t remainder = block1->GetRemainingMemory(size) - sizeof(MemBlock);
		MemBlock* next = block1->next;
		block1->Init(size, nullptr, 1);

		if (remainder == 0)
		{ // Allocation is a perfect fit ->
			block1->next = next;
			return block1->userDataPtr;
		}
		else
		{ // There is empty space after allocation ->
			MemBlock* block2 = block1->GetPredictedNextBlock();
			block2->Init(remainder, next, 0);
			block1->next = block2;
		}
		return block1->userDataPtr;
	}

	void release(void* ptr) { // Much like 'free' a function to un allocate a block
		MemBlock* block = reinterpret_cast<MemBlock*>(reinterpret_cast<char*>(ptr) - sizeof(MemBlock) - MemBlock::GuardBandSizeBytes);
		if (block != nullptr && block->id != 0) {
			block->id = 0;
			block->userDataPtr = nullptr;
			if (block->next->id == 0) { // Performs a cleanup operation to prevent segmentation
				MergeFreeBlock(block, block->next);
			}
		}
	}

	bool GuardBandConsitancyCheck() { //Check all guardbands are intact
		MemBlock* block = headBlock;
		while (block != nullptr) {
			if (block->id > 0 && !block->ValidateGuardBandConsistancy()) {
				return false;
			}
			block = block->next;
		}
		return true;
	}
};


int main()
{
	Allocator allocation;
	allocation.Init(8000);
	void* ptr = allocation.alloc(sizeof(int));
	void* ptr2 = allocation.alloc(sizeof(int));
	*(reinterpret_cast<int*>(ptr)) = 1;
	*(reinterpret_cast<int*>(ptr2)) = 2;
	printf("%d,%d\n", *(reinterpret_cast<int*>(ptr)), *(reinterpret_cast<int*>(ptr2)));
	printf("%d\n", allocation.GuardBandConsitancyCheck());
	allocation.release(ptr);
	printf("%d\n", allocation.GuardBandConsitancyCheck());
}

