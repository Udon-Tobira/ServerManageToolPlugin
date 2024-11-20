// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "ServerProcess.h"

#include <memory>
#include <vector>

class FToolBarBuilder;
class FMenuBuilder;

class FServerModePlayMenuModule: public IModuleInterface {
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void        RegisterMenus();
	void        OnBeginPIE(bool bIsSimulating);
	void        OnEndPIE(bool bIsSimulating);
	static void RegisterSeverInfoSetting();
	static void UnregisterSeverInfoSetting();

private:
	std::vector<std::shared_ptr<ServerProcess>> ServerProcesses;
};
