// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ServerManageLibraryBP.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LOG_SERVERMANAGELIBRARYBP, Log, All)

/**
 *
 */
UCLASS()
class SERVERMANAGELIBRARY_API UServerManageLibrary
    : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString GetServerAddress(TSoftObjectPtr<UWorld> Map);

	UFUNCTION(BlueprintCallable)
	static void RequestServerExit();
};
