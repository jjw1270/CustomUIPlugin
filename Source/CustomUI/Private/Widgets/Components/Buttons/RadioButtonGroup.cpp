// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/Buttons/RadioButtonGroup.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void URadioButtonGroup::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (IsInvalid(UniformGridPanel))
		return;

	UniformGridPanel->SetSlotPadding(_ButtonPadding);

	if (!_CurrentOrientation.IsSet() || _Orientation != _CurrentOrientation.GetValue())
	{
		_CurrentOrientation = _Orientation;
		CreateRadioButtons();
	}
}

void URadioButtonGroup::SelectRadioButtonName(FName _btn_name)
{
	for (auto child : UniformGridPanel->GetAllChildren())
	{
		auto radio_btn = Cast<URadioButton>(child);
		if (IsValid(radio_btn))
		{
			radio_btn->SetIsSelected(radio_btn->GetRadioButtonName() == _btn_name);
		}
	}
}

void URadioButtonGroup::SelectRadioButtonIndex(int32 _btn_idx)
{
	if (_btn_idx < 0 || _btn_idx >= UniformGridPanel->GetChildrenCount())
	{
		TRACE_WARNING(TEXT("out of index : %d"), _btn_idx);
	}

	int32 idx = 0;
	for (auto child : UniformGridPanel->GetAllChildren())
	{
		auto radio_btn = Cast<URadioButton>(child);
		if (IsValid(radio_btn))
		{
			radio_btn->SetIsSelected(idx == _btn_idx);
			idx++;
		}
	}
}

void URadioButtonGroup::CreateRadioButtons()
{
	if (IsInvalid(_RadioButtonClass))
		return;

	UniformGridPanel->ClearChildren();

	int32 row = 0;
	int32 col = 0;
	for (const auto& btn_config : _RadioButtonConfigs)
	{
		auto radio_btn = CreateWidget<URadioButton>(this, _RadioButtonClass);
		if (IsValid(radio_btn))
		{
			radio_btn->_RadioButtonName = btn_config.Name;
			radio_btn->_SelectedText = btn_config.SelectedText;
			radio_btn->_UnselectedText = btn_config.UnselectedText;
			radio_btn->_SelectedStateStyles = btn_config.SelectedStateStyles;
			radio_btn->_StateStyles = btn_config.UnselectedStateStyles;

			radio_btn->_OnButtonClicked.AddDynamic(this, &URadioButtonGroup::OnClickRadioButton);

			auto slot = UniformGridPanel->AddChildToUniformGrid(radio_btn, row, col);
			if (IsValid(slot))
			{
				slot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				slot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

				if (_CurrentOrientation.GetValue() == EOrientation::Orient_Horizontal)
					col++;
				else
					row++;
			}
		}
	}
}

void URadioButtonGroup::OnClickRadioButton(URadioButton* _btn)
{
	for (auto child : UniformGridPanel->GetAllChildren())
	{
		auto radio_btn = Cast<URadioButton>(child);
		if (IsValid(radio_btn))
		{
			radio_btn->SetIsSelected(radio_btn == _btn);
		}
	}
}
