// steam_api.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"



extern "C"
{
  __declspec(dllexport) HSteamPipe __cdecl GetHSteamPipe()
    {
        Logger( "steam_emu", "GetHSteamPipe" );
 
        return NULL;
    }
    __declspec(dllexport) HSteamUser __cdecl GetHSteamUser()
    {
        Logger( "steam_emu", "GetHSteamUser" );
 
        return NULL;
    }
    __declspec(dllexport) HSteamPipe __cdecl SteamAPI_GetHSteamPipe()
    {
        Logger( "steam_emu", "SteamAPI_GetHSteamPipe" );
 
        return NULL;
    }
    __declspec(dllexport) HSteamUser __cdecl SteamAPI_GetHSteamUser()
    {
        Logger( "steam_emu", "SteamAPI_GetHSteamUser" );
 
        return NULL;
    }
    __declspec(dllexport) const char *__cdecl SteamAPI_GetSteamInstallPath()
    {
        Logger( "steam_emu", "SteamAPI_GetSteamInstallPath" );
 
        return NULL;
    }

	__declspec(dllexport) bool __cdecl SteamAPI_Init()
    {
        Logger( "steam_emu", "SteamAPI_Init" );

		PBYTE offset = (PBYTE)GetProcAddress(GetModuleHandleA("ws2_32.dll"),"gethostbyname");
//		realgethostbyname = (gethostbyname_t)DetourFunction(offset, (PBYTE)&custom_gethostbyname);


        return true;
    }

    __declspec(dllexport) bool __cdecl SteamAPI_InitSafe()
    {
        Logger( "steam_emu", "SteamAPI_InitSafe" );
 
        return true;
    }

	__declspec(dllexport) char __cdecl SteamAPI_RestartApp()
    {
        Logger( "steam_emu", "SteamAPI_RestartApp" );
 
        return 1;
    }

	__declspec(dllexport) char __cdecl SteamAPI_RestartAppIfNecessary()
    {
        Logger( "steam_emu", "SteamAPI_RestartAppIfNecessary" );
 
        return 0;
    }
    
	__declspec(dllexport) void __cdecl SteamAPI_RegisterCallResult( CCallbackBase* pResult, SteamAPICall_t APICall )
    {
        Logger( "steam_emu", "SteamAPI_RegisterCallResult for call %d", APICall );
 
		CSteamBase::RegisterCallResult(APICall,pResult);
        //
    }

    __declspec(dllexport) void __cdecl SteamAPI_RegisterCallback( CCallbackBase *pCallback, int iCallback )
    {
        Logger( "steam_emu", "SteamAPI_RegisterCallback for call type %d", iCallback );

		CSteamBase::RegisterCallback(pCallback, iCallback);
        //
    }
    __declspec(dllexport) void __cdecl SteamAPI_RunCallbacks()
    {
        //Logger( "steam_emu", "SteamAPI_RunCallbacks" );
 
		CSteamBase::RunCallbacks();
        //
    }
    __declspec(dllexport) void __cdecl SteamAPI_SetMiniDumpComment( const char *pchMsg )
    {
        Logger( "steam_emu", "SteamAPI_SetMiniDumpComment" );
 
        //
    }
    __declspec(dllexport) bool __cdecl SteamAPI_SetTryCatchCallbacks( bool bUnknown )
    {
        Logger( "steam_emu", "SteamAPI_SetTryCatchCallbacks" );
 
        return bUnknown;
    }
    __declspec(dllexport) void __cdecl SteamAPI_Shutdown()
    {
        Logger( "steam_emu", "SteamAPI_Shutdown" );
 
        //
    }
    __declspec(dllexport) void __cdecl SteamAPI_UnregisterCallResult( CCallbackBase* pResult, SteamAPICall_t APICall )
    {
        Logger( "steam_emu", "SteamAPI_UnregisterCallResult" );
 
        //
    }
    __declspec(dllexport) void __cdecl SteamAPI_UnregisterCallback( CCallbackBase *pCallback, int iCallback )
    {
        Logger( "steam_emu", "SteamAPI_UnregisterCallback" );
 
		
        //
    }
    __declspec(dllexport) void __cdecl SteamAPI_WriteMiniDump( uint32 uStructuredExceptionCode, void* pvExceptionInfo, uint32 uBuildID )
    {
        Logger( "steam_emu", "SteamAPI_WriteMiniDump" );
 
        //
    }
    __declspec(dllexport) ISteamApps* __cdecl SteamApps()
    {
        Logger( "steam_emu", "SteamApps" );
 
        return NULL;
    }
    __declspec(dllexport) ISteamClient* __cdecl SteamClient()
    {
        Logger( "steam_emu", "SteamClient" );
 
        return NULL;
    }
    __declspec(dllexport) ISteamContentServer* __cdecl SteamContentServer()
    {
        Logger( "steam_emu", "SteamContentServer" );
 
        return NULL;
    }
    __declspec(dllexport) ISteamUtils* __cdecl SteamContentServerUtils()
    {
        Logger( "steam_emu", "SteamContentServerUtils" );
 
        return NULL;
    }
    __declspec(dllexport) bool __cdecl SteamContentServer_Init( unsigned int uLocalIP, unsigned short usPort )
    {
        Logger( "steam_emu", "SteamContentServer_Init" );
 
        return NULL;
    }
    __declspec(dllexport) void __cdecl SteamContentServer_RunCallbacks()
    {
        Logger( "steam_emu", "SteamContentServer_RunCallbacks" );
 
        //
    }
    __declspec(dllexport) void __cdecl SteamContentServer_Shutdown()
    {
        Logger( "steam_emu", "SteamContentServer_Shutdown" );
 
        //
    }
    __declspec(dllexport) ISteamFriends* __cdecl SteamFriends()
    {
        //Logger( "steam_emu", "SteamFriends" );
 
		return (ISteamFriends*)CSteamBase::GetInterface(INTERFACE_STEAMFRIENDS013);
    }
    __declspec(dllexport) ISteamGameServer* __cdecl SteamGameServer()
    {
        Logger( "steam_emu", "SteamGameServer" );
 
        return NULL;
    }
    __declspec(dllexport) ISteamUtils* __cdecl SteamGameServerUtils()
    {
        Logger( "steam_emu", "SteamGameServerUtils" );
 
        return NULL;
    }
    __declspec(dllexport) bool __cdecl SteamGameServer_BSecure()
    {
        Logger( "steam_emu", "SteamGameServer_BSecure" );
 
        return true;
    }
    __declspec(dllexport) HSteamPipe __cdecl SteamGameServer_GetHSteamPipe()
    {
        Logger( "steam_emu", "SteamGameServer_GetHSteamPipe" );
 
        return NULL;
    }
    __declspec(dllexport) HSteamUser __cdecl SteamGameServer_GetHSteamUser()
    {
        Logger( "steam_emu", "SteamGameServer_GetHSteamUser" );
 
        return NULL;
    }
    __declspec(dllexport) int32 __cdecl SteamGameServer_GetIPCCallCount()
    {
        Logger( "steam_emu", "SteamGameServer_GetIPCCallCount" );
 
        return NULL;
    }
    __declspec(dllexport) uint64 __cdecl SteamGameServer_GetSteamID()
    {
        Logger( "steam_emu", "SteamGameServer_GetSteamID" );
 
        return NULL;
    }
    __declspec(dllexport) bool __cdecl SteamGameServer_Init( uint32 unIP, uint16 usPort, uint16 usGamePort, EServerMode eServerMode, int nGameAppId, const char *pchGameDir, const char *pchVersionString )
    {
        Logger( "steam_emu", "SteamGameServer_Init" );
 
        return true;
    }
    __declspec(dllexport) bool __cdecl SteamGameServer_InitSafe( uint32 unIP, uint16 usPort, uint16 usGamePort, EServerMode eServerMode, int nGameAppId, const char *pchGameDir, const char *pchVersionString, unsigned long dongs )
    {
        Logger( "steam_emu", "SteamGameServer_InitSafe" );
 
        return true;
    }
    __declspec(dllexport) void __cdecl SteamGameServer_RunCallbacks()
    {
        //Logger( "steam_emu", "SteamGameServer_RunCallbacks" );
 
        //
    }
    __declspec(dllexport) void __cdecl SteamGameServer_Shutdown()
    {
        Logger( "steam_emu", "SteamGameServer_Shutdown" );
 
        //
    }
    __declspec(dllexport) ISteamMasterServerUpdater* __cdecl SteamMasterServerUpdater()
    {
        Logger( "steam_emu", "SteamMasterServerUpdater" );
 
        return NULL;
    }
    __declspec(dllexport) ISteamMatchmaking* __cdecl SteamMatchmaking()
    {
        //Logger( "steam_emu", "SteamMatchmaking" );
		return (ISteamMatchmaking*)CSteamBase::GetInterface(INTERFACE_STEAMMATCHMAKING009);
    }
    __declspec(dllexport) ISteamMatchmakingServers* __cdecl SteamMatchmakingServers()
    {
        Logger( "steam_emu", "SteamMatchmakingServers" );
 
        return NULL;
    }
    __declspec(dllexport) ISteamNetworking* __cdecl SteamNetworking()
    {
        //Logger( "steam_emu", "SteamNetworking" );
 
        return NULL;
    }
    __declspec(dllexport) void* __cdecl SteamRemoteStorage()
    {
        //Logger( "steam_emu", "SteamRemoteStorage" );
 
        return NULL;
    }
    __declspec(dllexport) ISteamUser* __cdecl SteamUser()
    {
        //Logger( "steam_emu", "SteamUser" );
 
        return (ISteamUser*)CSteamBase::GetInterface( INTERFACE_STEAMUSER016);
    }
    __declspec(dllexport) ISteamUserStats* __cdecl SteamUserStats()
    {
        Logger( "steam_emu", "SteamUserStats" );
 
		return (ISteamUserStats*)CSteamBase::GetInterface(INTERFACE_STEAMUSERSTATS11);
    }
    __declspec(dllexport) ISteamUtils* __cdecl SteamUtils()
    {
        Logger( "steam_emu", "SteamUtils" );
 
        return (ISteamUtils*)CSteamBase::GetInterface(INTERFACE_STEAMUTILS005);
    }
    __declspec(dllexport) HSteamUser __cdecl Steam_GetHSteamUserCurrent()
    {
        Logger( "steam_emu", "Steam_GetHSteamUserCurrent" );
 
        return NULL;
    }
    __declspec(dllexport) void __cdecl Steam_RegisterInterfaceFuncs( void *hModule )
    {
        Logger( "steam_emu", "Steam_RegisterInterfaceFuncs" );
 
        //
    }
    __declspec(dllexport) void __cdecl Steam_RunCallbacks( HSteamPipe hSteamPipe, bool bGameServerCallbacks )
    {
        Logger( "steam_emu", "Steam_RunCallbacks" );
 
        //
    }
    __declspec(dllexport) void *g_pSteamClientGameServer = NULL;
}
