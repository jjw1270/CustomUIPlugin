// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "Widgets/Components/Buttons/RadioButton.h"
#include "RadioButtonGroup.generated.h"


USTRUCT(BlueprintType)
struct FRadioButtonConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName Name = FName();

	UPROPERTY(EditAnywhere)
	FText SelectedText = FText();

	UPROPERTY(EditAnywhere)
	FText UnselectedText = FText();

	UPROPERTY(EditAnywhere)
	TMap<EButtonState, FButtonStyleConfig> SelectedStateStyles;

	UPROPERTY(EditAnywhere)
	TMap<EButtonState, FButtonStyleConfig> UnselectedStateStyles;
};

UCLASS(Abstract)
class CUSTOMUI_API URadioButtonGroup : public UWidgetBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> UniformGridPanel = nullptr;

protected:
	UPROPERTY(EditAnywhere, Category = "RadioButtonGroup")
	TEnumAsByte<EOrientation> _Orientation;

	UPROPERTY(EditAnywhere, Category = "RadioButtonGroup")
	FMargin _ButtonPadding = FMargin();

	UPROPERTY(EditAnywhere, Category = "RadioButtonGroup")
	TSubclassOf<URadioButton> _RadioButtonClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "RadioButtonGroup")
	TArray<FRadioButtonConfig> _RadioButtonConfigs;

protected:
	TOptional<EOrientation> _CurrentOrientation;

protected:
	virtual void SynchronizeProperties() override;

public:
	UFUNCTION(BlueprintCallable)
	void SelectRadioButtonName(FName _btn_name);

	UFUNCTION(BlueprintCallable)
	void SelectRadioButtonIndex(int32 _btn_idx);

protected:
	void CreateRadioButtons();

	UFUNCTION()
	void OnClickRadioButton(URadioButton* _btn);
};
