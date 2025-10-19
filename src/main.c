#include "stdio.h"
#include "windows.h"


HANDLE g_ToChild = NULL;     
HANDLE g_FromChild = NULL;   
HANDLE g_FromParent = NULL;  
HANDLE g_ToParent = NULL;    

int CreateChildProcess();

int main() {
  SECURITY_ATTRIBUTES saAttr;
  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;  
  saAttr.lpSecurityDescriptor = NULL;

 
  if (!CreatePipe(&g_FromChild, &g_ToParent, &saAttr, 0)) return -1;


  if (!SetHandleInformation(g_FromChild, HANDLE_FLAG_INHERIT, 0)) return -1;


  if (!CreatePipe(&g_FromParent, &g_ToChild, &saAttr, 0)) return -1;

  if (!SetHandleInformation(g_ToChild, HANDLE_FLAG_INHERIT, 0)) return -1;
  if (CreateChildProcess() != 0) return -1;


  HANDLE writeHandle;


  writeHandle = GetStdHandle(STD_INPUT_HANDLE);
  HANDLE readHandle = GetStdHandle(STD_OUTPUT_HANDLE);

  char buffer[256]; 
  DWORD dwReadFileName; 
  if (ReadFile(writeHandle, buffer, sizeof(buffer) - 1, &dwReadFileName,
               NULL)) {
    buffer[dwReadFileName] = '\0'; 
    for (int i = 0; i < 256; i++) {
      if (buffer[i] == '\r' || buffer[i] == '\n') {
        buffer[i] = '\0';
        break;
      }
    }
  } else {
    return -1;
  }
  DWORD realLen = dwReadFileName;
 
  WriteFile(g_ToChild, buffer, realLen, &dwReadFileName, NULL);
  system("cls"); 

    
    DWORD dwRead;
    char numStr[256];

    if (ReadFile(writeHandle, numStr, sizeof(numStr) - 1, &dwRead, NULL) &&
        dwRead > 0) {
      numStr[dwRead] = '\0';

     
      int countToBreak = 0;
      for (int i = 0; i < 256; i++) {
        
        if (numStr[i] == '\r' || numStr[i] == '\n') {
          numStr[i] = '\0';
          break;
        } 
      }
      


      
    } 
   
    
  
  WriteFile(g_ToChild, numStr, dwRead, &dwRead, NULL);

 float result = 0.0f;
  DWORD bytesRead;
  BOOL readSuccess =
      ReadFile(g_FromChild, &result, sizeof(float), &bytesRead, NULL);

  if (readSuccess != FALSE && bytesRead == sizeof(float)) {
    char header[] = "Result: ";
    char newline[] = "\r\n";
    char floatStr[50];

    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), header, (DWORD)strlen(header),
              &dwRead, NULL);

   
    _gcvt_s(floatStr, sizeof(floatStr), result, 6);
    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), floatStr,
              (DWORD)strlen(floatStr), &dwRead, NULL);
    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), newline, (DWORD)strlen(newline),
              &dwRead, NULL);
  }

  CloseHandle(g_ToChild);
  CloseHandle(g_FromChild);
  return 0;
}

int CreateChildProcess() {
  TCHAR szCmdline[] = TEXT("ChildProcess.exe");
  PROCESS_INFORMATION piProcInfo;
  STARTUPINFO siStartInfo;
  BOOL bSuccess = FALSE;

  ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

  
  ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
  siStartInfo.cb = sizeof(STARTUPINFO);
  siStartInfo.hStdError = g_ToParent;    
  siStartInfo.hStdOutput = g_ToParent;   
  siStartInfo.hStdInput = g_FromParent;  
  siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

  // Создаем дочерний процесс
  bSuccess = CreateProcess(NULL,
                           szCmdline,     
                           NULL,          
                           NULL,         
                           TRUE,          
                           0,             
                           NULL,         
                           NULL,         
                           &siStartInfo,  
                           &piProcInfo);  

  if (!bSuccess)
    return -1;
  else {
  
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);

  
    CloseHandle(g_ToParent);  
    CloseHandle(g_FromParent);
  }
  return 0;
}