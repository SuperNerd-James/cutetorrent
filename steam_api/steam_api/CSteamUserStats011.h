#pragma once
#include "RemoteStorageCommon.h"
#include "ISteamUserStats011.h"
class CSteamUserStats011 : ISteamUserStats011
{
public:
	

	virtual bool RequestCurrentStats();

	virtual bool GetStat( const char *pchName, float *pData );

	virtual bool GetStat( const char *pchName, int32 *pData );

	virtual bool SetStat( const char *pchName, float fData );

	virtual bool SetStat( const char *pchName, int32 nData );

	virtual bool UpdateAvgRateStat( const char *pchName, float flCountThisSession, double dSessionLength );

	virtual bool GetAchievement( const char *pchName, bool *pbAchieved );

	virtual bool SetAchievement( const char *pchName );

	virtual bool ClearAchievement( const char *pchName );

	virtual bool GetAchievementAndUnlockTime( const char *pchName, bool *pbAchieved, uint32 *punUnlockTime );

	virtual bool StoreStats();

	virtual int GetAchievementIcon( const char *pchName );

	virtual const char * GetAchievementDisplayAttribute( const char *pchName, const char *pchKey );

	virtual bool IndicateAchievementProgress( const char *pchName, uint32 nCurProgress, uint32 nMaxProgress );

	virtual uint32 GetNumAchievements( CGameID nGameID );

	virtual const char * GetAchievementName( uint32 iAchievement );

	virtual SteamAPICall_t RequestUserStats( CSteamID steamIDUser );

	virtual bool GetUserStat( CSteamID steamIDUser, const char *pchName, float *pData );

	virtual bool GetUserStat( CSteamID steamIDUser, const char *pchName, int32 *pData );

	virtual bool GetUserAchievement( CSteamID steamIDUser, const char *pchName, bool *pbAchieved );

	virtual bool GetUserAchievementAndUnlockTime( CSteamID steamIDUser, const char *pchName, bool *pbAchieved, uint32 *punUnlockTime );

	virtual bool ResetAllStats( bool bAchievementsToo );

	virtual SteamAPICall_t FindOrCreateLeaderboard( const char *pchLeaderboardName, ELeaderboardSortMethod eLeaderboardSortMethod, ELeaderboardDisplayType eLeaderboardDisplayType );

	virtual SteamAPICall_t FindLeaderboard( const char *pchLeaderboardName );

	virtual const char * GetLeaderboardName( SteamLeaderboard_t hSteamLeaderboard );

	virtual int GetLeaderboardEntryCount( SteamLeaderboard_t hSteamLeaderboard );

	virtual ELeaderboardSortMethod GetLeaderboardSortMethod( SteamLeaderboard_t hSteamLeaderboard );

	virtual ELeaderboardDisplayType GetLeaderboardDisplayType( SteamLeaderboard_t hSteamLeaderboard );

	virtual SteamAPICall_t DownloadLeaderboardEntries( SteamLeaderboard_t hSteamLeaderboard, ELeaderboardDataRequest eLeaderboardDataRequest, int nRangeStart, int nRangeEnd );

	virtual SteamAPICall_t DownloadLeaderboardEntriesForUsers( SteamLeaderboard_t hSteamLeaderboard, CSteamID *prgUsers, int cUsers );

	virtual bool GetDownloadedLeaderboardEntry( SteamLeaderboardEntries_t hSteamLeaderboardEntries, int index, LeaderboardEntry_t *pLeaderboardEntry, int32 pDetails[], int cDetailsMax );

	virtual SteamAPICall_t UploadLeaderboardScore( SteamLeaderboard_t hSteamLeaderboard, ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod, int32 nScore, const int32 *pScoreDetails, int cScoreDetailsCount );

	virtual SteamAPICall_t AttachLeaderboardUGC( SteamLeaderboard_t hSteamLeaderboard, UGCHandle_t hUGC );

	virtual SteamAPICall_t GetNumberOfCurrentPlayers();

	virtual SteamAPICall_t RequestGlobalAchievementPercentages();

	virtual int GetMostAchievedAchievementInfo( char *pchName, uint32 unNameBufLen, float *pflPercent, bool *pbAchieved );

	virtual int GetNextMostAchievedAchievementInfo( int iIteratorPrevious, char *pchName, uint32 unNameBufLen, float *pflPercent, bool *pbAchieved );

	virtual bool GetAchievementAchievedPercent( const char *pchName, float *pflPercent );

	virtual SteamAPICall_t RequestGlobalStats( int nHistoryDays );

	virtual bool GetGlobalStat( const char *pchStatName, double *pData );

	virtual bool GetGlobalStat( const char *pchStatName, int64 *pData );

	virtual int32 GetGlobalStatHistory( const char *pchStatName, double *pData, uint32 cubData );

	virtual int32 GetGlobalStatHistory( const char *pchStatName, int64 *pData, uint32 cubData );

};
