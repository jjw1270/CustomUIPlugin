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

friend class UWidgetHelpers;
friend class UPopupBase;

public:
	virtual void Initialize(FSubsystemCollectionBase& _collection) override;
	virtual void Deinitialize() override;
	virtual void PlayerControllerChanged(APlayerController* _new_pc) override;

private:
	AWidgetPlayerController* GetLocalPlayerController() const;

protected:
	void ClearAllWidgets(bool _clear_close_event);
	void RebuildWidgets(AWidgetPlayerController* _pc);

///////////////////////////////////////////////////////////////////
#pragma region Page
protected:
	UPROPERTY()
	TSubclassOf<UPageBase> _RemainingPageClass = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UPageBase> _CurrentPage = nullptr;

protected:
	UPageBase* CreatePage(TSubclassOf<UPageBase> _page_class);

// getter
protected:
	UPageBase* GetCurrentPage() const { return _CurrentPage; }

#pragma endregion Page

///////////////////////////////////////////////////////////////////
#pragma region Popup
protected:
	UPROPERTY()
	TArray<TSubclassOf<UPopupBase>> _RemainingPopupClasses;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UPopupBase>> _CurrentPopups;

protected:
	UPopupBase* CreatePopup(TSubclassOf<UPopupBase> _popup_class);

	UFUNCTION() void OnPopupClosed(UWidgetBase* _widget, bool _is_removed);

protected:
	UPopupBase* GetTopPopup() const;

	bool CanOpenPopup(TSubclassOf<UPopupBase> _popup_class) const;
	bool IsPopupOpened(TSubclassOf<UPopupBase> _popup_class) const;

#pragma endregion Popup

};

UCLASS()
class CUSTOMUI_API UWidgetHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "_world_ctx"))
	static UPageBase* OpenPage(const UObject* _world_ctx, TSubclassOf<UPageBase> _page_class);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "_world_ctx"))
	static UPopupBase* OpenPopup(const UObject* _world_ctx, TSubclassOf<UPopupBase> _popup_class);

	UFUNCTION(BlueprintCallable)
	static void ClosePopup(UPopupBase* _popup, bool _force_immediately);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "_world_ctx"))
	static UPopupBase* GetTopPopup(const UObject* _world_ctx);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "_widget"))
	static bool IsDesignTime(const UUserWidget* _widget);
};
