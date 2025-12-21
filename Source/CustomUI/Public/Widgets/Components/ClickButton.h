// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ButtonBase.h"
#include "ClickButton.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable)
class CUSTOMUI_API UClickButton : public UButtonBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Button")
	bool _UseSubClick = false;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> _SubClickSound = nullptr;

	static TSet<FKey> SubClickKeyList;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDM_OnDoubleClicked, UClickButton*, _btn);

	UPROPERTY(BlueprintAssignable)
	FDM_OnDoubleClicked _OnDoubleClicked;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDM_OnSubClicked, UClickButton*, _btn);

	UPROPERTY(BlueprintAssignable)
	FDM_OnSubClicked _OnSubClicked;

protected:
	virtual void NativeOnMouseEnter(const FGeometry& _geo, const FPointerEvent& _mouse_event) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& _mouse_event) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& _geo, const FPointerEvent& _mouse_event) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& _geo, const FPointerEvent& _mouse_event) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& _geo, const FPointerEvent& _mouse_event) override;

public:
	static void SetSubClickKeyList(const TSet<FKey>& _key_list);

};
