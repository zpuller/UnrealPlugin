// Copyright Epic Games, Inc. All Rights Reserved.

#include "HelloToolbarPluginStyle.h"
#include "HelloToolbarPlugin.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FHelloToolbarPluginStyle::StyleInstance = nullptr;

void FHelloToolbarPluginStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FHelloToolbarPluginStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FHelloToolbarPluginStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("HelloToolbarPluginStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FHelloToolbarPluginStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("HelloToolbarPluginStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("HelloToolbarPlugin")->GetBaseDir() / TEXT("Resources"));

	Style->Set("HelloToolbarPlugin.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FHelloToolbarPluginStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FHelloToolbarPluginStyle::Get()
{
	return *StyleInstance;
}
