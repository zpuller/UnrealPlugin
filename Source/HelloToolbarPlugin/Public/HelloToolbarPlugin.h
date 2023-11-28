// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHello, VeryVerbose, All);

class FToolBarBuilder;
class FMenuBuilder;

class FHelloToolbarPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void CreateNodeImpl(const FEdGraphEditAction& InAction);
	
	/** This function will be bound to Command. */
	void HandleCreateNode();
	void HandleLinkPreviousNode();

	void MapCommands(UObject* Object, IAssetEditorInstance* Instance);
	void ExtendToolBar(IAssetEditorInstance* Instance);
	void FillToolbar(FToolBarBuilder& ToolbarBuilder);
	
private:

	void HandleAssetEditorOpened(UObject* Object, IAssetEditorInstance* Instance);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<FExtender> ToolbarExtender;

	bool AddedGraphCallback = false;
	UEdGraphNode* InputGraphNode = nullptr;
};
