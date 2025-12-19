// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Components/Buttons/ButtonBase.h"
#include "ToggleButton.generated.h"

/**
 *
 */
UCLASS(Abstract, Blueprintable)
class CUSTOMUI_API UToggleButton : public UButtonBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ToggleButton")
	bool _IsSelected = false;

	UPROPERTY(EditAnywhere, Category = "ToggleButton")
	FText _SelectedText = FText();

	UPROPERTY(EditAnywhere, Category = "ToggleButton")
	FText _UnselectedText = FText();

	UPROPERTY(EditAnywhere, Category = "ToggleButton")
	TMap<EButtonState, FButtonStyleConfig> _SelectedStateStyles;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDM_OnButtonClicked, UToggleButton*, _btn, bool, _is_selected);

	UPROPERTY(BlueprintAssignable)
	FDM_OnButtonClicked _OnButtonClicked;
	
protected:
	virtual void SynchronizeProperties() override;

	virtual void NativeOnMouseEnter(const FGeometry& _geo, const FPointerEvent& _mouse_event) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& _mouse_event) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& _geo, const FPointerEvent& _mouse_event) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& _geo, const FPointerEvent& _mouse_event) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetIsSelected(bool _is_selected);

	UFUNCTION(BlueprintCallable)
	bool ToggleSelected();

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnSelectChanged();
	void OnSelectChanged_Implementation();

	virtual void UpdateButtonStyle() override;

public:
	UFUNCTION(BlueprintPure)
	bool GetIsSelected() const { return _IsSelected; }

};
