// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PopupBase.h"


void UPopupBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);
}
