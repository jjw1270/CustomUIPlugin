// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUI.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundCue.h"
#include "WidgetBase.generated.h"

UENUM(BlueprintType)
enum class EWidgetState : uint8
{
	Hide,
	OnShow,			// Start Anim 재생중
	Idle,
	OnHide,			// Hide Anim 재생중
};

UENUM(BlueprintType)
enum class EWidgetHideType : uint8
{
	NA					UMETA(Hidden),
	RemoveFromParent	UMETA(Hidden),
	Collapsed,
	Hidden
};

DECLARE_DELEGATE_OneParam(F_OnWidgetStateChanged, EWidgetState);
DECLARE_MULTICAST_DELEGATE_OneParam(FM_OnWidgetStateChanged, EWidgetState);
DECLARE_DYNAMIC_DELEGATE_OneParam(FD_OnWidgetStateChanged, EWidgetState, _old_state);

UCLASS(Abstract)
class CUSTOMUI_API UWidgetBase : public UUserWidget
{
	GENERATED_BODY()

private:
	// Widget Anims
	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> ShowAnim = nullptr;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> IdleAnim = nullptr;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> HideAnim = nullptr;

	UPROPERTY()
	TObjectPtr<UWidgetAnimation> _CurrentAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> _ShowSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundCue> _HideSound = nullptr;

private:
	EWidgetState _WidgetState = EWidgetState::Hide;

	EWidgetHideType _WidgetHideType = EWidgetHideType::NA;

	FM_OnWidgetStateChanged _OnWidgetStateChanged;

private:
	void SetWidgetState(EWidgetState _new_state);

public:
	UFUNCTION(BlueprintCallable)
	void Hide(EWidgetHideType _type, bool _force_immediately);

	UFUNCTION(BlueprintCallable)
	void Close(bool _force_immediately);

private:
	void HideWidget();

// Event
public:
	UFUNCTION(BlueprintCallable)
	void BindOnWidgetStateChanged(FD_OnWidgetStateChanged _proc);
	void BindOnWidgetStateChanged(F_OnWidgetStateChanged& _proc);

	UFUNCTION(BlueprintNativeEvent)
	void OnWidgetStateChanged(EWidgetState _old_state);
	virtual void OnWidgetStateChanged_Implementation(EWidgetState _old_state);

// Getters
public:
	UFUNCTION(BlueprintPure)
	EWidgetState GetWidgetState() const { return _WidgetState; }

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _delta) override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* _anim) override;
	virtual void SynchronizeProperties() override;

protected:
	UFUNCTION()
	virtual void OnVisibilityChanged(ESlateVisibility _visibility);

	// "BP"에서 변수에 변경이 있을 때 호출됩니다.
	UFUNCTION(BlueprintImplementableEvent, meta = (ForceAsFunction))
	void OnSynchronizeProperties();

};
