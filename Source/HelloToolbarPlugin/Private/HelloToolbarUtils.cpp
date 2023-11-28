#include "HelloToolbarUtils.h"

#include <GraphEditor.h>
#include <SGraphPanel.h>
#include <SGraphNode.h>
#include <SGraphPin.h>

DEFINE_LOG_CATEGORY(LogHelloUtils);

struct FWidgetTypeMatcher
{
	FWidgetTypeMatcher(const FName& InType)
		: TypeName(InType)
	{
	}

	bool IsMatch(const TSharedRef<const SWidget>& InWidget) const
	{
		return TypeName == InWidget->GetType();
	}

	const FName& TypeName;
};

SGraphEditor* HelloToolbarUtils::FindGraphEditorForTopLevelWindow() {
	FSlateApplication& Application = FSlateApplication::Get();
	auto ActiveWindow = Application.GetActiveTopLevelWindow();
	if (!ActiveWindow.IsValid())
	{
		return nullptr;
	}
	FGeometry InnerWindowGeometry = ActiveWindow->GetWindowGeometryInWindow();
	FArrangedChildren JustWindow(EVisibility::Visible);
	JustWindow.AddWidget(FArrangedWidget(ActiveWindow.ToSharedRef(), InnerWindowGeometry));

	FWidgetPath WidgetPath(ActiveWindow.ToSharedRef(), JustWindow);
	if (WidgetPath.ExtendPathTo(FWidgetTypeMatcher("SGraphEditor"), EVisibility::Visible))
	{
		return StaticCast<SGraphEditor*>(&WidgetPath.GetLastWidget().Get());
	}
	return nullptr;
}

void HelloToolbarUtils::ConnectTwoPins(SGraphEditor* GraphEditor, const UEdGraphNode* SelectedGraphNode, const UEdGraphNode* InputGraphNode) {
	if (InputGraphNode == nullptr) {
		return;
	}

	UEdGraphPin* LeftPin = nullptr;
	const FString Then = "then";
	LeftPin = InputGraphNode->FindPin(Then);

	UEdGraphPin* RightPin = nullptr;
	const FString Execute = "execute";
	RightPin = SelectedGraphNode->FindPin(Execute);

	if (LeftPin && RightPin) {
		LeftPin->MakeLinkTo(RightPin);
	}
}

void HelloToolbarUtils::ConnectTwoPins(SGraphEditor* GraphEditor) {
	UEdGraphNode* SelectedGraphNode = nullptr;
	UEdGraphNode* InputGraphNode = nullptr;
	SGraphPanel* GraphPanel = GraphEditor->GetGraphPanel();

	for (UObject* Node : GraphEditor->GetSelectedNodes())
	{
		UEdGraphNode* GraphNode = Cast<UEdGraphNode>(Node);
		if (GraphNode)
		{
			SelectedGraphNode = GraphNode;
			break;
		}
	}

	if (SelectedGraphNode == nullptr) {
		return;
	}

	SGraphNode& SelectedGraphNodeWidget = *GraphPanel->GetNodeWidgetFromGuid(SelectedGraphNode->NodeGuid);
	FGeometry SelectedGeometry = SelectedGraphNodeWidget.GetCachedGeometry();
	FVector2D SelectedTopLeft = SelectedGeometry.GetLocalPositionAtCoordinates(FVector2D(0, 0));
	FVector2D SelectedBottomLeft = SelectedGeometry.GetLocalPositionAtCoordinates(FVector2D(0, 1));

	UE_LOG(LogHelloUtils, Log, TEXT("SelectedTopLeft: %f, %f"), SelectedTopLeft.X, SelectedTopLeft.Y);
	UE_LOG(LogHelloUtils, Log, TEXT("SelectedBottmLeft: %f, %f"), SelectedBottomLeft.X, SelectedBottomLeft.Y);

	for (auto Obj : GetAllGraphNodes(GraphEditor)) {
		if (UEdGraphNode* GraphNode = static_cast<UEdGraphNode*>(Obj.Get())) {
			if (GraphNode == SelectedGraphNode) {
				continue;
			}

			SGraphNode& GraphNodeWidget = *GraphPanel->GetNodeWidgetFromGuid(GraphNode->NodeGuid);
			FGeometry Geometry = GraphNodeWidget.GetCachedGeometry();
			FVector2D TopLeft = Geometry.GetLocalPositionAtCoordinates(FVector2D(0, 0));
			FVector2D BottomLeft = Geometry.GetLocalPositionAtCoordinates(FVector2D(0, 1));

			UE_LOG(LogHelloUtils, Log, TEXT("TopLeft: %f, %f"), TopLeft.X, TopLeft.Y);
			UE_LOG(LogHelloUtils, Log, TEXT("BottmLeft: %f, %f"), BottomLeft.X, BottomLeft.Y);

			double MaxTop = FMath::Max(TopLeft.Y, SelectedTopLeft.Y);
			double MinBottom = FMath::Min(BottomLeft.Y, SelectedBottomLeft.Y);

			if (MaxTop <= MinBottom) {
				InputGraphNode = GraphNode;
				break;
			}
		}
	}

	if (InputGraphNode == nullptr) {
		return;
	}

	UEdGraphPin* LeftPin = nullptr;
	const FString Then = "then";
	LeftPin = InputGraphNode->FindPin(Then);

	UEdGraphPin* RightPin = nullptr;
	const FString Execute = "execute";
	RightPin = SelectedGraphNode->FindPin(Execute);

	if (LeftPin && RightPin) {
		HighlightPin(GraphEditor, InputGraphNode, LeftPin);
		HighlightPin(GraphEditor, SelectedGraphNode, RightPin);
		LeftPin->MakeLinkTo(RightPin);
	}
}

void HelloToolbarUtils::HighlightPin(SGraphEditor* GraphEditor, const UEdGraphNode* GraphNode, UEdGraphPin* GraphPin) {
	SGraphPanel* GraphPanel = GraphEditor->GetGraphPanel();
	SGraphNode& GraphNodeWidget = *GraphPanel->GetNodeWidgetFromGuid(GraphNode->NodeGuid);
	SGraphPin& GraphPinWidget = *GraphNodeWidget.FindWidgetForPin(GraphPin);
	GraphPinWidget.SetRenderOpacity(0.5);
}

//void foo(const FEdGraphEditAction& InAction) {
//	UE_LOG(LogHelloUtils, Log, TEXT("GraphChanged"));
//}

void HelloToolbarUtils::CreateActionMenuByNode(SGraphEditor* GraphEditor, UEdGraphNode* GraphNode)
{
	//GraphEditor->GetCurrentGraph()->AddOnGraphChangedHandler(FOnGraphChanged::FDelegate::CreateLambda([](const FEdGraphEditAction& InAction) {
	//	foo(InAction);
	//	}));

	SGraphPanel* GraphPanel = GraphEditor->GetGraphPanel();
	SGraphNode& GraphNodeWidget = *GraphPanel->GetNodeWidgetFromGuid(GraphNode->NodeGuid);
	FVector2D NodePos = GraphNodeWidget.GetCachedGeometry().GetAbsolutePosition();
	int NodeSizeX = GraphNodeWidget.GetCachedGeometry().GetAbsoluteSize().X;
	FSlateApplication& Application = FSlateApplication::Get();
	Application.OnMouseUp(EMouseButtons::Right, FVector2D(NodePos.X + NodeSizeX + 200, NodePos.Y));
}

TArray<TObjectPtr<UEdGraphNode>> HelloToolbarUtils::GetAllGraphNodes(SGraphEditor* GraphEditor) {
	return GraphEditor->GetCurrentGraph()->Nodes;
}

void HelloToolbarUtils::CreateActionMenuBySelectedNode(SGraphEditor* GraphEditor)
{
	for (UObject* Node : GraphEditor->GetSelectedNodes())
	{
		UEdGraphNode* GraphNode = Cast<UEdGraphNode>(Node);
		if (GraphNode)
		{
			FString Name;
			GraphNode->GetName(Name);

			UE_LOG(LogHelloUtils, Log, TEXT("GraphNode found: %s"), *Name);

			CreateActionMenuByNode(GraphEditor, GraphNode);
			return;
		}
	}
}
