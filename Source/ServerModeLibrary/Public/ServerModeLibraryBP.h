// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ServerModeLibraryBP.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LOG_SERVERMODELIBRARYBP, Log, All)

/**
 *
 */
UCLASS()
class SERVERMODELIBRARY_API UServerModeLibrary
    : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString GetServerAddress(TSoftObjectPtr<UWorld> Map);
};
