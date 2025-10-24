#include "stdio.h"
#include "windows.h"


HANDLE g_ToChild = NULL;     
HANDLE g_FromChild = NULL;   
HANDLE g_FromParent = NULL;  
HANDLE g_ToParent = NULL;    

int CreateChildProcess();

int main() {
  SECURITY_ATTRIBUTES saAttr; //структура чтобы создавать пайпы
  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); //контроль версий api, разные виндовсы - разные сайзофы
  saAttr.bInheritHandle = TRUE;  
  saAttr.lpSecurityDescriptor = NULL;

 
  if (!CreatePipe(&g_FromChild, &g_ToParent, &saAttr, 0)) return -1;
//г фром чайлд это родительская штучка

  if (!SetHandleInformation(g_FromChild, HANDLE_FLAG_INHERIT, 0)) return -1;
//хендл наследуется ребенком, передаем значение ребенку от родителя, и оно будет валидным

  if (!CreatePipe(&g_FromParent, &g_ToChild, &saAttr, 0)) return -1;
//создаем еще пайп, этот будет возвращать(хз)(от родителя к ребенку)
  if (!SetHandleInformation(g_ToChild, HANDLE_FLAG_INHERIT, 0)) return -1;
  //хендл наследуется ребенком
  if (CreateChildProcess() != 0) return -1; //создает потомка


  HANDLE writeHandle; //указатель на что-то непрозрачное


  writeHandle = GetStdHandle(STD_INPUT_HANDLE); //стандартный ввод
  HANDLE readHandle = GetStdHandle(STD_OUTPUT_HANDLE); //стандартный вывод

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
    } //удаляем переводы строки и делаем их нулями
  } else {
    return -1;
  }
  DWORD realLen = dwReadFileName;
 
  WriteFile(g_ToChild, buffer, realLen, &dwReadFileName, NULL);
//г ту чайлд это от родителя к ребенку строчка 54
  system("cls"); 
//очистка экрана
    
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
  //пишет в пайп ребенку(код где родитель к ребенку)

 float result = 0.0f;
  DWORD bytesRead;
  BOOL readSuccess =
      ReadFile(g_FromChild, &result, sizeof(float), &bytesRead, NULL);
//читаем что вернет ребенок
  if (readSuccess != FALSE && bytesRead == sizeof(float)) {
    char header[] = "Result: ";
    char newline[] = "\r\n";
    char floatStr[50];

    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), header, (DWORD)strlen(header),
              &dwRead, NULL); //на экран выписали хедер

   
    _gcvt_s(floatStr, sizeof(floatStr), result, 6);
    //из числа с плав точкой делает строку 
    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), floatStr,
              (DWORD)strlen(floatStr), &dwRead, NULL);
    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), newline, (DWORD)strlen(newline),
              &dwRead, NULL);
  }

  CloseHandle(g_ToChild);
  CloseHandle(g_FromChild);
  //закрыли родителей
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