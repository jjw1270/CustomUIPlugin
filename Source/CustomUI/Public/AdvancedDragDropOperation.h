// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "AdvancedDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMUI_API UAdvancedDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	FVector2D _LocalOffset = FVector2D::ZeroVector;
};
