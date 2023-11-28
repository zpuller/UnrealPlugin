// Copyright Epic Games, Inc. All Rights Reserved.

#include "HelloToolbarPlugin.h"
#include "HelloToolbarPluginStyle.h"
#include "HelloToolbarPluginCommands.h"
#include "HelloToolbarUtils.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

#include <GraphEditor.h>
#include <SGraphPanel.h>
#include <SGraphNode.h>
#include <SGraphPin.h>

static const FName HelloToolbarPluginTabName("HelloToolbarPlugin");

#define LOCTEXT_NAMESPACE "FHelloToolbarPluginModule"

DEFINE_LOG_CATEGORY(LogHello);

void FHelloToolbarPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FHelloToolbarPluginStyle::Initialize();
	FHelloToolbarPluginStyle::ReloadTextures();

	FHelloToolbarPluginCommands::Register();
	
	//PluginCommands = MakeShareable(new FUICommandList);

	//PluginCommands->MapAction(
	//	FHelloToolbarPluginCommands::Get().PluginAction,
	//	FExecuteAction::CreateRaw(this, &FHelloToolbarPluginModule::PluginButtonClicked),
	//	FCanExecuteAction());

	if (GEditor)
	{
		GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OnAssetOpenedInEditor().AddRaw(this, &FHelloToolbarPluginModule::HandleAssetEditorOpened);
	}
}

void FHelloToolbarPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FHelloToolbarPluginStyle::Shutdown();

	FHelloToolbarPluginCommands::Unregister();
}

void FHelloToolbarPluginModule::CreateNodeImpl(const FEdGraphEditAction& InAction) {
	UE_LOG(LogHelloUtils, Log, TEXT("GraphChanged"));
	if (InAction.Action != EEdGraphActionType::GRAPHACTION_AddNode) {
		return;
	}
	SGraphEditor* GraphEditor = HelloToolbarUtils::FindGraphEditorForTopLevelWindow();
	if (GraphEditor != nullptr)
	{
		HelloToolbarUtils::ConnectTwoPins(GraphEditor, InAction.Nodes.Array()[0], InputGraphNode);
	}
}

void FHelloToolbarPluginModule::HandleCreateNode()
{
	SGraphEditor* GraphEditor = HelloToolbarUtils::FindGraphEditorForTopLevelWindow();
	if (GraphEditor == nullptr) {
		return;
	}

	InputGraphNode = nullptr;
	for (UObject* Node : GraphEditor->GetSelectedNodes())
	{
		UEdGraphNode* GraphNode = Cast<UEdGraphNode>(Node);
		if (GraphNode)
		{
			InputGraphNode = GraphNode;
			break;
		}
	}

	if (InputGraphNode == nullptr) {
		return;
	}

	if (!AddedGraphCallback) {
		AddedGraphCallback = true;

		GraphEditor->GetCurrentGraph()->AddOnGraphChangedHandler(FOnGraphChanged::FDelegate::CreateLambda([this](const FEdGraphEditAction& InAction) {
			this->CreateNodeImpl(InAction);
			}));
	}

	UE_LOG(LogHello, Log, TEXT("HandleCreateNode"));
	HelloToolbarUtils::CreateActionMenuBySelectedNode(GraphEditor);
}

void FHelloToolbarPluginModule::HandleLinkPreviousNode()
{
	UE_LOG(LogHello, Log, TEXT("HandleLinkPreviousNode"));
	SGraphEditor* GraphEditor = HelloToolbarUtils::FindGraphEditorForTopLevelWindow();
	if (GraphEditor != nullptr)
	{
		HelloToolbarUtils::ConnectTwoPins(GraphEditor);
	}
}

void FHelloToolbarPluginModule::HandleAssetEditorOpened(UObject* Object, IAssetEditorInstance* Instance)
{
	const FString ClassName(*Object->GetClass()->GetName());
	UE_LOG(LogHello, Log, TEXT("NewPlugin AssetEditorOpened for: %s"), *ClassName);

	if (ClassName == "Blueprint")
	{
		MapCommands(Object, Instance);
		//ExtendToolBar(Instance);
	}
}

void FHelloToolbarPluginModule::MapCommands(UObject* Object, IAssetEditorInstance* Instance)
{
	FAssetEditorToolkit* AssetEditorToolkit = StaticCast<FAssetEditorToolkit*>(Instance);

	const FHelloToolbarPluginCommands& Commands = FHelloToolbarPluginCommands::Get();
	TSharedRef<FUICommandList> ToolkitCommands = AssetEditorToolkit->GetToolkitCommands();
	if (ToolkitCommands->IsActionMapped(Commands.CreateNodeAction))
	{
		return;
	}
	ToolkitCommands->MapAction(
		FHelloToolbarPluginCommands::Get().CreateNodeAction,
		FExecuteAction::CreateRaw(this, &FHelloToolbarPluginModule::HandleCreateNode),
		FCanExecuteAction());
	ToolkitCommands->MapAction(
		FHelloToolbarPluginCommands::Get().LinkPreviousNodeAction,
		FExecuteAction::CreateRaw(this, &FHelloToolbarPluginModule::HandleLinkPreviousNode),
		FCanExecuteAction());
}

void FHelloToolbarPluginModule::ExtendToolBar(IAssetEditorInstance* Instance)
{
	FAssetEditorToolkit* AssetEditorToolkit = StaticCast<FAssetEditorToolkit*>(Instance);
	TSharedRef<FUICommandList> ToolkitCommands = AssetEditorToolkit->GetToolkitCommands();
	if (!ToolbarExtender.IsValid())
	{
		ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension(
			"Asset",
			EExtensionHook::After,
			ToolkitCommands,
			FToolBarExtensionDelegate::CreateRaw(this, &FHelloToolbarPluginModule::FillToolbar)
		);
	}
	AssetEditorToolkit->AddToolbarExtender(ToolbarExtender);
}

void FHelloToolbarPluginModule::FillToolbar(FToolBarBuilder& ToolbarBuilder)
{
    const FHelloToolbarPluginCommands& Commands = FHelloToolbarPluginCommands::Get();
    // clang-format off
    ToolbarBuilder.BeginSection("GraphFormatter");
    {
        ToolbarBuilder.AddToolBarButton(
            Commands.CreateNodeAction,
            NAME_None,
            TAttribute<FText>(),
            TAttribute<FText>(),
            TAttribute<FSlateIcon>(FSlateIcon(FHelloToolbarPluginStyle::Get().GetStyleSetName(), "HelloToolbarPlugin.PluginAction")),
            FName(TEXT("MyAction"))
        );
	}
    // clang-format on
    ToolbarBuilder.EndSection();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHelloToolbarPluginModule, HelloToolbarPlugin)