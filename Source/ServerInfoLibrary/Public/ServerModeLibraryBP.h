// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ServerInfoLibraryBP.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LOG_SERVERINFOLIBRARYBP, Log, All)

/**
 *
 */
UCLASS()
class SERVERINFOLIBRARY_API UServerInfoLibrary
    : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString GetServerAddress(TSoftObjectPtr<UWorld> Map);
};
