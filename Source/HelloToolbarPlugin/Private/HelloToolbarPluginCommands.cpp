// Copyright Epic Games, Inc. All Rights Reserved.

#include "HelloToolbarPluginCommands.h"

#define LOCTEXT_NAMESPACE "FHelloToolbarPluginModule"

void FHelloToolbarPluginCommands::RegisterCommands()
{
	UI_COMMAND(CreateNodeAction, "HelloToolbarPlugin", "Create Node", EUserInterfaceActionType::Button, FInputChord(EKeys::E, false, false, true, false));
	UI_COMMAND(LinkPreviousNodeAction, "HelloToolbarPlugin", "Link Previous Node", EUserInterfaceActionType::Button, FInputChord(EKeys::Q, false, false, true, false));
}

#undef LOCTEXT_NAMESPACE
