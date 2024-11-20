// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "ServerModeSetting.generated.h"

UENUM()
enum EServerPlayMode : int {
	/** Connect to the server specified in project settings */
	SERVER_ProjectSetting UMETA(DisplayName = "Project Setting"),
	/** Connect to a local server. The server starts automatically at startup */
	SERVER_LocalLaunch UMETA(DisplayName = "Local Launch")
};

/**
 *
 */
UCLASS(config = EditorPerProjectUserSettings, MinimalAPI)
class UServerModeSetting: public UObject {
	GENERATED_UCLASS_BODY()

public:
	void SetServerPlayMode(const EServerPlayMode InServerPlayMode) {
		ServerPlayMode = InServerPlayMode;
	}
	EServerPlayMode GetServerPlayMode() const {
		return ServerPlayMode;
	}

private:
	/** Server mode. */
	UPROPERTY(config, EditAnywhere, Category = "Multiplayer Options")
	TEnumAsByte<EServerPlayMode> ServerPlayMode;
};
