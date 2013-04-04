#include "StdAfx.h"
#include "CSteamFriends013.h"



const char * CSteamFriends013::GetPersonaName()
{
	return "USErNOOB";
}

SteamAPICall_t CSteamFriends013::SetPersonaName( const char *pchPersonaName )
{
	SteamAPICall_t call=0x13;
	return call;
}

EPersonaState CSteamFriends013::GetPersonaState()
{
	return k_EPersonaStateOnline;
}

int CSteamFriends013::GetFriendCount( int iFriendFlags )
{
	return 1;
}

EFriendRelationship CSteamFriends013::GetFriendRelationship( CSteamID steamIDFriend )
{
	return k_EFriendRelationshipFriend;
}

EPersonaState CSteamFriends013::GetFriendPersonaState( CSteamID steamIDFriend )
{
	return k_EPersonaStateOnline;
}

const char * CSteamFriends013::GetFriendPersonaName( CSteamID steamIDFriend )
{
	return "friendName";
}

bool CSteamFriends013::GetFriendGamePlayed( CSteamID steamIDFriend, FriendGameInfo_t *pFriendGameInfo )
{
	return false;
}

const char * CSteamFriends013::GetFriendPersonaNameHistory( CSteamID steamIDFriend, int iPersonaName )
{
	return "GetFriendPersonaNameHistory";
}

bool CSteamFriends013::HasFriend( CSteamID steamIDFriend, int iFriendFlags )
{
	return true;
}

int CSteamFriends013::GetClanCount()
{
	return 1;
}

const char * CSteamFriends013::GetClanTag( CSteamID steamIDClan )
{
	return "[CLAN]";
}

bool CSteamFriends013::GetClanActivityCounts( CSteamID steamID, int *pnOnline, int *pnInGame, int *pnChatting )
{
	return false;
}

const char * CSteamFriends013::GetClanName( CSteamID iClan ) 
{
	return "ClanName";
}																  

SteamAPICall_t CSteamFriends013::DownloadClanActivityCounts( CSteamID groupIDs[], int nIds )
{
	SteamAPICall_t call=0x13;
	return call;
}

int CSteamFriends013::GetFriendCountFromSource( CSteamID steamIDSource )
{
	return 1;
}

bool CSteamFriends013::IsUserInSource( CSteamID steamIDUser, CSteamID steamIDSource )
{
	return true;
}

void CSteamFriends013::SetInGameVoiceSpeaking( CSteamID steamIDUser, bool bSpeaking )
{

}

void CSteamFriends013::ActivateGameOverlay( const char *pchDialog )
{

}

void CSteamFriends013::ActivateGameOverlayToUser( const char *pchDialog, CSteamID steamID )
{

}

void CSteamFriends013::ActivateGameOverlayToWebPage( const char *pchURL )
{

}

void CSteamFriends013::ActivateGameOverlayToStore( AppId_t nAppID, EOverlayToStoreFlag eFlag )
{

}

void CSteamFriends013::SetPlayedWith( CSteamID steamIDUserPlayedWith )
{

}

void CSteamFriends013::ActivateGameOverlayInviteDialog( CSteamID steamIDLobby )
{

}

int CSteamFriends013::GetSmallFriendAvatar( CSteamID steamIDFriend )
{
	return 0;
}

int CSteamFriends013::GetMediumFriendAvatar( CSteamID steamIDFriend )
{
	return 0;
}

int CSteamFriends013::GetLargeFriendAvatar( CSteamID steamIDFriend )
{
	return 0;
}

bool CSteamFriends013::RequestUserInformation( CSteamID steamIDUser, bool bRequireNameOnly )
{
	return false;
}

SteamAPICall_t CSteamFriends013::RequestClanOfficerList( CSteamID steamIDClan )
{
	return 0;
}

int CSteamFriends013::GetClanOfficerCount( CSteamID steamIDClan )
{
	return 0;
}

EUserRestriction CSteamFriends013::GetUserRestrictions()
{
	return k_nUserRestrictionNone;
}

bool CSteamFriends013::SetRichPresence( const char *pchKey, const char *pchValue )
{
	return false;
}

void CSteamFriends013::ClearRichPresence()
{

}

const char * CSteamFriends013::GetFriendRichPresence( CSteamID steamIDFriend, const char *pchKey )
{
	return "";
}

int CSteamFriends013::GetFriendRichPresenceKeyCount( CSteamID steamIDFriend )
{
	return 0;
}

void CSteamFriends013::RequestFriendRichPresence( CSteamID steamIDFriend )
{

}

bool CSteamFriends013::InviteUserToGame( CSteamID steamIDFriend, const char *pchConnectString )
{
	return false;
}

int CSteamFriends013::GetCoplayFriendCount()
{
	return 0;
}

AppId_t CSteamFriends013::GetFriendCoplayGame( CSteamID steamIDFriend )
{
	return k_nGameIDTF2;
	
}

int CSteamFriends013::GetFriendCoplayTime( CSteamID steamIDFriend )
{
return 0;

}

SteamAPICall_t CSteamFriends013::JoinClanChatRoom( CSteamID steamIDClan )
{
	return 0;
}

bool CSteamFriends013::LeaveClanChatRoom( CSteamID steamIDClan )
{
return 0;
}

int CSteamFriends013::GetClanChatMemberCount( CSteamID steamIDClan )
{
return 1;
}

bool CSteamFriends013::SendClanChatMessage( CSteamID steamIDClanChat, const char *pchText )
{
return true;
}

int CSteamFriends013::GetClanChatMessage( CSteamID steamIDClanChat, int iMessage, void *prgchText, int cchTextMax, EChatEntryType *peChatEntryType, CSteamID *pSteamIDChatter )
{
	return 0;
}

bool CSteamFriends013::IsClanChatAdmin( CSteamID steamIDClanChat, CSteamID steamIDUser )
{
	return true;
}

bool CSteamFriends013::IsClanChatWindowOpenInSteam( CSteamID steamIDClanChat )
{
return false;
}

bool CSteamFriends013::OpenClanChatWindowInSteam( CSteamID steamIDClanChat )
{
return false;
}

bool CSteamFriends013::CloseClanChatWindowInSteam( CSteamID steamIDClanChat )
{
return false;
}

bool CSteamFriends013::SetListenForFriendsMessages( bool bInterceptEnabled )
{
return false;
}

bool CSteamFriends013::ReplyToFriendMessage( CSteamID steamIDFriend, const char *pchMsgToSend )
{
return false;
}

int CSteamFriends013::GetFriendMessage( CSteamID steamIDFriend, int iMessageID, void *pvData, int cubData, EChatEntryType *peChatEntryType )
{
return 0;
}

SteamAPICall_t CSteamFriends013::GetFollowerCount( CSteamID steamID )
{
return 0;
}

SteamAPICall_t CSteamFriends013::IsFollowing( CSteamID steamID )
{
return 0;
}

SteamAPICall_t CSteamFriends013::EnumerateFollowingList( uint32 uStartIndex )
{
return 0;
}
