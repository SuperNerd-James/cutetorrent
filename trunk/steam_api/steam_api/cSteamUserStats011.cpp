#include "StdAfx.h"

#include "CSteamUserStats011.h"


bool CSteamUserStats011::RequestCurrentStats()
{
	Logger( "steam_emu", "CSteamUserStats011::RequestCurrentStats()" );
	return true;
}

bool CSteamUserStats011::GetStat( const char *pchName, float *pData )
{
	Logger( "steam_emu", "CSteamUserStats011::GetStat(%s,%f)",pchName,*pData );
	return false;
}

bool CSteamUserStats011::GetStat( const char *pchName, int32 *pData )
{
	Logger( "steam_emu", "CSteamUserStats011::GetStat(%s,%d)",pchName,*pData );
	return false;
}

bool CSteamUserStats011::SetStat( const char *pchName, float fData )
{
	Logger( "steam_emu", "CSteamUserStats011::GetStat(%s,%f)",pchName,fData );
	return false;
}

bool CSteamUserStats011::SetStat( const char *pchName, int32 nData )
{
	Logger( "steam_emu", "CSteamUserStats011::GetStat(%s,%d)",pchName,nData );
	return false;
}

bool CSteamUserStats011::UpdateAvgRateStat( const char *pchName, float flCountThisSession, double dSessionLength )
{
	return false;
}

bool CSteamUserStats011::GetAchievement( const char *pchName, bool *pbAchieved )
{
	return false;
}

bool CSteamUserStats011::SetAchievement( const char *pchName )
{
	return false;
}

bool CSteamUserStats011::ClearAchievement( const char *pchName )
{
	return false;
}

bool CSteamUserStats011::GetAchievementAndUnlockTime( const char *pchName, bool *pbAchieved, uint32 *punUnlockTime )
{
	return false;
}

bool CSteamUserStats011::StoreStats()
{
	return false;
}

int CSteamUserStats011::GetAchievementIcon( const char *pchName )
{
	return 0;
}

const char * CSteamUserStats011::GetAchievementDisplayAttribute( const char *pchName, const char *pchKey )
{
	return "";
}

bool CSteamUserStats011::IndicateAchievementProgress( const char *pchName, uint32 nCurProgress, uint32 nMaxProgress )
{
	return false;
}

uint32 CSteamUserStats011::GetNumAchievements( CGameID nGameID )
{
	return 0;
}

const char * CSteamUserStats011::GetAchievementName( uint32 iAchievement )
{
	return "";
}

SteamAPICall_t CSteamUserStats011::RequestUserStats( CSteamID steamIDUser )
{
	
	SteamAPICall_t result = CSteamBase::RegisterCall();
	UserStatsReceived_t* retvals = (UserStatsReceived_t*)malloc(sizeof(UserStatsReceived_t));
	retvals->m_eResult = k_EResultFail;
	
	CSteamBase::ReturnCall(retvals, sizeof(UserStatsReceived_t), LobbyMatchList_t::k_iCallback, result);

	return result;
}

bool CSteamUserStats011::GetUserStat( CSteamID steamIDUser, const char *pchName, float *pData )
{
	return false;
}

bool CSteamUserStats011::GetUserStat( CSteamID steamIDUser, const char *pchName, int32 *pData )
{
	return false;
}

bool CSteamUserStats011::GetUserAchievement( CSteamID steamIDUser, const char *pchName, bool *pbAchieved )
{
	return false;
}

bool CSteamUserStats011::GetUserAchievementAndUnlockTime( CSteamID steamIDUser, const char *pchName, bool *pbAchieved, uint32 *punUnlockTime )
{
	return false;
}

bool CSteamUserStats011::ResetAllStats( bool bAchievementsToo )
{
	return false;
}

SteamAPICall_t CSteamUserStats011::FindOrCreateLeaderboard( const char *pchLeaderboardName, ELeaderboardSortMethod eLeaderboardSortMethod, ELeaderboardDisplayType eLeaderboardDisplayType )
{
	SteamAPICall_t result = CSteamBase::RegisterCall();
	LeaderboardFindResult_t* retvals = (LeaderboardFindResult_t*)malloc(sizeof(LeaderboardFindResult_t));
	retvals->m_bLeaderboardFound = 0;

	CSteamBase::ReturnCall(retvals, sizeof(LeaderboardFindResult_t), LobbyMatchList_t::k_iCallback, result);

	return result;

}

SteamAPICall_t CSteamUserStats011::FindLeaderboard( const char *pchLeaderboardName )
{
	SteamAPICall_t result = CSteamBase::RegisterCall();
	LeaderboardFindResult_t* retvals = (LeaderboardFindResult_t*)malloc(sizeof(LeaderboardFindResult_t));
	retvals->m_bLeaderboardFound = 0;

	CSteamBase::ReturnCall(retvals, sizeof(LeaderboardFindResult_t), LobbyMatchList_t::k_iCallback, result);

	return result;
}

const char * CSteamUserStats011::GetLeaderboardName( SteamLeaderboard_t hSteamLeaderboard )
{
	return "";
}

int CSteamUserStats011::GetLeaderboardEntryCount( SteamLeaderboard_t hSteamLeaderboard )
{
	return 0;
}

ELeaderboardSortMethod CSteamUserStats011::GetLeaderboardSortMethod( SteamLeaderboard_t hSteamLeaderboard )
{
	return k_ELeaderboardSortMethodNone;
}

ELeaderboardDisplayType CSteamUserStats011::GetLeaderboardDisplayType( SteamLeaderboard_t hSteamLeaderboard )
{
	return k_ELeaderboardDisplayTypeNone;
}

SteamAPICall_t CSteamUserStats011::DownloadLeaderboardEntries( SteamLeaderboard_t hSteamLeaderboard, ELeaderboardDataRequest eLeaderboardDataRequest, int nRangeStart, int nRangeEnd )
{
	
	SteamAPICall_t result = CSteamBase::RegisterCall();
	LeaderboardScoresDownloaded_t* retvals = (LeaderboardScoresDownloaded_t*)malloc(sizeof(LeaderboardScoresDownloaded_t));
	retvals->m_cEntryCount = 0;

	CSteamBase::ReturnCall(retvals, sizeof(LeaderboardScoresDownloaded_t), LobbyMatchList_t::k_iCallback, result);

	return result;
}

SteamAPICall_t CSteamUserStats011::DownloadLeaderboardEntriesForUsers( SteamLeaderboard_t hSteamLeaderboard, CSteamID *prgUsers, int cUsers )
{
	SteamAPICall_t result = CSteamBase::RegisterCall();
	LeaderboardScoresDownloaded_t* retvals = (LeaderboardScoresDownloaded_t*)malloc(sizeof(LeaderboardScoresDownloaded_t));
	retvals->m_cEntryCount = 0;

	CSteamBase::ReturnCall(retvals, sizeof(LeaderboardScoresDownloaded_t), LobbyMatchList_t::k_iCallback, result);

	return result;
}

bool CSteamUserStats011::GetDownloadedLeaderboardEntry( SteamLeaderboardEntries_t hSteamLeaderboardEntries, int index, LeaderboardEntry_t *pLeaderboardEntry, int32 pDetails[], int cDetailsMax )
{
	return false;
}

SteamAPICall_t CSteamUserStats011::UploadLeaderboardScore( SteamLeaderboard_t hSteamLeaderboard, ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod, int32 nScore, const int32 *pScoreDetails, int cScoreDetailsCount )
{
	
	SteamAPICall_t result = CSteamBase::RegisterCall();
	LeaderboardScoreUploaded_t* retvals = (LeaderboardScoreUploaded_t*)malloc(sizeof(LeaderboardScoreUploaded_t));
	retvals->m_bSuccess = 0;

	CSteamBase::ReturnCall(retvals, sizeof(LeaderboardScoreUploaded_t), LobbyMatchList_t::k_iCallback, result);

	return result;
}

SteamAPICall_t CSteamUserStats011::AttachLeaderboardUGC( SteamLeaderboard_t hSteamLeaderboard, UGCHandle_t hUGC )
{
	
	SteamAPICall_t result = CSteamBase::RegisterCall();
	LeaderboardUGCSet_t* retvals = (LeaderboardUGCSet_t*)malloc(sizeof(LeaderboardUGCSet_t));
	retvals->m_eResult = k_EResultFail;

	CSteamBase::ReturnCall(retvals, sizeof(LeaderboardUGCSet_t), LobbyMatchList_t::k_iCallback, result);

	return result;
}

SteamAPICall_t CSteamUserStats011::GetNumberOfCurrentPlayers()
{
	
	SteamAPICall_t result = CSteamBase::RegisterCall();
	NumberOfCurrentPlayers_t* retvals = (NumberOfCurrentPlayers_t*)malloc(sizeof(NumberOfCurrentPlayers_t));
	retvals->m_bSuccess = k_EResultOK;
	retvals->m_cPlayers = 13;
	CSteamBase::ReturnCall(retvals, sizeof(NumberOfCurrentPlayers_t), LobbyMatchList_t::k_iCallback, result);

	return result;
}

SteamAPICall_t CSteamUserStats011::RequestGlobalAchievementPercentages()
{
	
	SteamAPICall_t result = CSteamBase::RegisterCall();
	GlobalAchievementPercentagesReady_t* retvals = (GlobalAchievementPercentagesReady_t*)malloc(sizeof(GlobalAchievementPercentagesReady_t));
	retvals->m_eResult = k_EResultFail;
	
	CSteamBase::ReturnCall(retvals, sizeof(GlobalAchievementPercentagesReady_t), LobbyMatchList_t::k_iCallback, result);

	return result;
}

int CSteamUserStats011::GetMostAchievedAchievementInfo( char *pchName, uint32 unNameBufLen, float *pflPercent, bool *pbAchieved )
{
	return 0;
}

int CSteamUserStats011::GetNextMostAchievedAchievementInfo( int iIteratorPrevious, char *pchName, uint32 unNameBufLen, float *pflPercent, bool *pbAchieved )
{
	return 0;
}

bool CSteamUserStats011::GetAchievementAchievedPercent( const char *pchName, float *pflPercent )
{
	return false;
}

SteamAPICall_t CSteamUserStats011::RequestGlobalStats( int nHistoryDays )
{
	
	SteamAPICall_t result = CSteamBase::RegisterCall();
	GlobalStatsReceived_t* retvals = (GlobalStatsReceived_t*)malloc(sizeof(GlobalStatsReceived_t));
	retvals->m_eResult = k_EResultFail;

	CSteamBase::ReturnCall(retvals, sizeof(GlobalStatsReceived_t), LobbyMatchList_t::k_iCallback, result);

	return result;
}

bool CSteamUserStats011::GetGlobalStat( const char *pchStatName, double *pData )
{
	return false;
}

bool CSteamUserStats011::GetGlobalStat( const char *pchStatName, int64 *pData )
{
	return false;
}

int32 CSteamUserStats011::GetGlobalStatHistory( const char *pchStatName, double *pData, uint32 cubData )
{
	return 0;
}

int32 CSteamUserStats011::GetGlobalStatHistory( const char *pchStatName, int64 *pData, uint32 cubData )
{
	return 0;
}
