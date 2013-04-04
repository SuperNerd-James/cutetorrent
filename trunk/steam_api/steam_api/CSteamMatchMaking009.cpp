#include "StdAfx.h"
#include "CSteamMatchMaking009.h"

int CSteamMatchMaking009::GetFavoriteGameCount()
{
	return 0;
}

bool CSteamMatchMaking009::GetFavoriteGame( int iGame, AppId_t *pnAppID, uint32 *pnIP, uint16 *pnConnPort, uint16 *pnQueryPort, uint32 *punFlags, uint32 *pRTime32LastPlayedOnServer )
{
	return false;
}

int CSteamMatchMaking009::AddFavoriteGame( AppId_t nAppID, uint32 nIP, uint16 nConnPort, uint16 nQueryPort, uint32 unFlags, uint32 rTime32LastPlayedOnServer )
{
	return 0;
}

bool CSteamMatchMaking009::RemoveFavoriteGame( AppId_t nAppID, uint32 nIP, uint16 nConnPort, uint16 nQueryPort, uint32 unFlags )
{
	return false;
}

SteamAPICall_t CSteamMatchMaking009::RequestLobbyList()
{
	SteamAPICall_t result = CSteamBase::RegisterCall();
	LobbyMatchList_t* retvals = (LobbyMatchList_t*)malloc(sizeof(LobbyMatchList_t));
	retvals->m_nLobbiesMatching = 0;

	CSteamBase::ReturnCall(retvals, sizeof(LobbyMatchList_t), LobbyMatchList_t::k_iCallback, result);

	return result;
}
CSteamID CSteamMatchMaking009::GetLobbyByIndex( int iLobby )
{
	return CSteamID( 1337132, 0x40000, k_EUniversePublic, k_EAccountTypeChat );
}
CSteamID CSteamMatchMaking009::GetLobbyMemberByIndex( CSteamID steamIDLobby, int iMember )
{
	return CSteamID((uint64)0x0015);
}
void CSteamMatchMaking009::AddRequestLobbyListStringFilter( const char *pchKeyToMatch, const char *pchValueToMatch, ELobbyComparison eComparisonType )
{

}

void CSteamMatchMaking009::AddRequestLobbyListNumericalFilter( const char *pchKeyToMatch, int nValueToMatch, ELobbyComparison eComparisonType )
{

}

void CSteamMatchMaking009::AddRequestLobbyListNearValueFilter( const char *pchKeyToMatch, int nValueToBeCloseTo )
{

}

void CSteamMatchMaking009::AddRequestLobbyListFilterSlotsAvailable( int nSlotsAvailable )
{

}

void CSteamMatchMaking009::AddRequestLobbyListResultCountFilter( int cMaxResults )
{

}

void CSteamMatchMaking009::AddRequestLobbyListCompatibleMembersFilter( CSteamID steamID )
{

}

SteamAPICall_t CSteamMatchMaking009::CreateLobby( ELobbyType eLobbyType, int cMaxMembers )
{
	SteamAPICall_t result = CSteamBase::RegisterCall();
	LobbyCreated_t* retvals = (LobbyCreated_t*)malloc(sizeof(LobbyCreated_t));
	CSteamID id = CSteamID( 1337132, 0x40000, k_EUniversePublic, k_EAccountTypeChat );
	

	retvals->m_eResult = k_EResultOK;
	retvals->m_ulSteamIDLobby = id.ConvertToUint64();

	//

	CSteamBase::ReturnCall(retvals, sizeof(LobbyCreated_t), LobbyCreated_t::k_iCallback, result);

	JoinLobby(id);

	return result;
}

SteamAPICall_t CSteamMatchMaking009::JoinLobby( CSteamID steamIDLobby )
{
	SteamAPICall_t result = CSteamBase::RegisterCall();
	LobbyEnter_t* retvals = (LobbyEnter_t*)malloc(sizeof(LobbyEnter_t));
	retvals->m_bLocked = false;
	retvals->m_EChatRoomEnterResponse = k_EChatRoomEnterResponseSuccess;
	retvals->m_rgfChatPermissions = (EChatPermission)0xFFFFFFFF;
	retvals->m_ulSteamIDLobby = steamIDLobby.ConvertToUint64();

	CSteamBase::ReturnCall(retvals, sizeof(LobbyEnter_t), LobbyEnter_t::k_iCallback, result);

	return result;
}

void CSteamMatchMaking009::LeaveLobby( CSteamID steamIDLobby )
{
	
}

bool CSteamMatchMaking009::InviteUserToLobby( CSteamID steamIDLobby, CSteamID steamIDInvitee )
{
	return true;
}

int CSteamMatchMaking009::GetNumLobbyMembers( CSteamID steamIDLobby )
{
	return 1;
}

const char * CSteamMatchMaking009::GetLobbyData( CSteamID steamIDLobby, const char *pchKey)
{
	return "";
}

bool CSteamMatchMaking009::SetLobbyData( CSteamID steamIDLobby, const char *pchKey, const char *pchValue )
{
	return true;
}

int CSteamMatchMaking009::GetLobbyDataCount( CSteamID steamIDLobby )
{
	return 0;
}

bool CSteamMatchMaking009::GetLobbyDataByIndex( CSteamID steamIDLobby, int iLobbyData, char *pchKey, int cchKeyBufferSize, char *pchValue, int cchValueBufferSize )
{
	return false;
}

bool CSteamMatchMaking009::DeleteLobbyData( CSteamID steamIDLobby, const char *pchKey )
{
	return 0;
}

const char * CSteamMatchMaking009::GetLobbyMemberData( CSteamID steamIDLobby, CSteamID steamIDUser, const char *pchKey )
{
	return 0;
}

void CSteamMatchMaking009::SetLobbyMemberData( CSteamID steamIDLobby, const char *pchKey, const char *pchValue )
{
	
}

bool CSteamMatchMaking009::SendLobbyChatMsg( CSteamID steamIDLobby, const void *pvMsgBody, int cubMsgBody )
{
return true;
}

int CSteamMatchMaking009::GetLobbyChatEntry( CSteamID steamIDLobby, int iChatID, CSteamID *pSteamIDUser, void *pvData, int cubData, EChatEntryType *peChatEntryType )
{
	return 0;
}

bool CSteamMatchMaking009::RequestLobbyData( CSteamID steamIDLobby )
{
	return false;
}

void CSteamMatchMaking009::SetLobbyGameServer( CSteamID steamIDLobby, uint32 unGameServerIP, uint16 unGameServerPort, CSteamID steamIDGameServer )
{
	
}

bool CSteamMatchMaking009::GetLobbyGameServer( CSteamID steamIDLobby, uint32 *punGameServerIP, uint16 *punGameServerPort, CSteamID *psteamIDGameServer )
{
	return false;
}

bool CSteamMatchMaking009::SetLobbyMemberLimit( CSteamID steamIDLobby, int cMaxMembers )
{
	return true;
}

int CSteamMatchMaking009::GetLobbyMemberLimit( CSteamID steamIDLobby )
{
	return 0;
}

bool CSteamMatchMaking009::SetLobbyType( CSteamID steamIDLobby, ELobbyType eLobbyType )
{
	return true;
}

bool CSteamMatchMaking009::SetLobbyJoinable( CSteamID steamIDLobby, bool bLobbyJoinable )
{
	return true;
}
CSteamID CSteamMatchMaking009::GetLobbyOwner( CSteamID steamIDLobby )
{
	return CSteamID((uint64)0x00015);
}
bool CSteamMatchMaking009::SetLobbyOwner( CSteamID steamIDLobby, CSteamID steamIDNewOwner )
{
	return true;
}

bool CSteamMatchMaking009::SetLinkedLobby( CSteamID steamIDLobby, CSteamID steamIDLobby2 )
{
	return true;
}

void CSteamMatchMaking009::AddRequestLobbyListDistanceFilter( ELobbyDistanceFilter eLobbyDistanceFilter )
{

}
