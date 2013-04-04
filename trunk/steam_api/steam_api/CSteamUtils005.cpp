#include "StdAfx.h"
#include "CSteamUtils005.h"

uint32 CSteamUtils005::GetSecondsSinceComputerActive()
{
	return 0;
}

EUniverse CSteamUtils005::GetConnectedUniverse()
{
	return k_EUniversePublic;
}

uint32 CSteamUtils005::GetServerRealTime()
{
	return 0;
}

const char * CSteamUtils005::GetIPCountry()
{
	return "RU";
}

bool CSteamUtils005::GetImageSize( int iImage, uint32 *pnWidth, uint32 *pnHeight )
{
	return false;
}

bool CSteamUtils005::GetImageRGBA( int iImage, uint8 *pubDest, int nDestBufferSize )
{
	return false;
}

bool CSteamUtils005::GetCSERIPPort( uint32 *unIP, uint16 *usPort )
{
	return false;
}

uint8 CSteamUtils005::GetCurrentBatteryPower()
{
	return 100;
}

uint32 CSteamUtils005::GetAppID()
{
	return 202990; // 202990
}

void CSteamUtils005::SetOverlayNotificationPosition( ENotificationPosition eNotificationPosition )
{

}

bool CSteamUtils005::IsAPICallCompleted( SteamAPICall_t hSteamAPICall, bool *pbFailed )
{
	return false;
}

ESteamAPICallFailure CSteamUtils005::GetAPICallFailureReason( SteamAPICall_t hSteamAPICall )
{
	return k_ESteamAPICallFailureNone;
}

bool CSteamUtils005::GetAPICallResult( SteamAPICall_t hSteamAPICall, void *pCallback, int cubCallback, int iCallbackExpected, bool *pbFailed )
{
	return false;
}

void CSteamUtils005::RunFrame()
{

}

uint32 CSteamUtils005::GetIPCCallCount()
{
	return 0;
}

void CSteamUtils005::SetWarningMessageHook( SteamAPIWarningMessageHook_t pFunction )
{
	
}

bool CSteamUtils005::IsOverlayEnabled()
{
	return false;
}

bool CSteamUtils005::BOverlayNeedsPresent()
{
	return false;
}

uint32 CSteamUtils005::GetSecondsSinceAppActive()
{
	return 0;
}

SteamAPICall_t CSteamUtils005::CheckFileSignature( const char *szFileName )
{
	
	SteamAPICall_t result = CSteamBase::RegisterCall();
	CheckFileSignature_t* retvals = (CheckFileSignature_t*)malloc(sizeof(CheckFileSignature_t));
	retvals->m_eCheckFileSignature = k_ECheckFileSignatureValidSignature;

	CSteamBase::ReturnCall(retvals, sizeof(LobbyMatchList_t), LobbyMatchList_t::k_iCallback, result);

	return result;
}

bool CSteamUtils005::ShowGamepadTextInput( EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eInputLineMode, const char *szText, uint32 uMaxLength )
{
	return false;;
}

uint32 CSteamUtils005::GetEnteredGamepadTextLength()
{
	return 0;
}

bool CSteamUtils005::GetEnteredGamepadTextInput( char *pchValue, uint32 cchValueMax )
{
	return false;
}
