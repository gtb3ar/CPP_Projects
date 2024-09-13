// FileIO.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
using namespace std;


class FileInterface
{
private:
    char* path;
    char* buffer;
    size_t bufferSize;
public:

    FileInterface(char* Path) {
        path = Path;
        FILE* file = fopen(path, "r");
        if (file == nullptr) {
            printf("File failed to open at path. \n");
        } 
        else {
            printf("File open. \n");
            fclose(file);
        }
        buffer = nullptr;
        bufferSize = 0;
    }

    ~FileInterface() {
        printf("FileInterface destroyed");
    }

    void FileToBuffer() {
        if (buffer != nullptr) {
            free(buffer);
        }

        FILE* file = fopen(path, "rb");
        if (file != nullptr) {
            
            //Obtain file size
            long fileSize;
            fseek(file, 0, SEEK_END); //Ptr, to end of "file"
            fileSize = ftell(file); //Get pointer

            rewind(file); //Take ptr back to start

            char* memoryBuffer = reinterpret_cast<char*>(malloc(sizeof(char) * fileSize + sizeof(char))); //Make buffer (allocation) in memory for entire file

            if (memoryBuffer != nullptr) {
                size_t read = fread(memoryBuffer, sizeof(char), fileSize, file); // copy file into the buffer
                if (read == fileSize) {
                    bufferSize = read;
                    fclose(file);
                    printf("File, stored in buffer. \n");
                    buffer = memoryBuffer; // return the buffer
                    memoryBuffer += fileSize;
                    *(memoryBuffer) = '\0';
                }
            }
            
        }
    }

    void BufferToFileOverwrite() {
        FILE* file = fopen(path, "wb");
        if (file != nullptr) {
            fwrite(buffer, bufferSize, sizeof(char), file);
            fclose(file);
            printf("Buffer written to file. \n");
        }
    }

    void BufferToFileAppend() {
        FILE* file = fopen(path, "a+b");
        if (file != nullptr) {
            fwrite(buffer, sizeof(char), bufferSize, file);
            fclose(file);
            printf("Buffer written to file. \n");
        }
    }

    void PrintBuffer() {
        char* printBuffer = buffer;
        printf("\nPrinting file -> \n");
        if (*printBuffer != '\0') {
            printf(printBuffer);
        }
        printf("\n\n");
    }

    void FreeBuffer() {
        free(buffer);
    }


};

int main()
{   
    char str[44] = "Hello.txt";
    char* strPtr = str;
    FileInterface* file = new FileInterface(strPtr);
    
    
    file->FileToBuffer();
    file->PrintBuffer();
    file->BufferToFileAppend();
    file->FileToBuffer();
    file->PrintBuffer();
}

