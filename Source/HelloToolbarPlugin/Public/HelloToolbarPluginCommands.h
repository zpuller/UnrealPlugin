// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "HelloToolbarPluginStyle.h"

class FHelloToolbarPluginCommands : public TCommands<FHelloToolbarPluginCommands>
{
public:

	FHelloToolbarPluginCommands()
		: TCommands<FHelloToolbarPluginCommands>(TEXT("HelloToolbarPlugin"), NSLOCTEXT("Contexts", "HelloToolbarPlugin", "HelloToolbarPlugin Plugin"), NAME_None, FHelloToolbarPluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > CreateNodeAction;
	TSharedPtr< FUICommandInfo > LinkPreviousNodeAction;
};
