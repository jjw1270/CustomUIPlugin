// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBase.h"
#include "TabButton.h"
#include "TabBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDM_OnTabSelected, UTabBase*, _tab, FName, _tab_id);

UCLASS(Abstract)
class CUSTOMUI_API UTabBase : public UWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> UniformGridPanel = nullptr;

protected:
	UPROPERTY(EditAnywhere, Category = "Tab")
	TEnumAsByte<EOrientation> _Orientation;

	UPROPERTY(EditAnywhere, Category = "Tab")
	FMargin _ButtonPadding = FMargin();

	UPROPERTY(EditAnywhere, Category = "Tab")
	TSubclassOf<UTabButton> _TabButtonClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Tab")
	TMap<FName, FTabButtonConfig> _TabButtonMap;

protected:
	TOptional<EOrientation> _CurrentOrientation;

public:
	UPROPERTY(BlueprintAssignable)
	FDM_OnTabSelected _OnTabSelected;

protected:
	virtual void SynchronizeProperties() override;

public:
	void SelectTab(FName _tab_id);
	void SelectTabIndex(int32 _tab_idx);

protected:
	void CreateTabButtons();

	UFUNCTION()
		void OnClickTabButton(UButtonBase* _btn);
};
