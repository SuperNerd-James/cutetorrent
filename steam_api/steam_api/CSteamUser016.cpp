#include "stdafx.h"
#include "CSteamUser016.h"

HSteamUser CSteamUser016::GetHSteamUser()
{
	HSteamUser user=0x13;
	return user;
}

bool CSteamUser016::BLoggedOn()
{
	return true;
}

CSteamID CSteamUser016::GetSteamID()
{
	return CSteamID(0000015,k_EUniversePublic,k_EAccountTypeMax);
}

int CSteamUser016::InitiateGameConnection( void *pAuthBlob, int cbMaxAuthBlob, CSteamID steamIDGameServer, uint32 unIPServer, uint16 usPortServer, bool bSecure )
{
	return 1;
}

void CSteamUser016::TerminateGameConnection( uint32 unIPServer, uint16 usPortServer )
{

}

void CSteamUser016::TrackAppUsageEvent( CGameID gameID, EAppUsageEvent eAppUsageEvent, const char *pchExtraInfo /*= "" */ )
{

}

bool CSteamUser016::GetUserDataFolder( char *pchBuffer, int cubBuffer )
{
	return true;
}

void CSteamUser016::StartVoiceRecording()
{

}

void CSteamUser016::StopVoiceRecording()
{

}

EVoiceResult CSteamUser016::GetAvailableVoice( uint32 *pcbCompressed, uint32 *pcbUncompressed, uint32 nUncompressedVoiceDesiredSampleRate )
{
	return k_EVoiceResultOK;
}

EVoiceResult CSteamUser016::GetVoice( bool bWantCompressed, void *pDestBuffer, uint32 cbDestBufferSize, uint32 *nBytesWritten, bool bWantUncompressed, void *pUncompressedDestBuffer, uint32 cbUncompressedDestBufferSize, uint32 *nUncompressBytesWritten, uint32 nUncompressedVoiceDesiredSampleRate )
{
	return k_EVoiceResultOK;
}

EVoiceResult CSteamUser016::DecompressVoice( const void *pCompressed, uint32 cbCompressed, void *pDestBuffer, uint32 cbDestBufferSize, uint32 *nBytesWritten, uint32 nDesiredSampleRate )
{
	return k_EVoiceResultOK;
}

uint32 CSteamUser016::GetVoiceOptimalSampleRate()
{
	return 16;
}

HAuthTicket CSteamUser016::GetAuthSessionTicket( void *pTicket, int cbMaxTicket, uint32 *pcbTicket )
{
	HAuthTicket ticket=12;
	
	return ticket;
}

EBeginAuthSessionResult CSteamUser016::BeginAuthSession( const void *pAuthTicket, int cbAuthTicket, CSteamID steamID )
{
	return k_EBeginAuthSessionResultOK;
}

void CSteamUser016::EndAuthSession( CSteamID steamID )
{

}

void CSteamUser016::CancelAuthTicket( HAuthTicket hAuthTicket )
{

}

EUserHasLicenseForAppResult CSteamUser016::UserHasLicenseForApp( CSteamID steamID, AppId_t appID )
{
	return k_EUserHasLicenseResultHasLicense;
}

bool CSteamUser016::BIsBehindNAT()
{
	return false;
}

void CSteamUser016::AdvertiseGame( CSteamID steamIDGameServer, uint32 unIPServer, uint16 usPortServer )
{

}

SteamAPICall_t CSteamUser016::RequestEncryptedAppTicket( void *pDataToInclude, int cbDataToInclude )
{
	SteamAPICall_t call=3333;
	return call;
}

bool CSteamUser016::GetEncryptedAppTicket( void *pTicket, int cbMaxTicket, uint32 *pcbTicket )
{
	return true;
}

