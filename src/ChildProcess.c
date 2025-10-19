#include <stdlib.h>
#include <string.h>
#include <windows.h>

int main() {
  HANDLE readHandle = GetStdHandle(STD_INPUT_HANDLE);
  HANDLE writeHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD readedBytesNameFile, readedBytesDigits, writedBytes;

  char bufferNameFile[256];

  if (ReadFile(readHandle, bufferNameFile, sizeof(bufferNameFile) - 1,
               &readedBytesNameFile, NULL)) {
    bufferNameFile[readedBytesNameFile] = '\0';
  } else {
    return -1;
  }

  HANDLE fileHandle = CreateFileA(bufferNameFile, GENERIC_WRITE, 0, NULL,
                                  CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

  char bufferDigits[256];

  if (ReadFile(readHandle, bufferDigits, sizeof(bufferDigits) - 1,
               &readedBytesDigits, NULL)) {
    bufferDigits[readedBytesDigits] = '\0';

    char* context = NULL;
    char* tok = strtok_s(bufferDigits, " \t\r\n", &context);
    float summa = 0.0f;

    while (tok != NULL) {
      summa += strtof(tok, NULL);
      tok = strtok_s(NULL, " \t\r\n", &context);
    }

  
    if (fileHandle != INVALID_HANDLE_VALUE) {
      char header[] = "Sum: ";
      char newline[] = "\r\n";
      char floatStr[50];

      WriteFile(fileHandle, header, (DWORD)strlen(header), &writedBytes, NULL);

     
      _gcvt_s(floatStr, sizeof(floatStr), summa, 6);
      WriteFile(fileHandle, floatStr, (DWORD)strlen(floatStr), &writedBytes,
                NULL);
      WriteFile(fileHandle, newline, (DWORD)strlen(newline), &writedBytes,
                NULL);
    }

   
    WriteFile(writeHandle, &summa, sizeof(float), &writedBytes, NULL);

  } else {
    if (fileHandle != INVALID_HANDLE_VALUE) {
      CloseHandle(fileHandle);
    }
    return -1;
  }

  if (fileHandle != INVALID_HANDLE_VALUE) {
    CloseHandle(fileHandle);
  }
  return 0;
}