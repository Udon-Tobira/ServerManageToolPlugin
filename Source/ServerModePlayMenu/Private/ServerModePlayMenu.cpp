// Copyright Epic Games, Inc. All Rights Reserved.

#include "ServerModePlayMenu.h"

#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ServerInfoSettings.h"
#include "ServerModeSetting.h"
#include "ToolMenus.h"

static const FName ServerModePlayMenuTabName("ServerModePlayMenu");

#define LOCTEXT_NAMESPACE "FServerModePlayMenuModule"

class FInternalServerModeCommandCallbacks {
public:
	static int32 GetServerPlayMode();
	static void  SetServerPlayMode(int32 Value);
};

void FServerModePlayMenuModule::StartupModule() {
	UToolMenus::RegisterStartupCallback(
	    FSimpleMulticastDelegate::FDelegate::CreateRaw(
	        this, &FServerModePlayMenuModule::RegisterMenus));

	FEditorDelegates::BeginPIE.AddRaw(this,
	                                  &FServerModePlayMenuModule::OnBeginPIE);
	FEditorDelegates::EndPIE.AddRaw(this, &FServerModePlayMenuModule::OnEndPIE);

	RegisterSeverInfoSetting();
}

void FServerModePlayMenuModule::ShutdownModule() {
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	UnregisterSeverInfoSetting();
}

void FServerModePlayMenuModule::RegisterMenus() {
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	UToolMenu* PlayMenu =
	    UToolMenus::Get()->ExtendMenu("UnrealEd.PlayWorldCommands.PlayMenu");
	{
		FToolMenuSection& Section =
		    PlayMenu->FindOrAddSection("LevelEditorPlayInWindowNetwork");
		{
			Section.AddSubMenu(
			    "ServerMode", LOCTEXT("ServerModeMenu", "Server Mode"),
			    LOCTEXT("ServerModeToolTip", "The type of the server to connect to."),
			    FNewMenuDelegate::CreateLambda([](FMenuBuilder& InMenuBuilder) {
				    const UEnum* ServerPlayModeEnum = FindObject<UEnum>(
				        nullptr, TEXT("/Script/ServerModePlayMenu.EServerPlayMode"));

				    const auto NumPlayModes = ServerPlayModeEnum->NumEnums();
				    for (auto i = decltype(NumPlayModes){0}; i < NumPlayModes - 1;
				         ++i) {
					    FUIAction Action(
					        FExecuteAction::CreateStatic(
					            &FInternalServerModeCommandCallbacks::SetServerPlayMode,
					            i),
					        FCanExecuteAction(),
					        FIsActionChecked::CreateLambda(
					            [](int32 Index) {
						            return FInternalServerModeCommandCallbacks::
						                       GetServerPlayMode() == Index;
					            },
					            i));

					    InMenuBuilder.AddMenuEntry(
					        ServerPlayModeEnum->GetDisplayNameTextByIndex(i),
					        ServerPlayModeEnum->GetToolTipTextByIndex(i), FSlateIcon(),
					        Action, NAME_None, EUserInterfaceActionType::RadioButton);
				    }
			    }),
			    false);
		}
	}
}

void FServerModePlayMenuModule::OnBeginPIE(const bool bIsSimulating) {
	if (SERVER_LocalLaunch ==
	    FInternalServerModeCommandCallbacks::GetServerPlayMode()) {
		const auto* const ServerInfoConfig = GetDefault<UServerInfoSettings>();
		const auto&       ServerList       = ServerInfoConfig->ServerList;
		const auto&       NumServerList    = ServerList.Num();

		ensureAlways(ServerProcesses.empty());
		ServerProcesses.reserve(NumServerList);

		int32 Port = 7777;
		for (const auto& ServerMap : ServerList) {
			ServerProcesses.push_back(
			    std::make_shared<ServerProcess>(ServerMap.MapName, Port));
			++Port;
		}
	}
}

void FServerModePlayMenuModule::OnEndPIE(const bool bIsSimulating) {
	ServerProcesses.clear();
}

void FServerModePlayMenuModule::RegisterSeverInfoSetting() {
	if (ISettingsModule* SettingsModule =
	        FModuleManager::GetModulePtr<ISettingsModule>("Settings")) {
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings(
		    "Project", "Project", "zd", LOCTEXT("ServerInfoSettingName", "Servers"),
		    LOCTEXT("ServerInfoDescription", "Settings about servers"),
		    GetMutableDefault<UServerInfoSettings>());

		// if (SettingsSection.IsValid()) {
		//	SettingsSection->OnModified().BindRaw(
		//	    this, &FMySettingModule::HandleSettingsSaved);
		// }
	}
}

void FServerModePlayMenuModule::UnregisterSeverInfoSetting() {
	if (ISettingsModule* SettingsModule =
	        FModuleManager::GetModulePtr<ISettingsModule>("Settings")) {
		SettingsModule->UnregisterSettings("Project", "MySettingCategory",
		                                   "MySetting");
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FServerModePlayMenuModule, ServerModePlayMenu)

int32 FInternalServerModeCommandCallbacks::GetServerPlayMode() {
	const auto* const ServerModeSetting = GetDefault<UServerModeSetting>();
	return static_cast<int32>(ServerModeSetting->GetServerPlayMode());
}
void FInternalServerModeCommandCallbacks::SetServerPlayMode(const int32 Value) {
	auto* const ServerModeSetting = GetMutableDefault<UServerModeSetting>();
	ServerModeSetting->SetServerPlayMode(static_cast<EServerPlayMode>(Value));

	ServerModeSetting->PostEditChange();
	ServerModeSetting->SaveConfig();
}
