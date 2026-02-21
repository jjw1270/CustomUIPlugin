// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "WidgetPlayerController.h"
#include "WidgetSubsystem.generated.h"

class UPageBase;
class UPopupBase;

UCLASS()
class CUSTOMUI_API UWidgetSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

friend class UWidgetHelper;

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

	UPROPERTY()
	TObjectPtr<UPageBase> _CurrentPage = nullptr;

	UPROPERTY()
	TSet<UPageBase*> _OpenedPageList; // 재사용을 위함. map jump시 초기화

protected:
	UPageBase* OpenPage(TSubclassOf<UPageBase> _page_class);
	UPageBase* FindOrCreatePage(TSubclassOf<UPageBase> _page_class);

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
	UPopupBase* OpenPopup(TSubclassOf<UPopupBase> _popup_class);

	UFUNCTION() void OnPopupClosed(UWidgetBase* _widget, bool _is_removed);

protected:
	UPopupBase* GetTopPopup() const;

	bool CheckCanOpenPopup(TSubclassOf<UPopupBase> _popup_class) const;
	bool IsPopupOpened(TSubclassOf<UPopupBase> _popup_class) const;

#pragma endregion Popup

};
