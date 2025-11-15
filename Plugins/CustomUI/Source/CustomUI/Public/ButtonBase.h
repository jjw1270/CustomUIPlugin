// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetBase.h"
#include "ButtonBase.generated.h"

UENUM(BlueprintType)
enum class EButtonType : uint8
{
	Click,
	Toggle
};

UENUM(BlueprintType)
enum class EButtonState : uint8
{
	Normal = 0,
	Hovered,
	Pressed,
	Disabled
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDM_OnButtonClicked, UButtonBase*, _btn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDM_OnButtonDoubleClicked, UButtonBase*, _btn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDM_OnButtonSubClicked, UButtonBase*, _btn);

UCLASS(Abstract)
class CUSTOMUI_API UButtonBase : public UWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class USizeBox> SizeBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<class UImage> Img_ButtonStyle = nullptr;

protected:
	UPROPERTY(EditAnywhere, Category = "Button")
	EButtonType _ButtonType = EButtonType::Click;

	UPROPERTY(EditAnywhere, Category = "Button")
	bool _UseSubClick = false;

	UPROPERTY(EditAnywhere, Category = "Button|FixedSize")
	bool _UseFixedSize = false;

	UPROPERTY(EditAnywhere, Category = "Button|FixedSize")
	FVector2D _FixedSize = FVector2D(100.0, 40.0f);

	UPROPERTY(EditAnywhere, Category = "Button")
	TMap<EButtonState, FSlateBrush> _ButtonStyle;

	// todo
	UPROPERTY(EditAnywhere, Category = "Button|Icon")
	bool _UseIcon = false;

	UPROPERTY(EditAnywhere, Category = "Button|Icon")
	TMap<EButtonState, FSlateBrush> _IconStyle;

	UPROPERTY(EditAnywhere, Category = "Button|Text")
	TMap<EButtonState, FText> _Text;


	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> _HoverSound = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> _ClickSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> _SubClickSound = nullptr;

	static TSet<FKey> ClickKeyList;
	static TSet<FKey> SubClickKeyList;

public:
	UPROPERTY(BlueprintAssignable)
	FDM_OnButtonClicked _OnButtonClicked;

	UPROPERTY(BlueprintAssignable, meta=(Tooltip="ButtonType = Click 타입만 가능"))
	FDM_OnButtonDoubleClicked _OnButtonDoubleClicked;

	UPROPERTY(BlueprintAssignable)
	FDM_OnButtonSubClicked _OnButtonSubClicked;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EButtonState _ButtonState = EButtonState::Normal;

protected:
	virtual void NativeConstruct() override;
	virtual void SynchronizeProperties() override;

	virtual void NativeOnMouseEnter(const FGeometry& _geo, const FPointerEvent& _mouse_event) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& _mouse_event) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& _geo, const FPointerEvent& _mouse_event) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& _geo, const FPointerEvent& _mouse_event) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& _geo, const FPointerEvent& _mouse_event) override;

public:
	UFUNCTION(BlueprintCallable)
	void ResetButton();

protected:
	void SetButtonState(EButtonState _state);
	void UpdateButtonStyle();

public:
	static void SetClickKeyList(const TSet<FKey>& _key_list);
	static void SetSubClickKeyList(const TSet<FKey>& _key_list);
};
