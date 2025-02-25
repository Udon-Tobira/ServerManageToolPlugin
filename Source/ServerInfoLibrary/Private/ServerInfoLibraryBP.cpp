// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerManageLibraryBP.h"

#include "ServerInfoSettings.h"

#if WITH_EDITOR
#include "ServerModeSetting.h"
#endif

DEFINE_LOG_CATEGORY(LOG_SERVERMANAGELIBRARYBP)

FString UServerManageLibrary::GetServerAddress(TSoftObjectPtr<UWorld> Map) {
	// if map is not valid
	if (Map.IsNull()) {
		UE_LOG(LOG_SERVERMANAGELIBRARYBP, Error, TEXT("Map %s doesn't exist."),
		       *Map.ToString());
		return "";
	}

	const auto& MapName = Map.GetLongPackageName();

	const auto* const ServerInfoSetting = GetDefault<UServerInfoSettings>();
	const auto&       ServerList        = ServerInfoSetting->ServerList;

	const auto& ServerMapSettingPtr = ServerList.FindByPredicate(
	    [&](const auto& ServerMap) { return MapName == ServerMap.MapName; });

	// if setting for the map doesn't exist
	if (!ServerMapSettingPtr) {
		UE_LOG(LOG_SERVERMANAGELIBRARYBP, Error,
		       TEXT("Server address for the Map %s doesn't exist."), *MapName);
		return "";
	}

#if WITH_EDITOR
	const auto* const ServerModeSetting = GetDefault<UServerModeSetting>();
	const auto&       ServerPlayMode    = ServerModeSetting->GetServerPlayMode();
	if (SERVER_LocalLaunch == ServerPlayMode) {
		const auto& MapIndex =
		    std::distance(&*ServerList.begin(), ServerMapSettingPtr);

		return FString::Printf(TEXT("127.0.0.1:%d"), 7777 + MapIndex);
	}
#endif

	return ServerMapSettingPtr->ServerAddress;
}
