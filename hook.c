#include <windows.h>
#include <commdlg.h>
#include <stdio.h>

typedef WCHAR * (CDECL *PFN_wine_get_dos_file_name)(LPCSTR);
typedef char * (CDECL *PFN_wine_get_unix_file_name)(LPCWSTR);
static PFN_wine_get_dos_file_name p_wine_get_dos_file_name = NULL;
static PFN_wine_get_unix_file_name p_wine_get_unix_file_name = NULL;

#define POPEN_OPEN_EXEC "zenity --file-selection"
#define POPEN_SAVE_EXEC "zenity --file-selection --save --confirm-overwrite"

#define WINCOMMDLGAPI __declspec(dllexport)

#define DLL_FUNC_EXPORT(name, ret_type, ...) \
__declspec(dllexport) ret_type WINAPI name(__VA_ARGS__) 

BOOL comdlg_open_file_dialog(int is_save, int is_wide, void *ofn) {
    UINT32 rand_fname = GetTickCount() + GetCurrentProcessId() + rand();
    char exec_cmd[512] = {0};
    char tmp_dos_path[256] = {0};
    char tmp_unix_path[256] = {0};
    snprintf(tmp_unix_path, sizeof(tmp_unix_path), "/tmp/%u.txt", rand_fname);
    snprintf(tmp_dos_path, sizeof(tmp_dos_path), "Z:\\tmp\\%u.txt", rand_fname);
    if (is_save) {
        snprintf(exec_cmd, 
            sizeof(exec_cmd),
            "start /min /exec Z:\\usr\\bin\\bash -c \"out=$(%s);((${#out}>0))&&echo \"$out\" > %s || echo null > %s\"",
            POPEN_SAVE_EXEC, tmp_unix_path, tmp_unix_path
        );
    } else {
        snprintf(exec_cmd, 
            sizeof(exec_cmd),
            "start /min /exec Z:\\usr\\bin\\bash -c \"out=$(%s);((${#out}>0))&&echo \"$out\" > %s || echo null > %s\"",
            POPEN_OPEN_EXEC, tmp_unix_path, tmp_unix_path
        );
    }
    system(exec_cmd);
    while(1) {
        FILE *fp = fopen(tmp_dos_path, "r");
        if (fp) {
            char buf[1024] = {0};
            char mb_path[1024] = {0};
            WCHAR *win_path;
            if (fgets(buf, sizeof(buf), fp)) {
                fclose(fp);
                unlink(tmp_dos_path);
                if(!strcmp(buf, "null\n")) {
                    return FALSE;
                }
                buf[strcspn(buf, "\n")] = 0;
                if (p_wine_get_dos_file_name) {
                    win_path = p_wine_get_dos_file_name(buf);
                } else {
                    return FALSE;
                }
                if (!win_path) {
                    return FALSE;
                }
                WideCharToMultiByte(CP_UTF8, 0, win_path, -1, mb_path, sizeof(mb_path), NULL, NULL);
                if (is_wide) {
                    LPWSTR lpstrFile = ((LPOPENFILENAMEW)ofn)->lpstrFile;
                    int nMaxFile = ((LPOPENFILENAMEW)ofn)->nMaxFile;
                    MultiByteToWideChar(CP_UTF8, 0, mb_path, -1, lpstrFile, nMaxFile);
                } else { 
                    strncpy(((LPOPENFILENAMEA)ofn)->lpstrFile, mb_path, ((LPOPENFILENAMEA)ofn)->nMaxFile - 1);
                    ((LPOPENFILENAMEA)ofn)->lpstrFile[((LPOPENFILENAMEA)ofn)->nMaxFile - 1] = '\0';
                }
                return TRUE;
            }
            fclose(fp);
        }
        Sleep(200);
    }
    return FALSE;
}

WINCOMMDLGAPI BOOL WINAPI GetOpenFileNameA(LPOPENFILENAMEA ofn) {
    return comdlg_open_file_dialog(0, 0, ofn);
}

WINCOMMDLGAPI BOOL WINAPI GetOpenFileNameW(LPOPENFILENAMEW ofn) {
    return comdlg_open_file_dialog(0, 1, ofn);
}

WINCOMMDLGAPI BOOL WINAPI GetSaveFileNameA(LPOPENFILENAMEA ofn) {
    return comdlg_open_file_dialog(1, 0, ofn);
}

WINCOMMDLGAPI BOOL WINAPI GetSaveFileNameW(LPOPENFILENAMEW ofn) {
    return comdlg_open_file_dialog(1, 1, ofn);
}

DLL_FUNC_EXPORT(ChooseColorW, BOOL, LPCHOOSECOLORW){
}

DLL_FUNC_EXPORT(PrintDlgW, BOOL, LPPRINTDLGW){
}

DLL_FUNC_EXPORT(PageSetupDlgW, BOOL, LPPAGESETUPDLGW){
}

DLL_FUNC_EXPORT(ChooseFontW, BOOL, LPCHOOSEFONTW){
}

DLL_FUNC_EXPORT(FindTextW, HWND, LPFINDREPLACEW){
}

DLL_FUNC_EXPORT(ReplaceTextW, HWND, LPFINDREPLACEW){
}

DLL_FUNC_EXPORT(GetFileTitleW, short, LPCWSTR, LPWSTR, WORD){
}

DLL_FUNC_EXPORT(CommDlgExtendedError, DWORD, void){
}

DLL_FUNC_EXPORT(ChooseColorA, BOOL, LPCHOOSECOLORA){
}

DLL_FUNC_EXPORT(PrintDlgA, BOOL, LPPRINTDLGA){
}

DLL_FUNC_EXPORT(PageSetupDlgA, BOOL, LPPAGESETUPDLGA){
}

DLL_FUNC_EXPORT(ChooseFontA, BOOL, LPCHOOSEFONTA){
}

DLL_FUNC_EXPORT(FindTextA, HWND, LPFINDREPLACEA){
}

DLL_FUNC_EXPORT(ReplaceTextA, HWND, LPFINDREPLACEA){
}

DLL_FUNC_EXPORT(GetFileTitleA, short, LPCSTR, LPSTR, WORD){

}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    if (!p_wine_get_dos_file_name) {
        HMODULE h = GetModuleHandleA("kernel32.dll");
        if (!h) h = LoadLibraryA("kernel32.dll");
        if (h) {
            p_wine_get_dos_file_name = 
                (PFN_wine_get_dos_file_name)GetProcAddress(h, "wine_get_dos_file_name");
            p_wine_get_unix_file_name =
                (PFN_wine_get_unix_file_name)GetProcAddress(h, "wine_get_unix_file_name");
        }
    }
    return TRUE;
}
