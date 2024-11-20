// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class ServerProcess {
public:
	ServerProcess(const FString& MapName, int32 Port = 7777);

public:
	void Kill();

public:
	~ServerProcess();

private:
	FProcHandle ProcessHandle;
};
