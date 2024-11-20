// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerProcess.h"

ServerProcess::ServerProcess(const FString& MapName, int32 Port) {
	// UnrealEditor.exe のパスを取得
	FString EditorPath = FPlatformProcess::ExecutablePath();

	// 現在開いているプロジェクトのパスを取得
	FString ProjectFilePath = FPaths::GetProjectFilePath();

	if (!FPaths::FileExists(EditorPath)) {
		UE_LOG(LogTemp, Error, TEXT("UnrealEditor.exe was not found at: %s"),
		       *EditorPath);
		return;
	}

	if (!FPaths::FileExists(ProjectFilePath)) {
		UE_LOG(LogTemp, Error, TEXT("uproject was not found at: %s"),
		       *ProjectFilePath);
		return;
	}

	// コマンドライン引数を作成
	FString CommandLineArgs =
	    FString::Printf(TEXT("\"%s\" %s -Port=%d -server -log"), *ProjectFilePath,
	                    *MapName, Port);

	// プロセスを起動
	ProcessHandle =
	    FPlatformProcess::CreateProc(*EditorPath, *CommandLineArgs, true, false,
	                                 false, nullptr, 0, nullptr, nullptr);

	if (!ProcessHandle.IsValid()) {
		UE_LOG(LogTemp, Error, TEXT("UnrealEditor プロセスの起動に失敗しました。"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("UnrealEditor プロセスを起動しました: %s %s"),
	       *EditorPath, *CommandLineArgs);
}

void ServerProcess::Kill() {
	if (ProcessHandle.IsValid()) {
		FPlatformProcess::TerminateProc(ProcessHandle);
		FPlatformProcess::CloseProc(ProcessHandle);
		ProcessHandle.Reset();
		UE_LOG(LogTemp, Log, TEXT("UnrealEditor プロセスを終了しました。"));
	}
}

ServerProcess::~ServerProcess() {
	Kill();
}
