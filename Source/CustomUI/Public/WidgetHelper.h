// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomUI.h"
#include "WidgetHelper.generated.h"

class UPageBase;
class UPopupBase;

UCLASS()
class CUSTOMUI_API UWidgetHelper : public UBlueprintFunctionLibrary
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
