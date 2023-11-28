#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHelloUtils, VeryVerbose, All);

class HelloToolbarUtils
{
public:
	static SGraphEditor* FindGraphEditorForTopLevelWindow();

	static void ConnectTwoPins(SGraphEditor* GraphEditor, const UEdGraphNode* SelectedGraphNode, const UEdGraphNode* InputGraphNode);

	static void ConnectTwoPins(SGraphEditor* GraphEditor);
	static void HighlightPin(SGraphEditor* GraphEditor, const UEdGraphNode* GraphNode, UEdGraphPin* GraphPin);
	static TArray<TObjectPtr<UEdGraphNode>> GetAllGraphNodes(SGraphEditor* GraphEditor);
	static void CreateActionMenuBySelectedNode(SGraphEditor* GraphEditor);

private:
	static void CreateActionMenuByNode(SGraphEditor* GraphEditor, UEdGraphNode* GraphNode);
};

