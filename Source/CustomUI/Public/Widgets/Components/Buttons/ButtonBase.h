// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "ButtonBase.generated.h"

UENUM(BlueprintType)
enum class EButtonState : uint8
{
	Normal = 0,
	Hovered,
	Pressed,
	Disabled
};


USTRUCT(BlueprintType)
struct FButtonTextConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FVector2D ShadowOffset = FVector2D();

	UPROPERTY(EditAnywhere)
	FLinearColor ShadowColor = FLinearColor();

	UPROPERTY(EditAnywhere)
	ETextTransformPolicy TransformPolicy = ETextTransformPolicy::None;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETextJustify::Type> Justification = ETextJustify::Center;
};

USTRUCT(BlueprintType)
struct FButtonStyleConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FSlateFontInfo Font = FSlateFontInfo();

	UPROPERTY(EditAnywhere)
	FSlateBrush Brush = FSlateBrush();

	UPROPERTY(EditAnywhere)
	FLinearColor ContentColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
};

UCLASS(Abstract, NotBlueprintable, HideDropdown)
class CUSTOMUI_API UButtonBase : public UWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class USizeBox> SizeBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<class UBorder> Border = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock = nullptr;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button")
	FText _ButtonText = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button")
	EButtonState _ButtonState = EButtonState::Normal;

	UPROPERTY(EditAnywhere, Category = "Button")
	bool _HandleButtonEvents = true;

	UPROPERTY(EditAnywhere, Category = "Button")
	bool _UseFixedSize = false;

	UPROPERTY(EditAnywhere, Category = "Button", meta = (EditCondition = "_UseFixedSize"))
	FVector2D _FixedSize = FVector2D(100.0, 40.0f);

	UPROPERTY(EditAnywhere, Category = "Button")
	FButtonTextConfig _TextConfig;

	UPROPERTY(EditAnywhere, Category = "Button")
	TMap<EButtonState, FButtonStyleConfig> _StateStyles;

	UPROPERTY(EditAnywhere, Category = "Button")
	FMargin _Padding = FMargin(8.0f, 4.0f);

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> _HoverSound = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> _ClickSound = nullptr;

	static TSet<FKey> ClickKeyList;

protected:
	virtual void NativeConstruct() override;
	virtual void SynchronizeProperties() override;

	virtual void SetIsEnabled(bool _is_enabled) override final;

	FReply GetReply() const
	{
		return _HandleButtonEvents ? FReply::Handled() : FReply::Unhandled();
	}

public:
	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "_text"))
	void SetText(const FText& _text);

	UFUNCTION(BlueprintCallable)
	void SetButtonDisabled(bool _is_disabled);

	UFUNCTION(BlueprintCallable)
	void ResetButtonState();

	static void SetClickKeyList(const TSet<FKey>& _key_list);

protected:
	void SetButtonState(EButtonState _state);
	virtual void UpdateButtonStyle();
};
