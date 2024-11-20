// Copyright Epic Games, Inc. All Rights Reserved.

#include "ServerModePlayMenuStyle.h"
#include "ServerModePlayMenu.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FServerModePlayMenuStyle::StyleInstance = nullptr;

void FServerModePlayMenuStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FServerModePlayMenuStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FServerModePlayMenuStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("ServerModePlayMenuStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FServerModePlayMenuStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("ServerModePlayMenuStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("ServerModePlayMenu")->GetBaseDir() / TEXT("Resources"));

	Style->Set("ServerModePlayMenu.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FServerModePlayMenuStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FServerModePlayMenuStyle::Get()
{
	return *StyleInstance;
}
