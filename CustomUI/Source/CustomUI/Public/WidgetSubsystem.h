// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetPlayerController.h"
#include "Widgets/PageBase.h"
#include "Widgets/PopupBase.h"
#include "WidgetSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMUI_API UWidgetSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& _collection) override;
	virtual void Deinitialize() override;
	virtual void PlayerControllerChanged(APlayerController* _new_pc) override;

protected:
	void ClearAllWidgets();
	void RebuildWidgets(AWidgetPlayerController* _pc);

///////////////////////////////////////////////////////////////////
#pragma region Page
protected:
	UPROPERTY()
	TSubclassOf<UPageBase> _RemainingPageClass = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UPageBase> _CurrentPage = nullptr;

public:
	void CreatePage(AWidgetPlayerController* _pc, TSubclassOf<UPageBase> _page_class);

public:
	UFUNCTION(BlueprintCallable)
	UPageBase* GetCurrentPage() const { return _CurrentPage; }

#pragma endregion Page

///////////////////////////////////////////////////////////////////
#pragma region Popup
protected:
	UPROPERTY()
	TArray<TSubclassOf<UPopupBase>> _RemainingPopupClasses;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UPopupBase>> _CurrentPopups;

public:
	void CreatePopup(AWidgetPlayerController* _pc, TSubclassOf<UPopupBase> _popup_class);

#pragma endregion Popup 

};

UCLASS()
class UWidgetHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void OpenPage(const UObject* _world_ctx, TSubclassOf<UPageBase> _page_class);

	UFUNCTION(BlueprintCallable)
	static void OpenPopup(const UObject* _world_ctx, TSubclassOf<UPopupBase> _popup_class);
};
